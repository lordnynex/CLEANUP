/*
 * File:   SMSMessage.h
 * Author: mohtep
 *
 * Created on 17 Февраль 2010 г., 15:19
 */

#ifndef _SMSMESSAGE_H
#define	_SMSMESSAGE_H

/*
 SMSMessage - this class is designed to provide information about SMS in system
 SMSMessage::ID
 SMSMessage::Status
 SMSMessage::HistoryElement

 PTR - this class is designed to create an auto-locked SMSMessage pointer

 SMSMessageManager - this class controls the creation, synchronization to DB, and caching of SNSMessage

 */


#include <boost/shared_ptr.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/noncopyable.hpp>
#include <boost/serialization/singleton.hpp>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>

#include <cstdio>
#include <list>

#include "SMSRequest.h"
#include "MessageClassifier.h"
#include "Error.h"
#include "Operation.h"

namespace sms {

    using namespace boost::multi_index;

    class MessageNotFoundError: public CriticalError {};
    class MessageAlreadyExistsError: public CriticalError {};

    struct SMSTax {
        std::string name;
        int length;
        double price;

        SMSTax( std::string _name, int _length, double _price ): name(_name),length(_length),price(_price){}
        SMSTax() {}
    };

    namespace message {

        class Status {
        public:

            enum __STATUS {
                ST_REJECTED = -3,
                ST_EXPIRED = -2,
                ST_NOT_DELIVERED = -1,
                ST_DELIVERED = 0,
                ST_BUFFERED = 1,
                ST_ABSENT = 2,
                ST_PREPARING = 3,
                ST_UNKNOWN = 4,
                ST_BILLED = 5,
                ST_PAID = 6,
                ST_CANCELED = 7
            };

            static std::string statusDescr( const Status& st ) {
                switch (st.value) {
                case ST_UNKNOWN:
                    return "ST_UNKNOWN";
                case ST_REJECTED:
                    return "ST_REJECTED";
                case ST_PREPARING:
                    return "ST_PREPARING";
                case ST_BUFFERED:
                    return "ST_BUFFERED";
                case ST_ABSENT:
                    return "ST_ABSENT";
                case ST_DELIVERED:
                    return "ST_DELIVERED";
                case ST_NOT_DELIVERED:
                    return "ST_NOT_DELIVERED";
                case ST_EXPIRED:
                    return "ST_EXPIRED";
                case ST_BILLED:
                    return "ST_BILLED";
                case ST_PAID:
                    return "ST_PAID";
                case ST_CANCELED:
                    return "ST_CANCELED";
                }
            }

            static std::string russianDescr( const Status& st ) {
                switch (st.value) {
                case ST_UNKNOWN:
                    return "Ожидает отправки";
                case ST_REJECTED:
                    return "Неверный номер";
                case ST_PREPARING:
                    return "Подготовка";
                case ST_BUFFERED:
                    return "Отправлено";
                case ST_ABSENT:
                    return "Вне зоны";
                case ST_DELIVERED:
                    return "Доставлено";
                case ST_NOT_DELIVERED:
                    return "Не доставлено";
                case ST_EXPIRED:
                    return "Просрочено";
                case ST_BILLED:
                    return "Счет выставлен";
                case ST_PAID:
                    return "Оплачено";
                case ST_CANCELED:
                    return "Отказ оплаты";
                }
            }

            Status & operator=( const Status& orig ) {
                value = orig.value;
                return *this;
            }

            Status( const int& _value ) {
                value = static_cast<__STATUS>( _value );
            }

            Status() {
                value = ST_UNKNOWN;
            }

            int operator() () const {
                return value;
            }

            bool operator<(const Status& r) const {
                return ( priority(value) < priority(r.value));
            }

            bool operator==(const Status& r) const {
                return ( priority(value) == priority(r.value));
            }

            bool operator!=(const Status& r) const {
                return ( priority(value) != priority(r.value));
            }

        private:
            __STATUS value;

