#include "PartnerManager.h"
#include "PGSql.h"
#include "Logger.h"
#include "Timer.h"

#include <boost/lexical_cast.hpp>

using namespace std;

bool PartnerInfo::isAdmin() {
    try {
        return ( boost::lexical_cast<int>( this->pId ) < 100 );
    } catch ( ... ) {
        return false;
    }
}


PartnerManager::PartnerManager() {

    loadFromDb();

    tid = sms::Timer::Instance()->addPeriodicEvent( boost::bind( &PartnerManager::loadFromDb, this ), 20 );
}

PartnerInfo PartnerManager::findByName( string pName ) throw ( PartnerNotFoundError ){
    boost::recursive_mutex::scoped_lock lck( pmlock );
    if ( pbox.get<1>().find( pName ) != pbox.get<1>().end() ) {
        return *pbox.get<1>().find( pName );
    }
    BOOST_THROW_EXCEPTION(  PartnerNotFoundError()
                            << throw_pName( pName.c_str() )
                            << sms::throw_descr( "PartnerManager::findByName [ Partner Not Found ]" ) );

}

PartnerInfo PartnerManager::findById( string id ) throw ( PartnerNotFoundError ){
    boost::recursive_mutex::scoped_lock lck( pmlock );
    if ( pbox.get<2>().find( id ) != pbox.get<2>().end() ) {
        return *pbox.get<2>().find( id );
    }
    BOOST_THROW_EXCEPTION(  PartnerNotFoundError()
                            << throw_pId( id.c_str() )
                            << sms::throw_descr( "PartnerManager::findById [ Partner Not Found ]" ) );

}

std::list< PartnerInfo > PartnerManager::getAll( std::string ownerid ) {
    boost::recursive_mutex::scoped_lock lck( pmlock );
    std::list< PartnerInfo > pi;

    pBox::nth_index<2>::type::iterator it;

    for ( it = pbox.get<2>().begin(); it != pbox.get<2>().end(); it++ ) {
        if ( ( ownerid.empty() ) || ( it->ownerId == ownerid ) )
            pi.push_back( *it );
    }

    return pi;
}

void PartnerManager::loadFromDb() {
    PGSql& db = PGSqlConnPoolSystem::get_mutable_instance().getdb();

    std::ostringstream out;
    out << "Loading partners data ";
    pBox pb;
    try {

        PGSql::ConnectionHolder cHold( db );
        ConnectionPTR conn = cHold.get();
        TransactionPTR tr = db.openTransaction( conn, "PartnerManager::loadFromDb" );
        std::ostringstream dbreq1;
        dbreq1  << " SELECT pid, uname, pass, cname, manager, balance, credit, plimit, postplay, trial, priority, phone, contact, tariff, ts, fname, lname, mname, companyname, caddress, email, ownerid, adminpass FROM partners;";

        Result res = tr->exec( dbreq1.str() );
        tr->commit();

        for ( Result::const_iterator dbr = res.begin(); dbr != res.end(); dbr++ ) {
            //TODO onlone reloading info
            pb.get<1>().insert( PartnerInfo(
                                            (*dbr)[1].as<string>(),
                                            (*dbr)[2].as<string>(),
                                            (*dbr)[22].as<string>(),
                                            (*dbr)[0].as<string>(),
                                            (*dbr)[21].as<string>(),
                                            (*dbr)[3].as<string>(),
                                            (*dbr)[4].as<string>(),
                                            (*dbr)[15].as<string>(),
                                            (*dbr)[16].as<string>(),
                                            (*dbr)[17].as<string>(),
                                            (*dbr)[18].as<string>(),
                                            (*dbr)[19].as<string>(),
                                            (*dbr)[20].as<string>(),
                                            (*dbr)[11].as<string>(),
                                            (*dbr)[12].as<string>(),
                                            (*dbr)[13].as<string>(),                                  
                                            (*dbr)[5].as<double>(),
                                            (*dbr)[6].as<double>(),
                                            (*dbr)[7].as<int>(),
                                            (*dbr)[8].as<bool>(),
                                            (*dbr)[9].as<bool>(),
                                            (*dbr)[10].as<int>(),
                                            (*dbr)[14].as<int>()
                                   ) );
        }

    } catch ( PGSqlError & err ) {
        out << "error; " << err.what();
        Logger::get_mutable_instance().smslogerr( out.str() );
    } catch ( PGBrokenConnection& err ) {
        out << "error; " << err.what();
        Logger::get_mutable_instance().smslogwarn( out.str() );
    }

    boost::recursive_mutex::scoped_lock lck( pmlock );
    pbox = pb;

    out << "parsed;";
    Logger::get_mutable_instance().smsloginfo( out.str() );
}

