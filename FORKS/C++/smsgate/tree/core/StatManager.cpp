#include "StatManager.h"
#include "ConfigManager.h"
#include "RequestTracker.h"
#include "Timer.h"
#include "utils.h"
#include "Logger.h"

#include <list>
#include <stdexcept>
#include <iostream>
#include <boost/thread/xtime.hpp>

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>

const int on1SecondUpdateInterval = 1;
const int on1MinuteUpdateInterval = 1;
const int on5MinuteUpdateInterval = 1;
const int on1HourUpdateInterval = 60;
const int on1DayUpdateInterval = 60*60;
const int onCountryInfoUpdateInterval = 60*30;

namespace sms {

    StatManager* StatManager::pInstance_;

    StatManager::StatManager(): db( PGSqlConnPoolStats::get_mutable_instance().getdb() ) {
        boost::recursive_mutex::scoped_lock lck(lock);
        on1SecondHandlerSMPPGate = Timer::Instance()->addPeriodicEvent(
                boost::bind( &StatManager::on1SecondUpdateSMPPGate, this ), on1SecondUpdateInterval );
        on1MinuteHandlerSMPPGate = Timer::Instance()->addPeriodicEvent(
                boost::bind( &StatManager::on1MinuteUpdateSMPPGate, this ), on1MinuteUpdateInterval );
        on5MinuteHandlerSMPPGate = Timer::Instance()->addPeriodicEvent(
                boost::bind( &StatManager::on5MinuteUpdateSMPPGate, this ), on5MinuteUpdateInterval );
        on1HourHandlerSMPPGate = Timer::Instance()->addPeriodicEvent(
                boost::bind( &StatManager::on1HourUpdateSMPPGate, this ), on1HourUpdateInterval );
        on1DayHandlerSMPPGate = Timer::Instance()->addPeriodicEvent(
                boost::bind( &StatManager::on1DayUpdateSMPPGate, this ), on1DayUpdateInterval );
        onCountryInfoHandler = Timer::Instance()->addPeriodicEvent(
                boost::bind( &StatManager::onCountryInfoUpdate, this ), onCountryInfoUpdateInterval );

        onStoreHandler = Timer::Instance()->addPeriodicEvent(
                boost::bind( &StatManager::onStore, this ), on1HourUpdateInterval );

        onLoad();

        Timer::Instance()->addSingleEvent( boost::bind( &StatManager::on1SecondUpdateSMPPGate, this ), 0 );
        Timer::Instance()->addSingleEvent( boost::bind( &StatManager::on1MinuteUpdateSMPPGate, this ), 0 );
        Timer::Instance()->addSingleEvent( boost::bind( &StatManager::on5MinuteUpdateSMPPGate, this ), 0 );
        Timer::Instance()->addSingleEvent( boost::bind( &StatManager::on1HourUpdateSMPPGate, this ), 0 );
        Timer::Instance()->addSingleEvent( boost::bind( &StatManager::on1DayUpdateSMPPGate, this ), 0 );
        Timer::Instance()->addSingleEvent( boost::bind( &StatManager::onCountryInfoUpdate, this ), 0 );
        boost::xtime_get( &uptime, boost::TIME_UTC_ );

    }

