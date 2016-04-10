#include <Wt/WServer>
#include "RequestHandler.h"
#include "DeliveryHandler.h"
#include "DLRHandler.h"
#include "MonitoringHandler.h"
#include "RequestTracker.h"
#include "MTHandler.h"
#include "ConfigManager.h"
#include "StatManager.h"
#include "AdminPage.h"
#include "CountryGatesStats.h"
#include "ReportPage.h"
#include "NumberInfoPage.h"
#include "PersonalCabinet.h"
#include "PersonalPage.h"

#include <boost/thread/thread.hpp>
#include <signal.h>

#include "SMPPGateFilterParser.h"
#include "utils.h"

using namespace Wt;
using namespace sms;
using namespace std;

int main(int argc, char **argv) {

    signal(SIGPIPE, SIG_IGN);


    RequestTracker* trck = RequestTracker::Instance();
    StatManager* sm = StatManager::Instance();
    for ( int i = 0; i < ConfigManager::Instance()->getProperty<int>("system.threadsnum"); i++) {
        boost::thread maineventloop(boost::bind( &RequestTracker::MainEventLoop, trck ));
    }
    boost::thread delayedeventloop(boost::bind( &RequestTracker::DelayedEventLoop, trck ));
    boost::thread outboxpartnereventloop(boost::bind( &RequestTracker::OutboxPartnerEventLoop, trck ));
    boost::thread outboxeventloop(boost::bind( &RequestTracker::OutboxEventLoop, trck ));

    try {
        WServer server(argv[0]);
        server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);

        server.addResource( new RequestHandler(), "/mt.cgi" );
        server.addResource( new RequestHandlerSMST(), "/mtt.cgi" );
        server.addResource( new DeliveryHandler(), "/rcv.cgi" );
        server.addResource( new DLRHandler(), "/dlr.cgi" );
        server.addResource( new MonitoringHandler(), "/stat.cgi" );
        server.addResource( new MTHandler(), "/mth.cgi" );
        server.addEntryPoint( Wt::Application, createAdminPage, "/admin.cgi");
        server.addEntryPoint( Wt::Application, createGatesStatsPage, "/gstats.cgi");
        server.addEntryPoint( Wt::Application, createNumberInfoPage, "/gnuminfo.cgi");

        server.addEntryPoint( Wt::Application, createPersonalCabinet, "/cabinet.cgi");
        server.addEntryPoint( Wt::Application, createPersonalPage, "/");

        if (server.start()) {
            int sig = WServer::waitForShutdown();

            std::cerr << "Shutdown (signal = " << sig << ")" << std::endl;
            server.stop();
        }
    } catch (WServer::Exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    } catch (std::exception& e) {
        std::cerr << "exception: " << e.what() << "\n";
        return 1;
    }

//    SPriorityQueue<int> pq( 3 );
//    pq.push( 20 );
//    pq.push( 21, 2, 1 );
//    pq.push( 22, 2 );

//    while ( !pq.isEmpty() ) {
//        std::cout << pq.top() << std::endl;
//        pq.pop();
//    }

//    std::string msg = "Пришлите свой возраст в ответ на эту СМС, если Вы хотите получить код к архиву Skype";
//    cout << utils::getGsmParts( msg );
}