void PartnerManager::updateToDb( PartnerInfo& pi ) {
    boost::recursive_mutex::scoped_lock lck( pmlock );

    if ( pi.pId.empty() ) {
        setPid( pi );
    }

    PGSql& db = PGSqlConnPoolSystem::get_mutable_instance().getdb();

    std::ostringstream out;
    out << "Loading partners data ";
    try {

        PGSql::ConnectionHolder cHold( db );
        ConnectionPTR conn = cHold.get();
        TransactionPTR tr = db.openTransaction( conn, "PartnerManager::updateToDb" );
        std::ostringstream dbreq1;
        dbreq1  << "update partners set "
                << "uname='" << tr->esc( pi.pName ) << "' , "
                << "ownerid='" << tr->esc( pi.ownerId ) << "' , "
                << "pass='" << tr->esc( pi.pPass ) << "' , "
                << "adminpass='" << tr->esc( pi.pAdminPass ) << "' , "
                << "cname='" << tr->esc( pi.pCName ) << "' , "
                << "manager='" << tr->esc( pi.pManager ) << "' , "
                << "balance='" << ( pi.pBalance ) << "' , "
                << "credit='" << ( pi.pCredit ) << "' , "
                << "plimit='" << ( pi.pLimit ) << "' , "
                << "postplay='" << ( pi.pPostPay ) << "' , "
                << "trial='" << ( pi.pIsTrial ) << "' , "
                << "priority='" << ( pi.pPriority ) << "' , "
                << "phone='" << tr->esc( pi.phone ) << "' , "
                << "contact='" << tr->esc( pi.pContact ) << "' , "
                << "tariff='" << tr->esc( pi.tariff.getName() ) << "' , "
                << "ts='" << ( pi.tzone ) << "' , "
                << "fname='" << tr->esc( pi.pFirstName ) << "' , "
                << "lname='" << tr->esc( pi.pLastName ) << "' , "
                << "mname='" << tr->esc( pi.pMiddleName ) << "' , "
                << "companyname='" << tr->esc( pi.pCompanyName ) << "' , "
                << "caddress='" << tr->esc( pi.pCompanyAddress ) << "' , "
                << "email='" << tr->esc( pi.pEmail ) << "' "
                << "WHERE pid='" << tr->esc( pi.pId ) << "' returning pid;";


        Result res = tr->exec( dbreq1.str() );
        tr->commit();
        if ( res.size() == 0 ) {
            TransactionPTR tr = db.openTransaction( conn, "PartnerManager::insertToDb" );
            std::ostringstream dbreq1;
            dbreq1  << "INSERT into partners "
                    << "(pid, uname, pass, adminpass, ownerid, cname, manager, balance, credit, plimit, postplay, trial, priority, phone, contact, tariff, ts, fname, lname, mname, companyname, caddress, email ) "
                    << "VALUES ("
                    << "'" << tr->esc( pi.pId ) << "',"
                    << "'" << tr->esc( pi.pName ) << "',"
                    << "'" << tr->esc( pi.pPass ) << "',"
                    << "'" << tr->esc( pi.pAdminPass ) << "',"
                    << "'" << tr->esc( pi.ownerId ) << "',"
                    << "'" << tr->esc( pi.pCName ) << "',"
                    << "'" << tr->esc( pi.pManager ) << "',"
                    << "'" << ( pi.pBalance ) << "',"
                    << "'" << ( pi.pCredit ) << "',"
                    << "'" << ( pi.pLimit ) << "',"
                    << "'" << ( pi.pPostPay ) << "',"
                    << "'" << ( pi.pIsTrial ) << "',"
                    << "'" << ( pi.pPriority ) << "',"
                    << "'" << tr->esc( pi.phone ) << "',"
                    << "'" << tr->esc( pi.pContact ) << "',"
                    << "'" << tr->esc( pi.tariff.getName() ) << "',"
                    << "'" << ( pi.tzone ) << "',"
                    << "'" << tr->esc( pi.pFirstName ) << "',"
                    << "'" << tr->esc( pi.pLastName ) << "',"
                    << "'" << tr->esc( pi.pMiddleName ) << "',"
                    << "'" << tr->esc( pi.pCompanyName ) << "',"
                    << "'" << tr->esc( pi.pCompanyAddress ) << "',"
                    << "'" << tr->esc( pi.pEmail ) << "');";

            tr->exec( dbreq1.str() );
            tr->commit();
        }
    } catch ( PGSqlError & err ) {
        out << "error; " << err.what();
        Logger::get_mutable_instance().smslogerr( out.str() );
    } catch ( PGBrokenConnection& err ) {
        out << "error; " << err.what();
        Logger::get_mutable_instance().smslogwarn( out.str() );
    }

    out << "parsed;";
    Logger::get_mutable_instance().smsloginfo( out.str() );

    loadFromDb();
}

void PartnerManager::setPid( PartnerInfo& pi ) {
    boost::recursive_mutex::scoped_lock lck( pmlock );

    pBox::nth_index<2>::type::iterator it;

    int pId_max = 100;
    for ( it = pbox.get<2>().begin(); it != pbox.get<2>().end(); it++ ) {
        try {
            if ( boost::lexical_cast< int >( it->pId ) > pId_max ) {
                pId_max = boost::lexical_cast< int >( it->pId );
            }
        } catch ( ... ) {
            continue;
        }
    }

    pi.pId = boost::lexical_cast< std::string >( pId_max + 1 );
}