    StatManager::StatManager( const StatManager& orig_class ):db( orig_class.db ) {
        StatManager& orig = const_cast<StatManager&>( orig_class );
        boost::recursive_mutex::scoped_lock lck(orig.lock);
        boost::recursive_mutex::scoped_lock lck2(lock);

        on1SecondStatsSMPPGate = orig.on1SecondStatsSMPPGate;
        on1MinuteStatsSMPPGate = orig.on1MinuteStatsSMPPGate;
        on5MinuteStatsSMPPGate = orig.on5MinuteStatsSMPPGate;
        on1HourStatsSMPPGate = orig.on1HourStatsSMPPGate;
        on1DayStatsSMPPGate = orig.on1DayStatsSMPPGate;

        on1SecondHandlerSMPPGate = Timer::Instance()->addPeriodicEvent(
                boost::bind( &StatManager::on1SecondUpdateSMPPGate, this ), on1SecondUpdateInterval );
        on1MinuteHandlerSMPPGate = Timer::Instance()->addPeriodicEvent(
                boost::bind( &StatManager::on1MinuteUpdateSMPPGate, this ), on1MinuteUpdateInterval );
        on5MinuteHandlerSMPPGate = Timer::Instance()->addPeriodicEvent(
                boost::bind( &StatManager::on5MinuteUpdateSMPPGate, this ), on5MinuteUpdateInterval );
        on1HourHandlerSMPPGate = Timer::Instance()->addPeriodicEvent(
                boost::bind( &StatManager::on1HourUpdateSMPPGate, this ), on1HourUpdateInterval );
        on1DayHandlerSMPPGate = Timer::Instance()->addPeriodicEvent(
                boost::bind( &StatManager::on1DayUpdateSMPPGate, this ), on1DayUpdateInterval );
        onCountryInfoHandler = Timer::Instance()->addPeriodicEvent(
                boost::bind( &StatManager::onCountryInfoUpdate, this ), onCountryInfoUpdateInterval );

        Timer::Instance()->addSingleEvent( boost::bind( &StatManager::on1SecondUpdateSMPPGate, this ), 0 );
        Timer::Instance()->addSingleEvent( boost::bind( &StatManager::on1MinuteUpdateSMPPGate, this ), 0 );
        Timer::Instance()->addSingleEvent( boost::bind( &StatManager::on5MinuteUpdateSMPPGate, this ), 0 );
        Timer::Instance()->addSingleEvent( boost::bind( &StatManager::on1HourUpdateSMPPGate, this ), 0 );
        Timer::Instance()->addSingleEvent( boost::bind( &StatManager::on1DayUpdateSMPPGate, this ), 0 );
        Timer::Instance()->addSingleEvent( boost::bind( &StatManager::onCountryInfoUpdate, this ), 0 );
    }

    StatManager::~StatManager() {
        boost::recursive_mutex::scoped_lock lck(lock);
        Timer::Instance()->cancelEvent( on1SecondHandlerSMPPGate );
        Timer::Instance()->cancelEvent( on1MinuteHandlerSMPPGate );
        Timer::Instance()->cancelEvent( on5MinuteHandlerSMPPGate );
        Timer::Instance()->cancelEvent( on1HourHandlerSMPPGate );
        Timer::Instance()->cancelEvent( on1DayHandlerSMPPGate );
        Timer::Instance()->cancelEvent( onCountryInfoHandler );
        Timer::Instance()->cancelEvent( onStoreHandler );
    }

    StatManager::gNamePropMap StatManager::get1SecondStatsSMPPGate(){
        boost::recursive_mutex::scoped_lock lck(lock);
        return on1SecondStatsSMPPGate;
    }

    StatManager::gNamePropMap StatManager::get1MinuteStatsSMPPGate(){
        boost::recursive_mutex::scoped_lock lck(lock);
        return on1MinuteStatsSMPPGate;
    }

    StatManager::gNamePropMap StatManager::get5MinuteStatsSMPPGate(){
        boost::recursive_mutex::scoped_lock lck(lock);
        return on5MinuteStatsSMPPGate;
    }

    StatManager::gNamePropMap StatManager::get1HourStatsSMPPGate() {
        boost::recursive_mutex::scoped_lock lck(lock);
        return on1HourStatsSMPPGate;
    }

    StatManager::gNamePropMap StatManager::get1DayStatsSMPPGate() {
        boost::recursive_mutex::scoped_lock lck(lock);
        return on1DayStatsSMPPGate;
    }

    StatManager::TCountryInfoTable StatManager::getCountryInfoLastUpdate( ) {
        boost::recursive_mutex::scoped_lock lck(lock);
        return countryInfoLastUpdate;
    }

