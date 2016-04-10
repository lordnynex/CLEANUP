#ifndef SMPPGATEMANAGER_H
#define SMPPGATEMANAGER_H

#include "PGSql.h"
#include "SMPPGate.h"
#include "SMPPGateFilterParser.h"
#include "SMSMessage.h"
#include "SMSRequest.h"
#include "Timer.h"
#include "MessagesQueue.h"

#include <map>

#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>


namespace sms {

    class NoMoreGates: public FailureError {};
    class NoUnderLoadedGates: public RetryError {};
    class NoEnabledGates: public RetryError {};
    class AllGatesSuspended: public RetryError {};

    class SMPPGateManager {
    public:
        SMPPGateManager();
        ~SMPPGateManager();
        typedef std::map< std::string, SMPPGate > SMPPGatesMap;

        static SMPPGateManager* Instance() {
            if (!pInstance_)
                pInstance_ = new SMPPGateManager;
            return pInstance_;
        }

        void send( SMSRequest::PTR req, SMSMessage::ID msgid, string gname );
        void pushToQueue( SMSRequest::PTR req, SMSMessage::ID msgid );
        bool canAccept( string gName, SMSRequest::PTR req, SMSMessage::ID msgid );
        void gatesMapUpdate();
        void timeToSend();
        SMPPGatesMap getGates();

    private:
        static SMPPGateManager* pInstance_;
        std::list< string > chooseGate( SMSRequest::PTR req, SMSMessage::ID msgid );
        string suggestUrl( std::string sg, SMSMessage::ID msgid );
        bool isExpiredACK( const msgqueue::MessageInfo& mi);
        bool isExpired( const msgqueue::MessageInfo& mi);

        PGSql& db;
        msgqueue::MessagesQueue mqueue;
        boost::recursive_mutex lock;
        boost::recursive_mutex squeue_lock;
        boost::mt19937 gen;

        SMPPGatesMap gmap;
        int timerid;
        int updateid;
        int sendid;
        bool isSending;

        boost::shared_ptr< HttpClient > kannel;
    };

}
#endif // SMPPGATEMANAGER_H
