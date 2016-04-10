#ifndef MESSAGESQUEUE_H
#define MESSAGESQUEUE_H

#include "SMSMessage.h"
#include "SMSRequest.h"

#include <boost/thread/recursive_mutex.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

namespace msgqueue {

    using namespace sms;
    using namespace boost::multi_index;
    using namespace boost;
    using namespace std;

    struct MessageInfo {
        SMSMessage::ID msgid;
        SMSRequest::PTR req;
        long added;
        int parts;
        int partner_priority;
        int message_priority;
        string to;
        string txt;
        string gateway;
        string idp;
        list< string > gateways;
    };

    struct IDX_ID{};
    struct SEQUENCE{};
    struct IDX_ADDED_ASC{};
    struct IDX_ADDED_DESC{};
    struct IDX_PARTNER_PRIORITY{};
    struct IDX_PARTNER{};
    struct IDX_MESSAGE_PRIORITY{};
    struct IDX_GATEWAY{};

    typedef multi_index_container<
            MessageInfo,
            indexed_by<
                sequenced< tag<SEQUENCE> >,
                hashed_unique< tag<IDX_ID>, BOOST_MULTI_INDEX_MEMBER(MessageInfo, SMSMessage::ID, msgid), sms::MsgIDHash >,
                hashed_non_unique< tag<IDX_PARTNER>, BOOST_MULTI_INDEX_MEMBER(MessageInfo, string, idp) >,
                ordered_non_unique< tag<IDX_ADDED_ASC>, BOOST_MULTI_INDEX_MEMBER(MessageInfo, long, added), std::greater<long> >,
                ordered_non_unique< tag<IDX_ADDED_DESC>, BOOST_MULTI_INDEX_MEMBER(MessageInfo, long, added), std::less<long> >,
                ordered_non_unique< tag<IDX_PARTNER_PRIORITY>, BOOST_MULTI_INDEX_MEMBER(MessageInfo, int, partner_priority), std::less<int> >,
                ordered_non_unique< tag<IDX_MESSAGE_PRIORITY>, BOOST_MULTI_INDEX_MEMBER(MessageInfo, int, message_priority), std::less<int> >,
                ordered_non_unique< tag<IDX_GATEWAY>, BOOST_MULTI_INDEX_MEMBER(MessageInfo, string, gateway) >
            >
        > MessageInfoQueue;

    /*
     MessageFilter is a mechanism to create custom selects from messages heap
     It accepts inpute sequence, creates some transformations and returns it back
     MesasgeFilters can be joined into filter queue
     for example:
                    mqueue.select(
                        msgqueue::FilterBY< msgqueue::IDX_GATEWAY, string >( itg->first ) <<
                        msgqueue::OrderBY< msgqueue::IDX_MESSAGE_PRIORITY >() <<
                        msgqueue::OrderBY< msgqueue::IDX_PARTNER_PRIORITY >() <<
                        msgqueue::LimitBY( 100 )
                                );
     */
    class MessageFilter {
        typedef list< function<MessageInfoQueue( MessageInfoQueue& )> > TFuncList;
        TFuncList flist;
    public:
        MessageFilter() {}
        MessageFilter( const function<MessageInfoQueue( MessageInfoQueue& )>& r ) { flist.push_back( r ); }
        MessageFilter& operator << ( const MessageFilter& r );
        MessageInfoQueue apply( const MessageInfoQueue& q ) const;
    };

    // This filter apply strict order to a sequence
    template < class INDEX >
    struct OrderBY: public MessageFilter {
        static MessageInfoQueue filt( const MessageInfoQueue& q ) {
            MessageInfoQueue res;
            res.get<SEQUENCE>().insert( res.get<SEQUENCE>().begin(), q.get<INDEX>().begin(), q.get<INDEX>().end() );
            return res;
        }
        OrderBY(): MessageFilter( boost::bind( &OrderBY< INDEX >::filt, _1 ) ) {}
    };

    // This filter accepts value a paramater. If value fits the element is accepted
    template < class INDEX, class ValueType >
    struct FilterBY: public MessageFilter {
        static MessageInfoQueue filt( const MessageInfoQueue& q, const ValueType& v ) {
            MessageInfoQueue res, queue( q );
            while ( queue.get<INDEX>().find(v) != queue.get<INDEX>().end() ) {
                res.get<SEQUENCE>().push_back( *queue.get<INDEX>().find(v) );
                queue.get<INDEX>().erase( queue.get<INDEX>().find(v) );
            }
//            res.get<SEQUENCE>().insert( res.get<SEQUENCE>().begin(), q.get<INDEX>().begin(), q.get<INDEX>().end() );
            return res;
        }
        FilterBY( ValueType v ): MessageFilter( boost::bind( &FilterBY< INDEX, ValueType >::filt, _1, v ) ) {}
    };

    // This filter accepts function as a paramater. If function returns "true" the element is accepted
    struct FilterBYExpression: public MessageFilter {
        static MessageInfoQueue filt( const MessageInfoQueue& q, boost::function< bool( const MessageInfo& ) > func ) {
            MessageInfoQueue res;
            typedef boost::multi_index::index< MessageInfoQueue, SEQUENCE >::type typeT;
            typeT::const_iterator it;
            for ( it = q.get< SEQUENCE >().begin(); it != q.get< SEQUENCE >().end(); it++ ) {
                if ( func(*it) ) {
                    res.get<SEQUENCE>().push_back( *it );
                }
            }
            return res;
        }
        FilterBYExpression( boost::function< bool( const MessageInfo& ) > v ): MessageFilter( boost::bind( &FilterBYExpression::filt, _1, v ) ) {}
    };

    // This filter restores not more then N elements from sequence
    struct LimitBY: public MessageFilter {
        static MessageInfoQueue filt( const MessageInfoQueue& q, const int& n ) {
            MessageInfoQueue res;
            int i = 0;
            typedef boost::multi_index::index< MessageInfoQueue, SEQUENCE >::type typeT;
            typeT::const_iterator it;
            for ( it = q.get< SEQUENCE >().begin(); ( it != q.get< SEQUENCE >().end() ) && ( i < n ); it++ ) {
                MessageInfo msi = *it;
                res.get<SEQUENCE>().push_back( msi );
            }
            return res;
        }
        LimitBY( int n ): MessageFilter( boost::bind( &LimitBY::filt, _1, n ) ) {}
    };

    class MessagesQueue: public boost::noncopyable {
        MessageInfoQueue data;
        boost::recursive_mutex mq_lock;
    public:
        MessagesQueue() {}
        void insert( SMSRequest::PTR req, SMSMessage::ID msgid, list< string > gateways );
        int size() { return data.size(); }

        list< MessageInfo > select( const MessageFilter& filt );
    };


}

#endif // MESSAGESQUEUE_H