    StatManager::gNamePropMap StatManager::onUpdateFromToSMPPGate( long from, long to ) {
        RequestTracker* trck = RequestTracker::Instance();
        std::ostringstream out;
        out
                << "StatManager::onUpdateFromToSMPPGate [" << from << ";" << to << "]: ";

        std::ostringstream gwlist_req;
        std::list< string > gwlist;
        gNamePropMap pres;

        try {
            gwlist_req << "SELECT \"gName\" FROM gateways WHERE \"gEnabled\"=1;";
            PGSql::ConnectionHolder cHold( db );
            ConnectionPTR conn = cHold.get();
            TransactionPTR tr = db.openTransaction( conn, "StatManager::onUpdateFromToSMPPGate()" );
            Result res = tr->exec( gwlist_req.str() );
            tr->commit();
            for ( Result::size_type i = 0; i < res.size(); i++ ) {
                gwlist.push_back( res[i][0].as<string>() );
            }
        } catch ( PGBrokenConnection& err ) {
            Logger::get_mutable_instance().smslogerr( string("StatManager::onUpdateFromToSMPPGate ") + err.what() );
            return pres;
        } catch ( PGSqlError& err ) {
            Logger::get_mutable_instance().smslogerr( string("StatManager::onUpdateFromToSMPPGate ") + err.what() );
            return pres;
        }

        gwlist.push_front("jane");
        std::list< string >::iterator it;
        for ( it = gwlist.begin(); it != gwlist.end(); it++ ) {

            SMPPGateProperties p;
            p.requests = 0;
            p.acks = 0;
            p.deliveres = 0;
            p.responses = 0;
            //p.deliverytime = 0;

            try {
                PGSql::ConnectionHolder cHold( db );
                ConnectionPTR conn = cHold.get();
                std::ostringstream req_req;
                if ( *it == "jane" ) {
                    req_req     << "select SUM ( \"PARTS\" ) from message_status where "
                                << "\"WHEN\" BETWEEN " << from << " AND " << to << " UNION ALL ";

                    req_req     << "select SUM ( \"PARTS\" ) from message_status where "
                                << "\"WHEN\" BETWEEN " << from << " AND " << to << " AND "
                                << "NOT \"STATUS\" IN ( 3, 4 )  UNION ALL ";

                    req_req     << "select SUM ( \"PARTS\" ) from message_status where "
                                << "\"WHEN\" BETWEEN " << from << " AND " << to << " AND "
                                << "\"STATUS\" IN ( 0, -1, -2, -3 ) UNION ALL ";

                    req_req     << "select SUM ( \"PARTS\" ) from message_status where "
                                << "\"WHEN\" BETWEEN " << from << " AND " << to << " AND "
                                << "\"STATUS\"=0;";

                } else {
                    req_req     << "select SUM ( \"PARTS\" ) from message_status where ( \"REQUESTID\", \"MESSAGEID\" ) IN ( "
                                << "select \"REQUESTID\", \"MESSAGEID\" from message_history where "
                                << "\"WHEN\" BETWEEN " << from << " AND " << to << " AND "
                                << "\"GATEWAY\"='" << *it << "' AND "
                                << "\"OP_CODE\"=0 AND \"OP_DIRECTION\"=0 ) "
                                << " AND \"WHEN\" BETWEEN " << from << " AND " << to << " "
                                << "UNION ALL ";

                    req_req     << "select SUM ( \"PARTS\" ) from message_status where ( \"REQUESTID\", \"MESSAGEID\" ) IN ( "
                                << "select \"REQUESTID\", \"MESSAGEID\" from message_history where "
                                << "\"WHEN\" BETWEEN " << from << " AND " << to << " AND "
                                << "\"GATEWAY\"='" << *it << "' AND "
                                << "\"OP_CODE\"=1 AND \"OP_DIRECTION\"=1 AND \"OP_RESULT\"=1 )  "
                                << " AND \"WHEN\" BETWEEN " << from << " AND " << to << " "
                                << "UNION ALL ";

                    req_req     << "select SUM ( \"PARTS\" ) from message_status where ( \"REQUESTID\", \"MESSAGEID\" ) IN ( "
                                << "select \"REQUESTID\", \"MESSAGEID\" from message_history where "
                                << "\"WHEN\" BETWEEN " << from << " AND " << to << " AND "
                                << "\"GATEWAY\"='" << *it << "' AND "
                                << "\"OP_CODE\"=0 AND \"OP_DIRECTION\"=0 INTERSECT "
                                << "select \"REQUESTID\", \"MESSAGEID\" from message_history where "
                                << "\"WHEN\" BETWEEN " << from << " AND " << to << " AND "
                                << "\"GATEWAY\"='" << *it << "' AND "
                                << "\"OP_CODE\"=1 AND \"OP_DIRECTION\"=1 AND \"OP_RESULT\" IN (0, -1, -2, -3) )  "
                                << " AND \"WHEN\" BETWEEN " << from << " AND " << to << " "
                                << "UNION ALL ";

                    req_req     << "select SUM ( \"PARTS\" ) from message_status where ( \"REQUESTID\", \"MESSAGEID\" ) IN ( "
                                << "select \"REQUESTID\", \"MESSAGEID\" from message_history where "
                                << "\"WHEN\" BETWEEN " << from << " AND " << to << " AND "
                                << "\"GATEWAY\"='" << *it << "' AND "
                                << "\"OP_CODE\"=0 AND \"OP_DIRECTION\"=0 INTERSECT "
                                << "select \"REQUESTID\", \"MESSAGEID\" from message_history where "
                                << "\"WHEN\" BETWEEN " << from << " AND " << to << " AND "
                                << "\"GATEWAY\"='" << *it << "' AND "
                                << "\"OP_CODE\"=1 AND \"OP_DIRECTION\"=1 AND \"OP_RESULT\"=0 ) "
                                    << " AND \"WHEN\" BETWEEN " << from << " AND " << to << " "
                                << ";";
                }


                {
                    TransactionPTR tr = db.openTransaction( conn, "StatManager::onUpdateFromToSMPPGate()" );
                    Result res = tr->exec( req_req.str() );
                    tr->commit();
                    p.requests =  res.size() > 0 ? ( res[0][0].is_null() ? 0: res[0][0].as<long>() ): 0;
                    p.acks =      res.size() > 1 ? ( res[1][0].is_null() ? 0: res[1][0].as<long>() ): 0;
                    p.responses = res.size() > 2 ? ( res[2][0].is_null() ? 0: res[2][0].as<long>() ): 0;
                    p.deliveres = res.size() > 3 ? ( res[3][0].is_null() ? 0: res[3][0].as<long>() ): 0;
                }

            } catch ( PGBrokenConnection& err ) {
                Logger::get_mutable_instance().smslogerr( string("StatManager::onUpdateFromToSMPPGate ") + err.what() );
                return pres;
            } catch ( PGSqlError& err ) {
                Logger::get_mutable_instance().smslogerr( string("StatManager::onUpdateFromToSMPPGate ") + err.what() );
                return pres;
            } catch ( std::exception& err ) {
                Logger::get_mutable_instance().smslogerr( string("StatManager::onUpdateFromToSMPPGate ") + err.what() );
            }

            pres[ *it ] = p;
        }

        return pres;
    }

