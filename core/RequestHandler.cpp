/*
 * RequestHandler.cpp
 *
 *  Created on: 22.01.2010
 *      Author: mohtep
 */

#include <iostream>
#include <Wt/WApplication>
#include <Wt/Http/Request>

#include <boost/thread/mutex.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>



#include "RequestHandler.h"
#include "RequestTracker.h"
#include "utils.h"
#include "PartnerManager.h"

using namespace std;
using namespace sms;
using namespace sms::utils;

RequestHandler::RequestHandler(Wt::WObject *parent) : Wt::WResource(parent) {
}

RequestHandler::~RequestHandler() {
	beingDeleted();
}

void RequestHandler::handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response) {

        response.setMimeType("text/xml");

	SMSRequest* req = new SMSRequest();
	const string e = "";
        const string zero = "0";

        RequestTracker *trck = RequestTracker::Instance();
        ostringstream o, out;

	const string uname		= request.getParameter("user")		? *request.getParameter("user")		: e;
	const string pass		= request.getParameter("pass")		? *request.getParameter("pass")		: e;
        string to			= request.getParameter("to")		? *request.getParameter("to")           : e;
        if ( to.empty() )    to		= request.getParameter("phone")		? *request.getParameter("phone")	: e;
              string txt		= request.getParameter("txt")  		? *request.getParameter("txt")		: e;

	const string charset		= request.getParameter("charset")  	? *request.getParameter("charset")	: e;
	const string tid		= request.getParameter("tid")  		? *request.getParameter("tid")		: e;
	const string sn 		= request.getParameter("sn") 		? *request.getParameter("sn")		: e;
        const string from		= request.getParameter("from") 		? *request.getParameter("from")		: sn;
              string utf		= request.getParameter("utf") 		? *request.getParameter("utf")		: zero;
	const string subpref            = request.getParameter("subpref") 	? *request.getParameter("subpref")	: e;
        const string hex		= request.getParameter("hex") 		? *request.getParameter("hex")		: zero;
              string udh		= request.getParameter("udh") 		? *request.getParameter("udh")		: e;
              string delay              = request.getParameter("delay") 	? *request.getParameter("delay")	: zero;
        const string date		= request.getParameter("datetime")   	? *request.getParameter("datetime")     : e;
              string tz 		= request.getParameter("tz")       	? *request.getParameter("tz")           : e;
        const string dlr		= request.getParameter("dlr")  		? *request.getParameter("dlr")		: zero;
              string pid		= request.getParameter("idp")  		? *request.getParameter("idp")		: e;
        const string piduser		= request.getParameter("idpuser")  	? *request.getParameter("idpuser")		: e;
        const int    priority           = request.getParameter("priority")  	? atoi( request.getParameter("priority")->c_str() ) : 0;
        const string garant		= request.getParameter("garant")  	? *request.getParameter("garant")	: zero;

	to_vec tov;
	Tokenize( to, tov, ",");

	if ( !charset.empty() ) {
		try {
			txt = StringRecodeFromTo( txt, charset, "UTF-8" );
		} catch ( ... ) {
			Logger::get_mutable_instance().smslogwarn( "Invalid encoding" );
			return;
		}
                utf = "1";
	}

        PartnerInfo ptnr;
        try {
            ptnr = PartnerManager::get_mutable_instance().findByName( uname );
            pid = ptnr.pId;
        } catch ( PartnerNotFoundError& e ) {
            Logger::get_mutable_instance().smslogwarn( string("Unknown user is requesting status: [") + uname + string(":") + pass + "]");
            return;
        }

        if ( ptnr.ownerId.empty() && !piduser.empty() ) {
            try {
                ptnr = PartnerManager::get_mutable_instance().findByName( piduser );
                pid = ptnr.pId;
            } catch ( PartnerNotFoundError& e ) {
                Logger::get_mutable_instance().smslogwarn( string("Unknown user is requesting status: [") + uname + string(":") + pass + "]");
                return;
            }
        }

        if ( ptnr.ownerId.empty() && !pid.empty() ) {
            try {
                ptnr = PartnerManager::get_mutable_instance().findById( pid );
                pid = ptnr.pId;
            } catch ( PartnerNotFoundError& e ) {
                Logger::get_mutable_instance().smslogwarn( string("Unknown user is requesting status: [") + uname + string(":") + pass + "]");
                return;
            }
        }

        try {
            if ( tz.empty() ) {
                PartnerInfo ptnr = PartnerManager::get_mutable_instance().findById( pid );
                tz = boost::lexical_cast< std::string >( ptnr.tzone );
            } else {
                tz = boost::lexical_cast< std::string >( 4 + boost::lexical_cast< int >( tz ) );
            }
        } catch ( ... ) {
            tz = "4";
        }

        if ( !date.empty() ) {
            try {
                boost::xtime now;
                boost::xtime_get( &now, boost::TIME_UTC_ );

                int kdelay = utils::datetime2ts( date, boost::lexical_cast< int >( tz ) ) - now.sec;
                if ( ( kdelay > 0 ) && ( delay == "0" ) )
                    delay = boost::lexical_cast< std::string >( kdelay );
            } catch ( ... ) {

            }
        }

        udh = utils::String2Hex( udh );
        udh = utils::Hex2String( udh );
        udh = utils::String2Hex( udh );

        boost::xtime now;
        boost::xtime_get( &now, boost::TIME_UTC_ );

        req->parse( uname, pass, tov, txt, tid, from, utf, subpref, hex, udh, delay, dlr, pid, priority, garant, now.sec );
        SMSRequest::PTR reqptr = SMSRequest::PTR( req );
        
	if ( req->getErr().getCode() != ERR_OK ) {
                out << "Received message request with an error: " << req->getErr().getDescr() << " ";
                out << request.queryString() << "\n";
                out << "user=" << uname << "\n";
                out << "from=" << from << "\n";
                out << "to=" << to << "\n";
                out << "hex=" << hex << "\n";
                Logger::get_mutable_instance().smsloginfo( out.str() );

                response.out() << reqptr->genReport();
                return;
	}

	
	SMSRequest::ID reqID;        
        o << "Received message request ID=" << req->getID() << " ";
        to.empty() ? : o << "to=" << to << " ";
        req->tid.empty()? : o << "tid=" << req->tid << " ";
        req->from.empty()? : o << "from=" << req->from << " ";
        try {
            reqID = trck->registerRequest( reqptr );
            o << "parsed;";
            Logger::get_mutable_instance().smsloginfo( o.str() );
        } catch (...) {
            req->setState( ERR_SYSTEM, "" );
            o << "UNKNOWN ERROR";
            Logger::get_mutable_instance().smslogerr( string("err ") + o.str() );
        }

        response.out() << reqptr->genReport();
}


