/*
 * RequestTracker.h
 *
 *  Created on: 22.01.2010
 *      Author: mohtep
 */

#ifndef REQUESTTRACKER_H_
#define REQUESTTRACKER_H_

#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>
#include <Wt/WLogger>
#include <sstream>
#include <utility>

#include "SMSMessage.h"
#include "SMSRequest.h"
#include "PriorityQueue.h"
#include "LimitQueue.h"
#include "Cache.h"
#include "DelayQueue.h"
#include "Operation.h"
#include "PGSql.h"
#include "HttpClient.h"
#include "SMPPGate.h"
#include "MessagesQueue.h"

using namespace sms;

class RequestTracker {
public:
    struct DeliveryInfo {
        std::string gate;
        SMSMessage::Status status;

        DeliveryInfo( SMSMessage::Status _status, const std::string& _gate ) {
            gate = _gate;
            status = _status;
        }
    };

    enum OP_Code {
        OP_NewRequest,
        OP_NewMessage,
        OP_SendMessage,
        OP_SubmitMessage,
        OP_NewDelivery,
        OP_CheckDelivery,
        OP_CheckACK,
        OP_NewHistoryElement,
        OP_DeliveryNotify
    };

    enum OP_Priority {
        OP_NewRequestP = 100,
        OP_NewMessageP = 99,
        OP_SendMessageP = 50,
        OP_SubmitMessageP = 80,
        OP_NewDeliveryP = 98,
        OP_CheckDeliveryP = 51,
        OP_CheckACKP = 2,
        OP_NewHistoryElementP = 97,
        OP_DeliveryNotifyP = 1
    };

    typedef Operation<
    Vector9<
    SMSRequest::PTR,
    std::pair< SMSRequest::PTR, SMSMessage::ID >,
    std::pair< SMSRequest::PTR, SMSMessage::ID >,
    std::pair< SMSRequest::PTR, std::pair< SMSMessage::ID, string > >,
    std::pair< SMSMessage::ID, DeliveryInfo >,
    std::pair< SMSRequest::PTR, SMSMessage::ID >,
    std::pair< std::pair< SMSRequest::PTR, SMSMessage::ID >, string >,
    std::pair< SMSMessage::ID, SMSMessage::HistoryElement >,
    SMSMessage::ID
    >::Type
    > SMSOperation;

    RequestTracker();
    virtual ~RequestTracker();

    SMSRequest::ID registerRequest( SMSRequest::PTR req );
    void registerDeliveryNotification( SMSMessage::ID, SMSMessage::Status code, std::string gate );

    static RequestTracker* Instance() {
        if (!pInstance_)
            pInstance_ = new RequestTracker;
        return pInstance_;
    }

    SMSRequest::PTR loadRequestFromDb( SMSRequest::ID reqid );

    void MainEventLoop();
    void DelayedEventLoop();
    void OutboxPartnerEventLoop();
    void OutboxEventLoop();


    typedef SCache< SMSRequest::PTR, SMSRequest::ID > TReqCache;
    typedef SDelayQueue< SMSOperation > TDelQueue;
    typedef SPriorityQueue< SMSOperation > TOpQueue;
    typedef SLimitQueue< std::pair< SMSRequest::PTR, SMSMessage::ID > > TPartnersLimitQueue;
    typedef boost::shared_ptr< TPartnersLimitQueue > TPartnersLimitQueuePTR;
    typedef SPriorityQueue< std::pair< SMSRequest::PTR, SMSMessage::ID > > TOutBoxQueue;
    typedef boost::shared_ptr< TOutBoxQueue > TOutBoxQueuePTR;
    typedef map< string, TPartnersLimitQueuePTR > TPartnersLimitMap;
    TReqCache req_cache;
    TDelQueue del_queue;
    TOpQueue op_queue;
    TPartnersLimitMap pl_map;
    TOutBoxQueuePTR out_queue;

    const std::string kserver;
    const std::string kport;

    void parseMarkUndeliveredEvent( SMSRequest::PTR req, SMSMessage::ID msg );
    HttpClient client;
private:
    HttpClient kannel;
    PGSql& db;

    static RequestTracker* pInstance_;


    boost::recursive_mutex lock;

    void parseNewRequestEvent( SMSRequest::PTR req );
    void parseNewMessageEvent( SMSRequest::PTR req, SMSMessage::ID msg );
    void parseMessage2kannelEvent( SMSRequest::PTR req, SMSMessage::ID msg );
    void parseCheckDeliveryEvent( SMSRequest::PTR req, SMSMessage::ID msg );
    void parseCheckAckEvent( SMSRequest::PTR req, SMSMessage::ID msg, string gname );
    void parseDeliveryNotify( SMSMessage::ID msg );
    void parseNewDelivery( SMSMessage::ID msg_id, DeliveryInfo info );
    void parseNewHistoryElement( SMSMessage::ID msg_id, SMSMessage::HistoryElement element );
    void pushToSendingQueue( SMSRequest::PTR req, SMSMessage::ID msg, unsigned int ma_p = 0, unsigned char mi_p = 0 );

    void deliverUndelivered();
    void markUndelivered();


    friend class MonitoringHandler;
};

#endif /* REQUESTTRACKER_H_ */
