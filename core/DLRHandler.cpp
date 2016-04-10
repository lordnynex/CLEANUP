#include "DLRHandler.h"

#include <vector>
#include <sstream>

#include "DeliveryHandler.h"
#include "RequestTracker.h"
#include "PartnerManager.h"
#include "Logger.h"

#include "utils.h"

using std::string;

using namespace sms;
using namespace sms::utils;

DLRHandler::DLRHandler(Wt::WObject *parent) : Wt::WResource(parent) {
        logger.addField("message", true);
        logger.setStream( std::cout );
}

DLRHandler::~DLRHandler() {
        beingDeleted();
}

void DLRHandler::handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response) {

    Logger::get_mutable_instance().smslogwarn( "DLRHandler::handleRequest started" );

    response.setMimeType("text/xml");
    RequestTracker* trck = RequestTracker::Instance();
    std::ostringstream out;

    bool    auth = false;
    int     err_code = 0;
    string  err_descr = "OK";
    SMSMessage::Status code = 0;
    const string e = "";

    const string user   		= request.getParameter("user")		? *request.getParameter("user")		: "";
    const string pass		= request.getParameter("pass")		? *request.getParameter("pass")		: "";
    const string id			= request.getParameter("sms-id")  	? *request.getParameter("sms-id")	: "";

    try {
        PartnerInfo ptrn = PartnerManager::get_mutable_instance().findByName( user );
        if ( pass == ptrn.pPass ) {
            auth = true;
        }
    } catch ( PartnerNotFoundError& e ) {
        Logger::get_mutable_instance().smslogwarn( string("Unknown user is requesting status: [") + user + string(":") + pass + "]");
    }

    std::vector< std::string > res;

    if (!auth) {
        err_code = -1;
        err_descr = "Authorization failed";
    }

    if (id.length() > 100) {
        err_code = -1;
        err_descr = "Wrong sms-id";
    }

    Tokenize(id, res, ".");
    if (res.size() != 2) {
        err_code = -1;
        err_descr = "Wrong sms-id";
    }

    if ( ( auth ) && ( !err_code ) ) try {
        SMSRequest::ID reqid = atoll( res[0].c_str() );
        int msg_num = atoll( res[1].c_str() );
        SMSMessage::ID msg_id( reqid, msg_num );
        SMSMessage::PTR msg;

        msg = SMSMessageManager::get_mutable_instance().loadMessage(msg_id);

        code = msg->getStatus();

    } catch ( MessageNotFoundError& ) {
        err_code = -2;
        err_descr = "Message ID not found";
    } catch ( ... ) {
        err_code = -1;
        err_descr = "Internal error";
    }

    out     << "<?xml version=\"1.0\" ?>"
            << "<reply>"
            << "<result>" << err_descr << "</result>";
    if (!err_code)
        out << "<dlr sms-id=\"" << id << "\">"
                << "<code>" << code() << "</code>"
                << "<status>" << SMSMessage::Status::statusDescr( code ) << "</status>"
                << "</dlr>";
    out     << "</reply>";

    response.out() << out.str();

    Logger::get_mutable_instance().smslogwarn( "DLRHandler::handleRequest finished" );

}