            int priority(__STATUS _value) const {
                switch (_value) {
                case ST_UNKNOWN:
                    return -1;
                case ST_PREPARING:
                    return 10;
                case ST_BUFFERED:
                    return 11;
                case ST_ABSENT:
                    return 12;
                case ST_DELIVERED:
                    return 100;
                case ST_NOT_DELIVERED:
                    return 99;
                case ST_REJECTED:
                    return 98;
                case ST_EXPIRED:
                    return 97;
                case ST_BILLED:
                    return 200;
                case ST_PAID:
                    return 202;
                case ST_CANCELED:
                    return 201;

                }
                return 0;
            }
        };

        struct ID {
            SMSRequest::ID req;
            unsigned int msg_num;

            std::string to_str() const {
                char buf[100];
                sprintf(buf, "%lld.%d", req, msg_num);
                return std::string(buf);
            }

            bool operator<(const ID&r) const {
                return ( req*1000+msg_num < r.req*1000+r.msg_num );
            }

            bool operator==(const ID&r) const {
                return ( ( req == r.req ) && ( msg_num == r.msg_num ) );
            }

            bool operator!=(const ID&r) const {
                return ( ( req != r.req ) || ( msg_num != r.msg_num ) );
            }

            ID() {}

            ID(SMSRequest::ID _req, unsigned int _msg_num) {
                req = _req;
                msg_num = _msg_num;
            }
        };

        struct HistoryElement {
            int op_code;
            int op_direction;
            Status op_result;
            std::string gateway;
            long long when;

            HistoryElement( int _op_code, int _op_direction, Status _op_result, std::string _gateway, long long _when ) {
                op_code = _op_code;
                op_direction = _op_direction;
                op_result = _op_result;
                gateway = _gateway;
                when = _when;
            }
        };

    }

    namespace message {
        class PTR;
    }

    class SMSMessage: public boost::noncopyable, public SMSRequest {
    public:
        typedef std::list< message::HistoryElement > HistoryType;
        typedef message::HistoryElement HistoryElement;
        typedef message::ID ID;
        typedef message::Status Status;
        typedef message::PTR PTR;

        typedef Operation<
            Vector3<
                SMSMessage::ID,
                SMSMessage::ID,
                SMSMessage::HistoryElement
            >::Type
        > SMSSyncOperation;

        ID getID() const;
        HistoryType getHistory() const;
        void addHistoryElement( const HistoryElement& el );

        Status getStatus() const;
        std::string getPhone() const;
        MessageClassifier::CountryInfo getMsgClass() const;
        void setStatus( Status st );
        int getParts() const { return parts; }
        std::map< std::string, SMSTax > taxes_map;
        friend class msg_compare;
        // SMSMessageManager needs access to private members such as construcotr
        friend class SMSMessageManager;
    private:
        enum OP_MsgSync {
            OP_AddMessageToDB,
            OP_UpdateMessageToDB,
            OP_AddHistoryDetail
        };


        HistoryType history;
        Status delivery_status;
        SMSMessage::ID msg_id;
        MessageClassifier::CountryInfo msgClass;
        std::list< SMSSyncOperation > op_history;

        // We declare private constructor to prevent manual creating of SMSMessage instances
        SMSMessage( ID id, SMSRequest req );
        static SMSMessage* loadMsgFromDb( SMSMessage::ID msgid );
        void saveToDb( );
        void updateMessageToDb( );
        void addMessageHistoryToDb( const SMSMessage::HistoryElement& el );
    };


    // This struct declares the hash function for SMSMessage::ID
    struct MsgIDHash {
        std::size_t operator()(const SMSMessage::ID& key)const {
            return boost::hash<uint64_t>()(key.req) + boost::hash<uint32_t>()(key.msg_num);
        }
    };