    StatManager::gNamePropMap StatManager::onUpdateFromToSMPPGateCountry( string cname, string opname ) {
        RequestTracker* trck = RequestTracker::Instance();

        std::ostringstream gwlist_req;
        std::list< string > gwlist;
        gNamePropMap pres;

        PGSql::ConnectionHolder cHold( db );
        ConnectionPTR conn;
        {
            boost::xtime from, to;
            boost::xtime_get( &from, boost::TIME_UTC_ );
            conn = cHold.get();
            boost::xtime_get( &to, boost::TIME_UTC_ );
            std::ostringstream out;
            out << "Connection hold" << " for " << to.sec - from.sec << " seconds";
            Logger::get_mutable_instance().dbloginfo( out.str() );
        }


        TransactionPTR tr = db.openTransaction( conn, "StatManager::onUpdateFromToSMPPGate()" );
        try {
            gwlist_req << "SELECT \"gName\" FROM gateways;";
            Result res = tr->exec( gwlist_req.str() );
            for ( Result::size_type i = 0; i < res.size(); i++ ) {
                gwlist.push_back( res[i][0].as<string>() );
            }
        } catch ( PGBrokenConnection& err ) {
            Logger::get_mutable_instance().smslogerr( string("StatManager::onUpdateFromToSMPPGate ") + err.what() );
            tr->commit();
            return pres;
        } catch ( PGSqlError& err ) {
            Logger::get_mutable_instance().smslogerr( string("StatManager::onUpdateFromToSMPPGate ") + err.what() );
            tr->commit();
            return pres;
        }

        gwlist.push_front("jane");
        std::list< string >::iterator it;
        for ( it = gwlist.begin(); it != gwlist.end(); it++ ) {
            SMPPGateProperties p;
            p.requests = 0;
            p.acks = 0;
            p.deliveres = 0;
            p.responses = 0;
            p.deliverytime = 0;

            pres[ *it ] = p;
        }

        boost::xtime now;
        boost::xtime_get( &now, boost::TIME_UTC_ );
        try {

            {
                std::ostringstream req_jane, req_view;

                req_view        << "create or replace temp view idlist as "
                                << "select * from message_status where TRUE ";
                if ( !cname.empty() )
                    req_view    << "AND \"COUNTRY\"='"<< tr->esc( cname ) <<"' ";
                if ( !opname.empty() )
                    req_view    << "AND \"OPERATOR\"='"<< tr->esc( opname ) <<"' ";
                req_view        << "AND \"WHEN\">" << now.sec - 60*60*24 << ";";

                {
                    boost::xtime from, to;
                    boost::xtime_get( &from, boost::TIME_UTC_ );
                    tr->exec( req_view.str() );
                    boost::xtime_get( &to, boost::TIME_UTC_ );
                    req_view << " for " << to.sec - from.sec << " seconds";
                    Logger::get_mutable_instance().dbloginfo( req_view.str() );
                }

                req_jane        << "select count(*) from idlist UNION ALL ";
                req_jane        << "select count(*) from idlist where \"STATUS\"=0;";

                {
                    boost::xtime from, to;
                    boost::xtime_get( &from, boost::TIME_UTC_ );
                    Result res = tr->exec( req_jane.str() );
                    pres["jane"].requests =  res.size() > 0 ? ( res[0][0].is_null() ? 0: res[0][0].as<long>() ): 0;
                    pres["jane"].acks = 0;
                    pres["jane"].responses = 0;
                    pres["jane"].deliveres = res.size() > 1 ? ( res[1][0].is_null() ? 0: res[1][0].as<long>() ): 0;
                    boost::xtime_get( &to, boost::TIME_UTC_ );
                    req_jane << " for " << to.sec - from.sec << " seconds";
                    Logger::get_mutable_instance().dbloginfo( req_jane.str() );
                }
            }
            {
                std::ostringstream req_req;
                std::ostringstream req_dlr;

                req_req         << "select \"GATEWAY\", count(*) from message_history where ( \"REQUESTID\", \"MESSAGEID\" ) IN ("
                                << "select \"REQUESTID\", \"MESSAGEID\" from idlist) AND "
                                << "\"OP_CODE\"=0 AND \"OP_DIRECTION\"=0 GROUP BY \"GATEWAY\";";

                {
                    Result res = tr->exec( req_req.str() );
                    for ( Result::const_iterator dbr = res.begin(); dbr != res.end(); dbr++ ) {
                        pres[(*dbr)[0].as<string>()].requests = (*dbr)[1].as<long>();
                    }
                }

                req_dlr         << "select \"GATEWAY\", count(*) from message_history where ( \"REQUESTID\", \"MESSAGEID\" ) IN ("
                                << "select \"REQUESTID\", \"MESSAGEID\" from idlist) AND "
                                << "\"OP_CODE\"=1 AND \"OP_DIRECTION\"=1 AND \"OP_RESULT\"=0 GROUP BY \"GATEWAY\";";

                {
                    boost::xtime from, to;
                    boost::xtime_get( &from, boost::TIME_UTC_ );
                    Result res = tr->exec( req_dlr.str() );
                    for ( Result::const_iterator dbr = res.begin(); dbr != res.end(); dbr++ ) {
                        pres[(*dbr)[0].as<string>()].deliveres = (*dbr)[1].as<long>();
                    }
                    boost::xtime_get( &to, boost::TIME_UTC_ );
                    req_dlr << " for " << to.sec - from.sec << " seconds";
                    Logger::get_mutable_instance().dbloginfo( req_dlr.str() );
                }
            }


        } catch ( PGBrokenConnection& err ) {
            Logger::get_mutable_instance().smslogerr( string("StatManager::onUpdateFromToSMPPGate ") + err.what() );
            tr->commit();
            return pres;
        } catch ( PGSqlError& err ) {
            Logger::get_mutable_instance().smslogerr( string("StatManager::onUpdateFromToSMPPGate ") + err.what() );
            tr->commit();
            return pres;
        } catch ( std::exception& err ) {
            tr->commit();
            Logger::get_mutable_instance().smslogerr( string("StatManager::onUpdateFromToSMPPGate ") + err.what() );
            return pres;
        }

        {
            boost::xtime from, to;
            boost::xtime_get( &from, boost::TIME_UTC_ );
            tr->commit();
            boost::xtime_get( &to, boost::TIME_UTC_ );
            std::ostringstream out;
            out << "Commited" << " for " << to.sec - from.sec << " seconds";
            Logger::get_mutable_instance().dbloginfo( out.str() );
        }

        return pres;
    }

