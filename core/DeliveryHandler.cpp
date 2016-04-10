/*
 * DeliveryHandler.cpp
 *
 *  Created on: 15.02.2010
 *      Author: mohtep
 */

#include <vector>
#include <sstream>

#include "DeliveryHandler.h"
#include "RequestTracker.h"

#include "utils.h"

using std::string;

using namespace sms;
using namespace sms::utils;

DeliveryHandler::DeliveryHandler(Wt::WObject *parent) : Wt::WResource(parent) {
	logger.addField("message", true);
	logger.setStream( std::cout );
}

DeliveryHandler::~DeliveryHandler() {
	beingDeleted();
}

void DeliveryHandler::handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response) {
        response.setMimeType("text/html");

        Logger::get_mutable_instance().smslogwarn( "DeliveryHandler::handleRequest started" );

        const string e = "";

        const string pass		= request.getParameter("pass")		? *request.getParameter("pass")		: "";
        const string result		= request.getParameter("result")	? *request.getParameter("result")	: "";
        const string descr		= request.getParameter("descr")		? *request.getParameter("descr")	: "";
        const string gate		= request.getParameter("gate")		? *request.getParameter("gate")         : "";
        const string id			= request.getParameter("id")  		? *request.getParameter("id")		: "";


        std::ostringstream out;

        out << "Received delivery message ";

        RequestTracker *trck = RequestTracker::Instance();
        std::vector< std::string > res;

        if (id.length() > 100) {
                out << "Invalid ID ";
                Logger::get_mutable_instance().smslogerr( out.str() );
		return;
        }

        Tokenize(id, res, ".");
        if (res.size() != 2) {
                out << "Invalid ID ";
                Logger::get_mutable_instance().smslogerr( out.str() );
                return;
        }

        try {
            SMSRequest::ID reqid = atoll( res[0].c_str() );
            int msg_num = atoll( res[1].c_str() );
            int code = atoi( result.c_str() );
            SMSMessage::Status st;
            if ( 128 == code)
                    st = SMSMessage::Status::ST_CANCELED; else
            if ( 64 == code)
                    st = SMSMessage::Status::ST_PAID; else
            if ( 32 == code)
                    st = SMSMessage::Status::ST_BILLED; else
            if ( 16 == code)
                    st = SMSMessage::Status::ST_REJECTED; else
            if ( 8 == code)
                    st = SMSMessage::Status::ST_BUFFERED; else
            if ( 4 == code)
                    st = SMSMessage::Status::ST_BUFFERED; else
            if ( 2 == code)
                    st = SMSMessage::Status::ST_REJECTED; else
            if ( 1 == code)
                    st = SMSMessage::Status::ST_DELIVERED; else
            out << "Unknown delivery code: " << result << " ";

            trck->registerDeliveryNotification( SMSMessage::ID( reqid, msg_num ), st, gate );
        } catch ( boost::exception& err ) {
            Logger::get_mutable_instance().smslogerr( boost::diagnostic_information( err ) );
            return;
        } catch ( ... ) {
            out << "Fatal error ";
            Logger::get_mutable_instance().smslogerr( out.str() );
        }

    out << "parsed;";
    Logger::get_mutable_instance().smsloginfo( out.str() );

    Logger::get_mutable_instance().smslogwarn( "DeliveryHandler::handleRequest finished" );
}
