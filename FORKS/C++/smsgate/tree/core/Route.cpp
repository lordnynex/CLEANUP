#include "Route.h"

#include "Timer.h"

#include <iostream>
#include <sstream>

#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>

Route::Route( ) {}

Route::Route( std::string name ) {
    route.name = name;
}

Route::Route( std::string name, std::string source ) {
    route.name = name;

    std::istringstream ifs( source );
    try {
        boost::archive::xml_iarchive ia( ifs );
        ia >> BOOST_SERIALIZATION_NVP( route );

    } catch ( std::exception& err ) {
        //throw std::runtime_error( string( "Cannot deserialize route[" ) + name + string( "]: " ) + err.what() );
    }
}

std::string Route::serialize() {
    std::ostringstream ofs;
    try {
        boost::archive::xml_oarchive oa(ofs);
        oa << BOOST_SERIALIZATION_NVP( route );
    } catch (...) {
        //throw std::runtime_error( "Cannot serialize route" );;
    }
    return ofs.str();
}

boost::logic::tribool Route::hasOption( std::string name ) {
    if ( route.options.find( name ) != route.options.end() )
        return true;

    return false;
}

boost::logic::tribool Route::hasOption( std::string name, std::string country ) {
    if ( route.countries.find( country ) != route.countries.end() )
        if ( route.countries[ country ].options.find( name ) != route.countries[ country ].options.end() )
            return true;

    boost::logic::tribool r = hasOption( name );
    if ( r == true )
        return boost::logic::indeterminate_keyword_t();

    return r;
}

boost::logic::tribool Route::hasOption( std::string name, std::string country, std::string oper ) {
    if ( route.countries.find( country ) != route.countries.end() )
        if ( route.countries[ country ].operators.find( oper ) != route.countries[ country ].operators.end() )
            if (
                    route.countries[ country ].operators[ oper ].options.find( name ) !=
                    route.countries[ country ].operators[ oper ].options.end()
                )
                return true;

    boost::logic::tribool r = hasOption( name, country );
    if ( r == true )
        return boost::logic::indeterminate_keyword_t();

    return r;
}

std::string Route::getOption( std::string name ) {
    if ( hasOption( name ) == true )
        return route.options[ name ];

    return "";
}

std::string Route::getOption( std::string name, std::string country ) {
    if ( hasOption( name, country ) == true )
        return route.countries[ country ].options[ name ];

    if ( hasOption( name ) == true )
        return route.options[ name ];

    return "";
}

std::string Route::getOption( std::string name, std::string country, std::string oper ) {
    if ( hasOption( name, country, oper ) == true )
        return route.countries[ country ].operators[oper].options[ name ];

    if ( hasOption( name, country ) == true )
        return route.countries[ country ].options[ name ];

    if ( hasOption( name ) == true )
        return route.options[ name ];

    return "";
}

void Route::setOption( std::string name, std::string value ) {
    route.options[ name ] = value;
}

void Route::setOption( std::string name, std::string country, std::string value ) {
    route.countries[country].options[ name ] = value;
}

void Route::setOption( std::string name, std::string country, std::string oper, std::string value ) {
    route.countries[country].operators[oper].options[ name ] = value;
}

void Route::removeOption( std::string name ) {
    if ( hasOption( name ) == true )
        route.options.erase( name );
}

void Route::removeOption( std::string name, std::string country ) {
    if ( hasOption( name, country ) == true )
        route.countries[ country ].options.erase( name );
}

void Route::removeOption( std::string name, std::string country, std::string oper ) {
    if ( hasOption( name, country, oper ) == true )
        route.countries[ country ].operators[ oper ].options.erase( name );
}


RouteManager::RouteManager(): db( PGSqlConnPoolSystem::get_mutable_instance().getdb() ) {
    updateTimerID = Timer::Instance()->addPeriodicEvent( boost::bind( &RouteManager::updateRouteList, this ), 60 );
    updateRouteList();
}

RouteManager::~RouteManager() {
    Timer::Instance()->cancelEvent( updateTimerID );
}