    StatManager::gNamePropMap StatManager::onUpdateDeltaSMPPGate( long delta ) {
        boost::xtime now;
        boost::xtime_get( &now, boost::TIME_UTC_ );

        return onUpdateFromToSMPPGate( now.sec - delta, now.sec );
    }

    void StatManager::on1SecondUpdateSMPPGate() {
        gNamePropMap p = onUpdateDeltaSMPPGate( 1 );
        boost::recursive_mutex::scoped_lock lck(lock);
        on1SecondStatsSMPPGate = p;
    }

    void StatManager::on1MinuteUpdateSMPPGate() {
        gNamePropMap p = onUpdateDeltaSMPPGate( 60 );
        boost::recursive_mutex::scoped_lock lck(lock);
        on1MinuteStatsSMPPGate = p;
    }

    void StatManager::on5MinuteUpdateSMPPGate() {
        gNamePropMap p = onUpdateDeltaSMPPGate( 5*60 );
        boost::recursive_mutex::scoped_lock lck(lock);
        on5MinuteStatsSMPPGate = p;
    }

    void StatManager::on1HourUpdateSMPPGate() {
        gNamePropMap p = onUpdateDeltaSMPPGate( 60*60 );
        boost::recursive_mutex::scoped_lock lck(lock);
        on1HourStatsSMPPGate = p;
    }

