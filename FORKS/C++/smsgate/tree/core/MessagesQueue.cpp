#include "MessagesQueue.h"
#include "SMSMessage.h"
#include "SMPPGateManager.h"
#include "PartnerManager.h"

namespace msgqueue {

    MessageFilter& MessageFilter::operator << ( const MessageFilter& r ) {
        flist.insert( flist.begin(), r.flist.begin(), r.flist.end() );
        return *this;
    }

    MessageInfoQueue MessageFilter::apply( const MessageInfoQueue& q ) const {
        MessageInfoQueue res = q;
        for ( TFuncList::const_iterator it = flist.begin(); it != flist.end(); it++ ) {
            res = (*it)( res );
        }
        return res;
    }

    list< MessageInfo > MessagesQueue::select( const MessageFilter& filt ) {
        recursive_mutex::scoped_lock lck( mq_lock );

        MessageInfoQueue q = filt.apply( this->data );
        list< MessageInfo > msglist;

        msglist.insert( msglist.begin(), q.get< SEQUENCE >().begin(), q.get< SEQUENCE >().end() );

        for ( list< MessageInfo >::const_iterator it = msglist.begin(); it != msglist.end(); it++  ) {
            data.get< IDX_ID >().erase( it->msgid );
        }

        return msglist;
    }

    void MessagesQueue::insert( SMSRequest::PTR req, SMSMessage::ID msgid, list< string > gateways ) {
        xtime now;
        xtime_get( &now, TIME_UTC );

        MessageInfo mi;
        mi.msgid = msgid;
        mi.gateway = *gateways.begin();
        mi.added= now.sec;
        try {
            mi.partner_priority = PartnerManager::get_mutable_instance().findById( req->pid ).pPriority;
        } catch (const PartnerNotFoundError&) {
            mi.partner_priority = 0;
        }
        mi.message_priority;
        mi.idp = req->pid;
        mi.txt = req->msg;
        mi.to = req->to[ msgid.msg_num ];
        mi.gateways = gateways;
        mi.req = req;

        recursive_mutex::scoped_lock lck( mq_lock );
        data.get< SEQUENCE >().push_back( mi );
    }
}
