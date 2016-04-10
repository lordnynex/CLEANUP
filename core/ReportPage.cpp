#include "ReportPage.h"
#include "stdlib.h"
#include <set>

using namespace Wt;
using namespace sms;

ReportPage::ReportPage( const WEnvironment& env ): WApplication( env ) {
    setTitle( "SMSGate report page" );
    wtbl = NULL;

    nPtnrRprtBtn = new WPushButton("Отчет по партнерам");
    nPtnrRprtBtn->clicked().connect(SLOT(this, ReportPage::onPtnrRprtBtnClicked));
    root()->addWidget( nPtnrRprtBtn );
}

ReportPage::~ReportPage() {
}

WApplication *createReportPage(const WEnvironment& env) {
    return new ReportPage(env);
}

void ReportPage::onPtnrRprtBtnClicked() {
}

ReportPage::msg_stats ReportPage::getMsgStats( string idp, WDate _from, WDate _to, std::set<string> gateways ) {
    boost::gregorian::date from_date = boost::gregorian::from_string( _from.toString("yyyy/MM/dd").toUTF8() );
    boost::gregorian::date to_date = boost::gregorian::from_string( _to.toString("yyyy/MM/dd").toUTF8() );
    boost::gregorian::date orig( 1970, boost::gregorian::Jan, 1 );
    boost::posix_time::ptime from( from_date, boost::posix_time::hours(0) );
    boost::posix_time::ptime to( to_date, boost::posix_time::hours(24) );
    boost::posix_time::ptime begin( orig, boost::posix_time::hours(0) );

    boost::posix_time::time_period lv( begin, from );
    boost::posix_time::time_period rv( begin, to );

    msg_stats stats;
    stats.total = 0;
    stats.delivered = 0;
    stats.undelivered = 0;
    PGSql& db( PGSqlConnPoolStats::get_mutable_instance().getdb() );

    try {
        std::ostringstream r;

        r       << "SELECT message_status.\"STATUS\" from smsrequest,message_status "
                << "WHERE smsrequest.\"REQUESTID\"=message_status.\"REQUESTID\" "
                << "AND smsrequest.\"PID\"='" << idp <<"' "
                << "AND smsrequest.\"WHEN\" "
                << "BETWEEN " << lv.length().total_seconds() << " AND " << rv.length().total_seconds() << ";";

        string a = r.str();

        PGSql::ConnectionHolder cHold( db );
        ConnectionPTR conn = cHold.get();
        TransactionPTR tr = db.openTransaction( conn, "RequestTracker::RequestTracker()" );
        Result res = tr->exec( r.str() );
        tr->commit();
        for ( Result::const_iterator it = res.begin(); it != res.end(); it++ ) {
            SMSMessage::ID msgid;
            stats.total++;
            switch( (*it)[0].as<int>() ) {
                case 0:
                    stats.delivered++;
                    break;
                case -1:
                    stats.undelivered++;
                    break;
            }
        }
    } catch ( PGBrokenConnection& err ) {
        Logger::get_mutable_instance().smslogerr( string("ReportPage::getMsgStats() ") + err.what() );
    } catch ( PGSqlError& err ) {
        Logger::get_mutable_instance().smslogerr( string("ReportPage::getMsgStats() ") + err.what() );
    }

    return stats;
}

void ReportPage::buildReport( string idp, WDate _from, WDate _to, std::set<string> gateways, bool adv ) {
}