Route RouteManager::loadRoute(std::string name) {
    boost::recursive_mutex::scoped_lock lck(lock);
    return tmap[ name ];
}

void RouteManager::updateRouteList() {
    RouteListT _tlist;
    RouteMapT _tmap;
    std::ostringstream out;
    try {
        std::ostringstream r;

        PGSql::ConnectionHolder cHold( db );
        ConnectionPTR conn = cHold.get();
        TransactionPTR tr = db.openTransaction( conn, "RouteManager::updateRouteList" );

        r       << "SELECT name, description from routes;";

        Result res = tr->exec( r.str() );
        tr->commit();
        for ( Result::const_iterator dbr = res.begin(); dbr != res.end(); dbr++ ) {
            _tlist.push_back( (*dbr)[0].as<std::string>() );
            _tmap.insert( std::make_pair( (*dbr)[0].as<std::string>(), Route( (*dbr)[0].as<std::string>(), (*dbr)[1].as<std::string>() ) ) );
        }

        boost::recursive_mutex::scoped_lock lck(lock);

        tlist = _tlist;
        tmap = _tmap;

    } catch ( PGSqlError& err ) {
        out << "Error while loading route: " << err.what();
        Logger::get_mutable_instance().smslogerr( out.str() );
    } catch ( PGBrokenConnection& err ) {
        out << "Connection Error while loading route: " << err.what();
        Logger::get_mutable_instance().smslogerr( out.str() );
    }
}

void RouteManager::saveRoute( std::string name, std::string owner, Route t ) {
    std::ostringstream out;
    t.setOption( string("ownerid"), owner );
    try {
        std::ostringstream r;

        PGSql::ConnectionHolder cHold( db );
        ConnectionPTR conn = cHold.get();
        TransactionPTR tr = db.openTransaction( conn, "RouteManager::saveRoute" );

        if ( tmap.find( name ) == tmap.end() ) {
            r       << "INSERT into routes values ("
                    << "'" << tr->esc( name ) << "', "
                    << "'" << tr->esc( t.serialize() ) << "');";
        } else {
            r       << "UPDATE routes "
                    << "set description='" << tr->esc( t.serialize() ) << "' "
                    << "WHERE name='" << tr->esc( name ) << "';";
        }

        Result res = tr->exec( r.str() );
        tr->commit();

    } catch ( PGSqlError& err ) {
        out << "Error while saving route: " << err.what();
        Logger::get_mutable_instance().smslogerr( out.str() );
    } catch ( PGBrokenConnection& err ) {
        out << "Connection Error while saving route: " << err.what();
        Logger::get_mutable_instance().smslogerr( out.str() );
    }

    updateRouteList();
}

void RouteManager::removeRoute( std::string name ) {
    std::ostringstream out;
    try {
        std::ostringstream r;

        PGSql::ConnectionHolder cHold( db );
        ConnectionPTR conn = cHold.get();
        TransactionPTR tr = db.openTransaction( conn, "RouteManager::saveRoute" );

        r       << "DELETE from routes where name='" << tr->esc( name ) << "';";

        Result res = tr->exec( r.str() );
        tr->commit();

    } catch ( PGSqlError& err ) {
        out << "Error while saving route: " << err.what();
        Logger::get_mutable_instance().smslogerr( out.str() );
    } catch ( PGBrokenConnection& err ) {
        out << "Connection Error while saving route: " << err.what();
        Logger::get_mutable_instance().smslogerr( out.str() );
    }

    updateRouteList();
}


RouteManager::RouteListT RouteManager::routes_list( std::string owner ) {
    boost::recursive_mutex::scoped_lock lck(lock);
    RouteListT res;
    for ( RouteMapT::iterator it = tmap.begin(); it != tmap.end(); it++ ) {
        if ( owner.empty() || ( it->second.hasOption( "ownerid" ) && ( it->second.getOption( "ownerid" ) == owner ) ) ) {
            res.push_back( it->first );
        }
    }
    return res;
}