RequestHandlerSMST::RequestHandlerSMST(Wt::WObject *parent) : Wt::WResource(parent) {
}

RequestHandlerSMST::~RequestHandlerSMST() {
        beingDeleted();
}

void RequestHandlerSMST::handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response) {

//        response.setMimeType("text/xml");

//        SMSRequest* req = new SMSRequest();
//        const string e = "";
//        const string zero = "0";
//        string pid = "";

//        RequestTracker *trck = RequestTracker::Instance();

//        ostringstream o, out;

//        const string operation		= request.getParameter("operation")	? *request.getParameter("operation")	: e;
//        const string uname		= request.getParameter("login")		? *request.getParameter("login")	: e;
//        const string pass		= request.getParameter("password")	? *request.getParameter("password")	: e;

//        if ( operation == "status" ) {
//            bool    auth = false;
//            int     err_code = 0;
//            string  err_descr = "OK";
//            SMSMessage::Status code = 0;
//            const string e = "";

//            const string id		= request.getParameter("sms_id")  	? *request.getParameter("sms_id")	: "";

//            try {
//                PartnerInfo ptrn = PartnerManager::get_mutable_instance().findByName( uname );
//                if ( pass == ptrn.pPass ) {
//                    auth = true;
//                }
//            } catch ( PartnerNotFoundError& e ) {
//                Logger::get_mutable_instance().smslogwarn( string("Unknown user is requesting status: [") + uname + string(":") + pass + "]");
//            }

//            std::vector< std::string > res;

//            if (!auth) {
//                err_code = -1;
//                err_descr = "Authorization failed";
//            }

//            if (id.length() > 100) {
//                err_code = -1;
//                err_descr = "Wrong sms-id";
//            }

//            Tokenize(id, res, ".");
//            if (res.size() != 2) {
//                err_code = -1;
//                err_descr = "Wrong sms-id";
//            }


//            if ( ( auth ) && ( !err_code ) )
//            try {
//                SMSMessage::PTR msg;
//                SMSRequest::ID reqid = atoll( res[0].c_str() );
//                int msg_num = atoll( res[1].c_str() );
//                SMSMessage::ID msg_id( reqid, msg_num );


//                msg = SMSMessageManager::get_mutable_instance().loadMessage(msg_id);

//                code = msg->getStatus();


//                boost::gregorian::date orig(1970, boost::gregorian::Jan, 1);
//                boost::posix_time::ptime dts(orig, boost::posix_time::seconds( msg->getHistory().begin()->when + 4*60*60 ));
//                boost::posix_time::ptime dtd(orig, boost::posix_time::seconds( msg->getHistory().begin()->when + 4*60*60 ));
//                boost::posix_time::ptime dtl(orig, boost::posix_time::seconds( (--msg->getHistory().end())->when + 4*60*60 ));

//                string sdts = boost::posix_time::to_iso_extended_string( dts ); sdts[10] = ' ';
//                string sdtd = boost::posix_time::to_iso_extended_string( dtd ); sdtd[10] = ' ';
//                string sdtl = boost::posix_time::to_iso_extended_string( dtl ); sdtl[10] = ' ';

//                string dlr_descr;
//                switch ( code() ) {
//                    case SMSMessage::Status::ST_BUFFERED:
//                        dlr_descr = "Buffered SMSC"; break;

//                    case SMSMessage::Status::ST_PREPARING:
//                        dlr_descr = "Buffered SMSC"; break;

//                    case SMSMessage::Status::ST_DELIVERED:
//                        dlr_descr = "Delivered"; break;

//                    case SMSMessage::Status::ST_NOT_DELIVERED:
//                        dlr_descr = "Non Delivered"; break;

//                    case SMSMessage::Status::ST_UNKNOWN:
//                        dlr_descr = "Unknown status"; break;

//                    case SMSMessage::Status::ST_ABSENT:
//                        dlr_descr = "Expired"; break;

//                    case SMSMessage::Status::ST_EXPIRED:
//                        dlr_descr = "Expired"; break;

//                    case SMSMessage::Status::ST_REJECTED:
//                        dlr_descr = "Rejected"; break;

//                }

//                out     << "<?xml version=\"1.0\" ?>"
//                        << "<reply>"
//                        << "<submission_date>" << sdts << "</submission_date>"
//                        << "<send_date>" << sdtd << "</send_date>"
//                        << "<last_status_change_date>" << sdtl << "</last_status_change_date>"
//                        << "<status>" << dlr_descr << "</status>"
//                        << "<error></error>"
//                        << "</reply>";

//                response.out() << out.str();
//                return;
//            } catch ( ... ) {
//                err_code = -1;
//                err_descr = "Internal error";
//            }


//            out     << "<?xml version=\"1.0\" ?>"
//                    << "<reply>"
//                    << "<result>" << "ERROR" << "</result>"
//                    << "<code>" << 1 << "</code>"
//                    << "<description>" << "no such message or this message does not belong to you" << "</description>"
//                    << "</reply>";

//            response.out() << out.str();
//            return;
//        }

//        string to			= request.getParameter("phones")	? *request.getParameter("phones")       : e;
//        const string txt		= request.getParameter("message") 	? *request.getParameter("message")	: e;
//        const string from		= request.getParameter("originator") 	? *request.getParameter("originator")	: e;
//              string utf		= request.getParameter("rus")    	? *request.getParameter("rus")  	: zero;

//        if ( utf == "5" ) { utf = "1"; } else { utf = "0"; }

//        to_vec tov;
//        Tokenize( to, tov, ",");

//        try {
//            PartnerInfo ptnr = PartnerManager::get_mutable_instance().findByName( uname );
//            if ( ptnr.pId != "1" ) { pid = ptnr.pId; }
//        } catch ( PartnerNotFoundError& e ) {
//            Logger::get_mutable_instance().smslogwarn( string("Unknown user is requesting status: [") + uname + string(":") + pass + "]");
//        }

//        req->parse( uname, pass, tov, txt, "", from, utf, "", "0", "", "0", "1", pid, 0, "0" );
//        SMSRequest::PTR reqptr = SMSRequest::PTR( req );


//        if ( req->getErr().getCode() != ERR_OK ) {
//                out << "Received message request with an error: " << req->getErr().getDescr() << " ";
//                out << request.queryString() << "\n";
//                out << "user=" << uname << "\n";
//                out << "from=" << from << "\n";
//                out << "to=" << to << "\n";
//                Logger::get_mutable_instance().smsloginfo( out.str() );

//                response.out() << reqptr->genReportSMST();
//                return;
//        }


//        SMSRequest::ID reqID;
//        o << "Received message request ID=" << req->getID() << " ";
//        to.empty() ? : o << "to=" << to << " ";
//        req->tid.empty()? : o << "tid=" << req->tid << " ";
//        req->from.empty()? : o << "from=" << req->from << " ";
//        try {
//            reqID = trck->registerRequest( reqptr );
//            o << "parsed;";
//            Logger::get_mutable_instance().smsloginfo( o.str() );
//        } catch (...) {
//            req->setState( ERR_SYSTEM, "" );
//            o << "UNKNOWN ERROR";
//            Logger::get_mutable_instance().smslogerr(o.str() );
//        }

//        response.out() << reqptr->genReportSMST();
}
