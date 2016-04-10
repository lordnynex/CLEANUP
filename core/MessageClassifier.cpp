#include "MessageClassifier.h"
#include "utils.h"
#include "Logger.h"

#include <iostream>
#include <fstream>
#include <cstdlib>

namespace sms {


    MessageClassifier::MessageClassifier() {
        loadReplacesMap();
        loadCountryOperatorMap();
        loadRoutingMap();
    }

    void MessageClassifier::loadReplacesMap() {
        ConfigManager& cfg = *ConfigManager::Instance();
        std::ostringstream out;

        out << "Loading replaces: ";

        std::string opdb = cfg.getProperty<std::string>("system.replacesmap");
        std::ifstream in(opdb.c_str());

        while ( in.good() ) {
            char l[1024];
            in.getline(l, 1024);
            std::vector< std::string > to_vec;
            utils::Tokenize( l, to_vec, "`" );

            if ( to_vec.size() != 3 )
                continue;

            replaces[ to_vec[0] ] = std::make_pair( to_vec[1], to_vec[2] );

        }

        out << replaces.size() << " replaces loaded;";
        Logger::get_mutable_instance().smsloginfo( out.str() );

    }

    void MessageClassifier::loadCountryOperatorMap() {
        comap.clear();
        {
            std::ostringstream r;

            r       << "select mcc, preffix, LOWER(code), name from countries;";

            PGSql& db = PGSqlConnPoolSystem::get_mutable_instance().getdb();

            PGSql::ConnectionHolder cHold( db );
            ConnectionPTR conn = cHold.get();
            TransactionPTR tr = db.openTransaction( conn, "MessageClassifier::loadCountryOperatorMap()" );
            Result res = tr->exec( r.str() );
            tr->commit();
            for ( Result::const_iterator dbr = res.begin(); dbr != res.end(); dbr++ ) {
                CountryInfo coinfo;
                coinfo.mcc = (*dbr)[0].as< std::string >();
                coinfo.cPreffix = (*dbr)[1].as< std::string >();
                coinfo.cCode= (*dbr)[2].as< std::string >();
                coinfo.cName = (*dbr)[3].as< std::string >();

                comap[ coinfo.mcc ] = coinfo;
                coprefmap[ coinfo.cPreffix ] = coinfo.mcc;
            }
        }

        {
            std::ostringstream r;

            r       << "select mcc, mnc, name, company from mccmnc;";

            PGSql& db = PGSqlConnPoolSystem::get_mutable_instance().getdb();

            PGSql::ConnectionHolder cHold( db );
            ConnectionPTR conn = cHold.get();
            TransactionPTR tr = db.openTransaction( conn, "MessageClassifier::loadCountryOperatorMap()" );
            Result res = tr->exec( r.str() );
            tr->commit();
            for ( Result::const_iterator dbr = res.begin(); dbr != res.end(); dbr++ ) {
                OperatorInfo coinfo;
                coinfo.mcc = (*dbr)[0].as< std::string>();
                coinfo.mnc = (*dbr)[1].as< std::string >();

                if ( coinfo.mnc.length() == 1 )
                    coinfo.mnc = std::string("0") + coinfo.mnc;

                coinfo.opName = (*dbr)[2].as< std::string >();
                coinfo.opCompany = (*dbr)[3].as< std::string >();

                comap[ coinfo.mcc ].operators[ coinfo.mnc ] = coinfo;
            }
        }
    }

    void MessageClassifier::loadRoutingMap() {
        preffmap.clear();
        {
            std::ostringstream r;

            r       << "select preffix, mcc, mnc, region from preffix_map;";

            PGSql& db = PGSqlConnPoolSystem::get_mutable_instance().getdb();

            PGSql::ConnectionHolder cHold( db );
            ConnectionPTR conn = cHold.get();
            TransactionPTR tr = db.openTransaction( conn, "MessageClassifier::loadRoutingMap()" );
            Result res = tr->exec( r.str() );
            tr->commit();
            for ( Result::const_iterator dbr = res.begin(); dbr != res.end(); dbr++ ) {
                std::string preff;
                std::string mcc;
                std::string mnc;
                std::string region;

                preff = (*dbr)[0].as< std::string >();
                mcc = (*dbr)[1].as< std::string >();
                mnc = (*dbr)[2].as< std::string >();
                if ( mnc.length() == 1 )
                    mnc = std::string("0") + mnc;

                region = (*dbr)[3].as< std::string >();

                preffmap.insert( std::make_pair( preff, boost::tuples::make_tuple( mcc, mnc, region ) ) );
            }
        }
    }

    std::string MessageClassifier::applyReplace( std::string phone ) {
        for ( int i = phone.length(); i > 0 ; i-- ) {
            std::string s = phone.substr(0, i );
            if ( replaces.find( s ) != replaces.end() ) {
                phone.replace( 0, replaces[ s ].first.size(), replaces[ s ].second );
                return phone;
            }
        }
        return phone;
    }

    MessageClassifier::CountryOperatorMapT MessageClassifier::getCOMap() {
        return comap;
    }

    MessageClassifier::CountryInfo MessageClassifier::getMsgClass( std::string phone ) {
        CountryInfo country;
        for ( int i = phone.length(); i > 0 ; i-- ) {
            std::string pref = phone.substr(0, i );

            if ( coprefmap.find( pref ) == coprefmap.end() )
                continue;

            country = comap[ coprefmap[ pref ] ];
            country.operators.clear();
            break;
        }

        for ( int i = phone.length(); i > 0 ; i-- ) {
            std::string pref = phone.substr(0, i );

            if ( preffmap.find( pref ) == preffmap.end() )
                continue;

            if ( preffmap.count( pref ) != 1 )
                break;

            std::string mcc    = boost::tuples::get<0>( preffmap.equal_range( pref ).first->second );
            std::string mnc    = boost::tuples::get<1>( preffmap.equal_range( pref ).first->second );
            std::string region = boost::tuples::get<2>( preffmap.equal_range( pref ).first->second );

            if ( ( comap.find( mcc ) != comap.end() ) && ( comap[ mcc ].operators.find( mnc ) != comap[ mcc ].operators.end() ) ) {
                OperatorInfo oi = comap[mcc].operators[mnc];
                oi.opRegion = region;
                country.operators[ mnc ] = oi;
            }

            break;
        }

        return country;
    }

}
