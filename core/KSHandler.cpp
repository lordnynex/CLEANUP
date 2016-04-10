#include "KSHandler.h"

#include <vector>
#include <sstream>

#include "RequestTracker.h"

#include "utils.h"

using std::string;

using namespace sms;
using namespace sms::utils;

KSHandler::KSHandler(Wt::WObject *parent) : Wt::WResource(parent) {
        logger.addField("message", true);
        logger.setStream( std::cout );
}

KSHandler::~KSHandler() {
        beingDeleted();
}

void KSHandler::handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response) {
    response.setMimeType("text/html");

    string body;
    while (!request.in().eof()) {
        char sbody[1000];
        request.in().getline( sbody, 999 );
        body += sbody;
    }

    size_t pmid = body.find( "mid=\"" );
    if ( pmid == string::npos ) {
        Logger::get_mutable_instance().smsloginfo( "Kievstar DLR error: invalid dlr report " + body );
        return;
    }

    size_t pidl = pmid + 5;
    size_t pidr = body.find( '"', pidl );
    if ( pidr == string::npos ) {
        Logger::get_mutable_instance().smsloginfo( "Kievstar DLR error: invalid dlr report " + body );
        return;
    }

    string id = body.substr( pidl, pidr - pidl );
    SMSMessage::Status code;
    if ( body.find( "Delivered" ) != string::npos ) {
        code = SMSMessage::Status::ST_DELIVERED;
    } else {
        code = SMSMessage::Status::ST_REJECTED;
    }

    std::vector< std::string > res;
    if (id.length() > 100) {
            Logger::get_mutable_instance().smsloginfo( "Kievstar DLR error: invalid ID " + body );
            return;
    }

    Tokenize(id, res, ".");
    if (res.size() != 2) {
            Logger::get_mutable_instance().smsloginfo( "Kievstar DLR error: invalid ID " + body );
            return;
    }


    try {
        SMSRequest::ID reqid = atoll( res[0].c_str() );
        int msg_num = atoll( res[1].c_str() );

        RequestTracker::Instance()->registerDeliveryNotification( SMSMessage::ID( reqid, msg_num ), code, "mt_uaks" );
    } catch ( boost::exception& err ) {
        Logger::get_mutable_instance().smslogerr( boost::diagnostic_information( err ) );
        return;
    } catch ( ... ) {
        Logger::get_mutable_instance().smsloginfo( "Kievstar DLR error: fatal error " + body );
    }

    Logger::get_mutable_instance().smsloginfo( "Kievstar DLR received: id=" + id + string(" code=") + sms::message::Status::statusDescr(code) );

    //Logger::get_mutable_instance().smslogwarn( "DeliveryHandler::handleRequest finished" );



}
