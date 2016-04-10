#include "MTHandler.h"

#include <iostream>
#include <Wt/WApplication>
#include <Wt/Http/Request>

#include <boost/thread/mutex.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "MTHandler.h"
#include "RequestTracker.h"
#include "utils.h"
#include "PartnerManager.h"
#include "HttpClient.h"

using namespace std;
using namespace sms;
using namespace sms::utils;

MTHandler::MTHandler(Wt::WObject *parent) : Wt::WResource(parent) {
}

MTHandler::~MTHandler() {
        beingDeleted();
}

void MTHandler::handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response) {

        Logger::get_mutable_instance().smslogwarn( "MTHandler::handleRequest started" );
        RequestTracker* trck = RequestTracker::Instance();

        response.setMimeType("text/html");

        try {

            const string e = "";

            const string pref 		= request.getParameter("pref")		? utils::StringCp1251ToUtf8(*request.getParameter("pref"))	: e;
            const string txt 		= request.getParameter("txt")		? utils::StringCp1251ToUtf8(*request.getParameter("txt"))	: e;
            const string tid		= request.getParameter("tid")		? *request.getParameter("tid")          : e;
            const string cn 		= request.getParameter("cn")  		? *request.getParameter("cn")		: e;
            const string op 		= request.getParameter("op")  		? *request.getParameter("op")		: e;
            const string phone 		= request.getParameter("phone") 	? *request.getParameter("phone")	: e;
            const string sn 		= request.getParameter("sn")  		? *request.getParameter("sn")		: e;

            const string test 		= request.getParameter("test")          ? *request.getParameter("test")		: e;
            const string repeat 	= request.getParameter("repeat")        ? *request.getParameter("repeat")	: e;
            const string rtime 		= request.getParameter("rtime") 	? *request.getParameter("rtime")	: e;
            const string opn		= request.getParameter("opn")  		? *request.getParameter("opn")		: e;
            const string mpref 		= request.getParameter("mpref")  	? *request.getParameter("mpref")	: e;
            const string tg 		= request.getParameter("tg")  		? *request.getParameter("tg")		: e;
            const string cost 		= request.getParameter("cost") 		? *request.getParameter("cost")		: e;
            const string md5 		= request.getParameter("md5")  		? *request.getParameter("md5")		: e;

            if ( ( pref == "labirint" ) || ( pref == "otpisat'" ) ) {
                ostringstream req;
                req << "http://sender.greensms.ru/mt.cgi";
                bool firstdone = false;
                if ( !pref.empty() ) {
                    req << ( firstdone? "&":"?" ) << "pref=" << pref;
                    firstdone = true;
                }
                if ( !txt.empty() ) {
                    req << ( firstdone? "&":"?" ) << "txt=" << txt;
                    firstdone = true;
                }
                if ( !tid.empty() ) {
                    req << ( firstdone? "&":"?" ) << "tid=" << tid;
                    firstdone = true;
                }
                if ( !cn.empty() ) {
                    req << ( firstdone? "&":"?" ) << "cn=" << cn;
                    firstdone = true;
                }
                if ( !op.empty() ) {
                    req << ( firstdone? "&":"?" ) << "op=" << op;
                    firstdone = true;
                }
                if ( !phone.empty() ) {
                    req << ( firstdone? "&":"?" ) << "phone=" << phone;
                    firstdone = true;
                }
                if ( !sn.empty() ) {
                    req << ( firstdone? "&":"?" ) << "sn=" << sn;
                    firstdone = true;
                }
                if ( !test.empty() ) {
                    req << ( firstdone? "&":"?" ) << "test=" << test;
                    firstdone = true;
                }
                if ( !repeat.empty() ) {
                    req << ( firstdone? "&":"?" ) << "repeat=" << repeat;
                    firstdone = true;
                }
                if ( !rtime.empty() ) {
                    req << ( firstdone? "&":"?" ) << "rtime=" << rtime;
                    firstdone = true;
                }
                if ( !opn.empty() ) {
                    req << ( firstdone? "&":"?" ) << "opn=" << opn;
                    firstdone = true;
                }
                if ( !mpref.empty() ) {
                    req << ( firstdone? "&":"?" ) << "mpref=" << mpref;
                    firstdone = true;
                }
                if ( !tg.empty() ) {
                    req << ( firstdone? "&":"?" ) << "tg=" << tg;
                    firstdone = true;
                }

                if ( !cost.empty() ) {
                    req << ( firstdone? "&":"?" ) << "cost=" << cost;
                    firstdone = true;
                }

                if ( !md5.empty() ) {
                    req << ( firstdone? "&":"?" ) << "md5=" << md5;
                    firstdone = true;
                }
                HttpClient::Response resp;
                resp = trck->client.get( req.str(), 1000 );
                response.out() << utils::StringUtf8ToCp1251(std::string("sms=Ваше сообщение принято."));
            } else {
                response.out() << utils::StringUtf8ToCp1251(std::string("sms=Ваше сообщение принято."));
            }

        } catch ( BasicError& err ) {
            response.out() << utils::StringUtf8ToCp1251(std::string("sms=") + err.what() );
        } catch ( std::exception& err ) {
            response.out() << utils::StringUtf8ToCp1251(std::string("sms=") + err.what() );
        }

        Logger::get_mutable_instance().smslogwarn( "MTHandler::handleRequest finished" );
}