    // SMSMessageManager is loaded once at program startup
    class SMSMessageManager: public boost::serialization::singleton< SMSMessageManager > {
        // Declare a struct to be hold in SMSMessage container
        struct SMSMessageInfo: public boost::noncopyable {
            SMSMessage::ID msgid;
            boost::shared_ptr< SMSMessage> msgptr;
            bool dirty;
            int last_updated;
            boost::recursive_mutex info_lock;
        };
        typedef boost::shared_ptr< SMSMessageInfo > SMSMessageInfoPTR;
        boost::recursive_mutex msgdata_lock;

        struct tag_id {};       // Quick access by SMSMessage::ID
        struct tag_dirty {};    // Access to messages witch are not synchronized
        struct tag_update {};   // Order by last access
        struct change_dirty {
          change_dirty(const bool& new_val):new_val(new_val){}
          void operator()(SMSMessageInfoPTR& e) {
            e->dirty=new_val;
          }
        private:
          bool new_val;
        };
        typedef boost::multi_index::multi_index_container<
                SMSMessageInfoPTR,
                indexed_by<
                    hashed_unique< tag<tag_id>, BOOST_MULTI_INDEX_MEMBER(SMSMessageInfo, SMSMessage::ID, msgid), MsgIDHash >,
                    ordered_non_unique< tag<tag_dirty>, BOOST_MULTI_INDEX_MEMBER(SMSMessageInfo, bool, dirty), std::greater<bool> >,
                    ordered_non_unique< tag<tag_update>, BOOST_MULTI_INDEX_MEMBER(SMSMessageInfo, int, last_updated) >
                >
            > TMsgData;
        typedef boost::multi_index::index<TMsgData, tag_id>::type TMsgDataHashIndex;
        typedef boost::multi_index::index<TMsgData, tag_dirty>::type TMsgDataDirtyIndex;
        typedef boost::multi_index::index<TMsgData, tag_update>::type TMsgDataUpdateIndex;
        TMsgData msg_data;
        //std::map< SMSMessage::ID, boost::shared_ptr< SMSMessageInfo > > msg_data;

    public:
        SMSMessageManager();
        SMSMessage::PTR loadMessage( SMSMessage::ID msgid ) throw ( MessageNotFoundError );
        void lockSMSMessage( SMSMessage::ID msgid );
        void unlockSMSMessage( SMSMessage::ID msgid );
        void createMessage( SMSMessage::ID msgid, SMSRequest req ) throw ( MessageAlreadyExistsError );
        void sync();
        void cleanup();
        bool setDirty( SMSMessage::ID, bool val );
        int count_dirty();
        int count();
    };

    namespace message {
        class PTR: public boost::shared_ptr< SMSMessage > {
        public:
            PTR(  ) {}
            PTR( SMSMessage* msg ): boost::shared_ptr< SMSMessage >( msg ) {
                if ( msg )
                    SMSMessageManager::get_mutable_instance().lockSMSMessage( msg->getID() );
            }
            PTR( boost::shared_ptr< SMSMessage >& msg ): boost::shared_ptr< SMSMessage >( msg ) {
                if ( msg.get() )
                    SMSMessageManager::get_mutable_instance().lockSMSMessage( msg->getID() );
            }
            PTR( const PTR& origptr ): boost::shared_ptr< SMSMessage >( origptr ) {
                SMSMessage* msg = origptr.get();
                if ( msg )
                    SMSMessageManager::get_mutable_instance().lockSMSMessage( msg->getID() );
            }
            PTR& operator= ( const PTR& origptr ) {
                if ( this == &origptr ) {
                    return *this;
                }

		if ( this->get() ) 
                    SMSMessageManager::get_mutable_instance().unlockSMSMessage( this->get()->getID() );
		
                boost::shared_ptr< SMSMessage >::operator=( origptr );

                if ( get() )
                    SMSMessageManager::get_mutable_instance().lockSMSMessage( this->get()->getID() );

                return *this;
            }
            ~PTR() {
                SMSMessage* msg = this->get();
                if ( msg )
                    SMSMessageManager::get_mutable_instance().unlockSMSMessage( msg->getID() );
            }
        };
    }

}

#endif	/* _SMSMESSAGE_H */