    void StatManager::on1DayUpdateSMPPGate() {
        gNamePropMap p = onUpdateDeltaSMPPGate( 60*60*25 );
        boost::recursive_mutex::scoped_lock lck(lock);
        on1DayStatsSMPPGate = p;
    }

    std::string StatManager::getUptime() {
        std::ostringstream out;
        out.setf(std::ios_base::right, std::ios_base::adjustfield);
        out.fill('0');
        boost::xtime now;
        boost::xtime_get( &now, boost::TIME_UTC_ );
        out << std::setw(2) << ( now.sec - this->uptime.sec ) / ( 60*60*24 ) << ":" <<
               std::setw(2) << (( now.sec - this->uptime.sec ) / 3600) % 24 << ":" <<
               std::setw(2) << (( now.sec - this->uptime.sec ) / 60) %60 << ":" <<
               std::setw(2) << ( now.sec - this->uptime.sec ) % 60;
        return out.str();
    }

    void StatManager::onCountryInfoUpdate() {
        TCountryInfoTable p;
        MessageClassifier::CountryOperatorMapT cnamelist = MessageClassifier::get_mutable_instance().getCOMap();

        int cpos;
        int gpos;
        MessageClassifier::CountryOperatorMapT::iterator it;

        for ( it = cnamelist.begin(), cpos = 0; it != cnamelist.end(); it++, cpos++ ) {
            string cname = it->second.cName;

            gNamePropMap gm = onUpdateFromToSMPPGateCountry( cname, "" );
            gNamePropMap::const_iterator gt;
            p.resize( cpos + 1 );
            p[cpos].resize( gm.size() );

            for ( gt = gm.begin(), gpos = 0; gt != gm.end(); gt++, gpos++ ) {
                p[cpos][gpos].cname = cname;
                p[cpos][gpos].opname = "Total";
                p[cpos][gpos].gname = gt->first;
                p[cpos][gpos].requests = gt->second.requests;
                p[cpos][gpos].acks = gt->second.acks;
                p[cpos][gpos].deliveres = gt->second.deliveres;
            }
            cpos++;

            for ( MessageClassifier::CountryInfo::OperatorMapT::iterator ct = it->second.operators.begin(); ct != it->second.operators.end(); ct++, cpos++ ) {
                string opname = ct->second.getName();
                gNamePropMap gm = onUpdateFromToSMPPGateCountry( cname, opname );
                gNamePropMap::const_iterator gt;
                p.resize( cpos + 1 );
                p[cpos].resize( gm.size() );
                for ( gt = gm.begin(), gpos = 0; gt != gm.end(); gt++, gpos++ ) {
                    p[cpos][gpos].cname = cname;
                    p[cpos][gpos].opname = opname;
                    p[cpos][gpos].gname = gt->first;
                    p[cpos][gpos].requests = gt->second.requests;
                    p[cpos][gpos].acks = gt->second.acks;
                    p[cpos][gpos].deliveres = gt->second.deliveres;
                }

            }
        }

        boost::recursive_mutex::scoped_lock lck(lock);
        countryInfoLastUpdate = p;
    }

    void StatManager::onStore() {
        boost::recursive_mutex::scoped_lock lck(lock);
        std::ofstream ofs("stats.xml");
        try {
            boost::archive::xml_oarchive oa(ofs);
            oa << BOOST_SERIALIZATION_NVP( this );
        } catch (...) {}
    }

    void StatManager::onLoad() {
        boost::recursive_mutex::scoped_lock lck(lock);
        std::ifstream ifs("stats.xml");
        if (!ifs.good()) return;
        try {
            boost::archive::xml_iarchive ia(ifs);
            ia >> BOOST_SERIALIZATION_NVP( *this );
        } catch (...) {}
    }
}
