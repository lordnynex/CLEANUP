#include "Tariff.h"
#include "Timer.h"

#include <iostream>
#include <sstream>

#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>

Tariff::Tariff( ) {}

Tariff::Tariff( std::string name ) {
    tariff.name = name;
}

Tariff::Tariff( std::string name, std::string source ) {
    tariff.name = name;

    std::istringstream ifs( source );
    try {
        boost::archive::xml_iarchive ia( ifs );
        ia >> BOOST_SERIALIZATION_NVP( tariff );

    } catch ( std::exception& err ) {
        //throw std::runtime_error( string( "Cannot deserialize tariff[" ) + name + string( "]: " ) + err.what() );
    }
}

std::string Tariff::serialize() {
    std::ostringstream ofs;
    try {
        boost::archive::xml_oarchive oa(ofs);
        oa << BOOST_SERIALIZATION_NVP( tariff );
    } catch (...) {
        //throw std::runtime_error( "Cannot serialize tariff" );;
    }
    return ofs.str();
}

void Tariff::setPrice( std::string cname, double price ) {
    tariff.countries[ cname ].options[ "price" ] = boost::lexical_cast< std::string >( price );
}

void Tariff::setPrice( std::string cname, std::string opcode, double price ) {
    tariff.countries[ cname ].operators[ opcode ].options[ "price" ] = boost::lexical_cast< std::string >( price );
}

double Tariff::currencyConvert( TariffCurrency from, TariffCurrency to, double price ) {

    if ( from.getValue() == "RUR" ) { }
    if ( from.getValue() == "EUR" ) { price = price * EURCOURSE; }
    if ( from.getValue() == "USD" ) { price = price * USDCOURSE; }
    if ( from.getValue() == "0.01RUR" ) { price = price / 100; }
    if ( from.getValue() == "0.01EUR" ) { price = price / 100 * EURCOURSE; }
    if ( from.getValue() == "0.01USD" ) { price = price / 100 * USDCOURSE; }

    if ( to.getValue() == "RUR" ) { }
    if ( to.getValue() == "EUR" ) { price = price / EURCOURSE; }
    if ( to.getValue() == "USD" ) { price = price / USDCOURSE; }
    if ( to.getValue() == "0.01RUR" ) { price = price * 100; }
    if ( to.getValue() == "0.01EUR" ) { price = price * 100 / EURCOURSE; }
    if ( to.getValue() == "0.01USD" ) { price = price * 100 / USDCOURSE; }


    return price;
}

double Tariff::costs( std::string cname ) {

    if ( hasOption( "price", cname ) ) {
        try {
            double price = boost::lexical_cast< double >( getOption( "price", cname ) );
            return currencyConvert( getOption< TariffCurrency >( cname ), TariffCurrency(), price );
        } catch ( ... ) {
            return INVALID_VALUE;
        }
    } else
    if ( !hasOption( "price", cname ) )  {
        if ( hasOption< TariffOptionUnknownPolicy >( cname ) == false )
            return INVALID_VALUE;
        else {
            double price;
            if ( tariff.countries.find( cname ) != tariff.countries.end() ) {
                TariffOptionUnknownPolicy policy = getOption< TariffOptionUnknownPolicy >( cname );
                if ( policy.getValue() == "FREE" )
                    price = 0.0;
                double maxval = INVALID_VALUE;
                if ( policy.getValue() == "MAXIMUM" ) {
                    for ( std::map< std::string, TariffOperatorInfo >::iterator it = tariff.countries[ cname ].operators.begin(); it != tariff.countries[ cname ].operators.end(); it++ ) {
                        std::string mnc = it->first;
                        if ( hasOption( "price", cname, mnc ) )
                        try {
                            double price = boost::lexical_cast< double >( getOption( "price", cname, mnc ) );
                            price = currencyConvert( getOption< TariffCurrency >( cname, mnc ), TariffCurrency(), price );
                            maxval = std::max( maxval, price );

                        } catch ( ... ) {}
                    }
                    price = maxval;
                }
                double sum = 0;
                double total = 0;
                if ( policy.getValue() == "AVERAGE" ) {
                    for ( std::map< std::string, TariffOperatorInfo >::iterator it = tariff.countries[ cname ].operators.begin(); it != tariff.countries[ cname ].operators.end(); it++ ) {
                        std::string mnc = it->first;
                        if ( hasOption( "price", cname, mnc ) ) {
                            try {
                                double price = boost::lexical_cast< double >( getOption( "price", cname, mnc ) );
                                price = currencyConvert( getOption< TariffCurrency >( cname, mnc ), TariffCurrency(), price );
                                sum += price;
                            } catch ( ... ) {
                                continue;
                            }

                            total++;
                        }
                    }
                    price = total == 0? INVALID_VALUE: sum/total;
                }
                return price;
            } else
            return INVALID_VALUE;
        }

    }

    try {
        double price = boost::lexical_cast< double >( getOption( "price" ) );
        return currencyConvert( getOption< TariffCurrency >( cname ), TariffCurrency(), price );
    } catch ( ... ) {
        return INVALID_VALUE;
    }

}

double Tariff::costs( std::string cname, std::string opcode ) {
    if ( hasOption( "price", cname, opcode ) ) {

        try {
            double price = boost::lexical_cast< double >( getOption( "price", cname, opcode ) );
            return currencyConvert( getOption< TariffCurrency >( cname, opcode ), TariffCurrency(), price );
        } catch ( ... ) {
            return INVALID_VALUE;
        }

    } else
    if ( !hasOption( "price", cname, opcode ) ) {
        if ( hasOption< TariffOptionUnknownPolicy >( cname, opcode ) == false )
            return INVALID_VALUE;
        else {
            return costs( cname );
        }
    }

    return costs( cname );
}

double Tariff::costs( std::string op, SMSMessage::Status status ) {
    TariffOptionPaidStatuses option = getOption< TariffOptionPaidStatuses >( op );
    TariffOptionPaidStatuses::ValueT opt_val = option.getValues();

    if ( ( opt_val.find( "REJECTED" ) != opt_val.end() ) && ( status == SMSMessage::Status::ST_REJECTED ) ) return 0.0;
    if ( ( opt_val.find( "EXPIRED" ) != opt_val.end() ) && ( status == SMSMessage::Status::ST_EXPIRED ) ) return 0.0;
    if ( ( opt_val.find( "UNDELIVERED" ) != opt_val.end() ) && ( status == SMSMessage::Status::ST_NOT_DELIVERED ) ) return 0.0;

    return costs( op );
}

double Tariff::costs( std::string cname, std::string opcode, SMSMessage::Status status ) {
    TariffOptionPaidStatuses option = getOption< TariffOptionPaidStatuses >( cname, opcode );
    TariffOptionPaidStatuses::ValueT opt_val = option.getValues();

    if ( ( opt_val.find( "REJECTED" ) != opt_val.end() ) && ( status == SMSMessage::Status::ST_REJECTED ) ) return 0.0;
    if ( ( opt_val.find( "EXPIRED" ) != opt_val.end() ) && ( status == SMSMessage::Status::ST_EXPIRED ) ) return 0.0;
    if ( ( opt_val.find( "UNDELIVERED" ) != opt_val.end() ) && ( status == SMSMessage::Status::ST_NOT_DELIVERED ) ) return 0.0;

    return costs( cname, opcode );
}

boost::logic::tribool Tariff::hasOption( std::string name ) {
    if ( tariff.options.find( name ) != tariff.options.end() )
        return true;

    return false;
}

boost::logic::tribool Tariff::hasOption( std::string name, std::string country ) {
    if ( tariff.countries.find( country ) != tariff.countries.end() )
        if ( tariff.countries[ country ].options.find( name ) != tariff.countries[ country ].options.end() )
            return true;

    boost::logic::tribool r = hasOption( name );
    if ( r == true )
        return boost::logic::indeterminate_keyword_t();

    return r;
}

boost::logic::tribool Tariff::hasOption( std::string name, std::string country, std::string oper ) {
    if ( tariff.countries.find( country ) != tariff.countries.end() )
        if ( tariff.countries[ country ].operators.find( oper ) != tariff.countries[ country ].operators.end() )
            if (
                    tariff.countries[ country ].operators[ oper ].options.find( name ) !=
                    tariff.countries[ country ].operators[ oper ].options.end()
                )
                return true;

    boost::logic::tribool r = hasOption( name, country );
    if ( r == true )
        return boost::logic::indeterminate_keyword_t();

    return r;
}

std::string Tariff::getOption( std::string name ) {
    if ( hasOption( name ) == true )
        return tariff.options[ name ];

    return "";
}

std::string Tariff::getOption( std::string name, std::string country ) {
    if ( hasOption( name, country ) == true )
        return tariff.countries[ country ].options[ name ];

    if ( hasOption( name ) == true )
        return tariff.options[ name ];

    return "";
}

std::string Tariff::getOption( std::string name, std::string country, std::string oper ) {
    if ( hasOption( name, country, oper ) == true )
        return tariff.countries[ country ].operators[oper].options[ name ];

    if ( hasOption( name, country ) == true )
        return tariff.countries[ country ].options[ name ];

    if ( hasOption( name ) == true )
        return tariff.options[ name ];

    return "";
}

void Tariff::setOption( std::string name, std::string value ) {
    tariff.options[ name ] = value;
}

void Tariff::setOption( std::string name, std::string country, std::string value ) {
    tariff.countries[country].options[ name ] = value;
}

void Tariff::setOption( std::string name, std::string country, std::string oper, std::string value ) {
    tariff.countries[country].operators[oper].options[ name ] = value;
}

void Tariff::removeOption( std::string name ) {
    if ( hasOption( name ) == true )
        tariff.options.erase( name );
}

void Tariff::removeOption( std::string name, std::string country ) {
    if ( hasOption( name, country ) == true )
        tariff.countries[ country ].options.erase( name );
}

void Tariff::removeOption( std::string name, std::string country, std::string oper ) {
    if ( hasOption( name, country, oper ) == true )
        tariff.countries[ country ].operators[ oper ].options.erase( name );
}


TariffManager::TariffManager(): db( PGSqlConnPoolSystem::get_mutable_instance().getdb() ) {
    updateTimerID = Timer::Instance()->addPeriodicEvent( boost::bind( &TariffManager::updateTariffList, this ), 60 );
    updateTariffList();
}

TariffManager::~TariffManager() {
    Timer::Instance()->cancelEvent( updateTimerID );
}

Tariff TariffManager::loadTariff(std::string name) {
    boost::recursive_mutex::scoped_lock lck(lock);
    return tmap[ name ];
}

void TariffManager::updateTariffList() {
    TariffListT _tlist;
    TariffMapT _tmap;
    std::ostringstream out;
    try {
        std::ostringstream r;

        PGSql::ConnectionHolder cHold( db );
        ConnectionPTR conn = cHold.get();
        TransactionPTR tr = db.openTransaction( conn, "TariffManager::updateTariffList" );

        r       << "SELECT name, description from tariffs;";

        Result res = tr->exec( r.str() );
        tr->commit();
        for ( Result::const_iterator dbr = res.begin(); dbr != res.end(); dbr++ ) {
            _tlist.push_back( (*dbr)[0].as<std::string>() );
            _tmap.insert( std::make_pair( (*dbr)[0].as<std::string>(), Tariff( (*dbr)[0].as<std::string>(), (*dbr)[1].as<std::string>() ) ) );
        }

        boost::recursive_mutex::scoped_lock lck(lock);
        tlist = _tlist;
        tmap = _tmap;

    } catch ( PGSqlError& err ) {
        out << "Error while loading tariff: " << err.what();
        Logger::get_mutable_instance().smslogerr( out.str() );
    } catch ( PGBrokenConnection& err ) {
        out << "Connection Error while loading tariff: " << err.what();
        Logger::get_mutable_instance().smslogerr( out.str() );
    }
}

void TariffManager::saveTariff( std::string name, std::string owner, Tariff t ) {
    std::ostringstream out;
    t.setOption( string("ownerid"), owner );
    try {
        std::ostringstream r;

        PGSql::ConnectionHolder cHold( db );
        ConnectionPTR conn = cHold.get();
        TransactionPTR tr = db.openTransaction( conn, "TariffManager::saveTariff" );

        if ( tmap.find( name ) == tmap.end() ) {
            r       << "INSERT into tariffs values ("
                    << "'" << tr->esc( name ) << "', "
                    << "'" << tr->esc( t.serialize() ) << "');";
        } else {
            r       << "UPDATE tariffs "
                    << "set description='" << tr->esc( t.serialize() ) << "' "
                    << "WHERE name='" << tr->esc( name ) << "';";
        }

        Result res = tr->exec( r.str() );
        tr->commit();

    } catch ( PGSqlError& err ) {
        out << "Error while saving tariff: " << err.what();
        Logger::get_mutable_instance().smslogerr( out.str() );
    } catch ( PGBrokenConnection& err ) {
        out << "Connection Error while saving tariff: " << err.what();
        Logger::get_mutable_instance().smslogerr( out.str() );
    }

    updateTariffList();
}

void TariffManager::removeTariff( std::string name ) {
    std::ostringstream out;
    try {
        std::ostringstream r;

        PGSql::ConnectionHolder cHold( db );
        ConnectionPTR conn = cHold.get();
        TransactionPTR tr = db.openTransaction( conn, "TariffManager::saveTariff" );

        r       << "DELETE from tariffs where name='" << tr->esc( name ) << "';";

        Result res = tr->exec( r.str() );
        tr->commit();

    } catch ( PGSqlError& err ) {
        out << "Error while saving tariff: " << err.what();
        Logger::get_mutable_instance().smslogerr( out.str() );
    } catch ( PGBrokenConnection& err ) {
        out << "Connection Error while saving tariff: " << err.what();
        Logger::get_mutable_instance().smslogerr( out.str() );
    }

    updateTariffList();
}


TariffManager::TariffListT TariffManager::tariffs_list( std::string owner ) {
    boost::recursive_mutex::scoped_lock lck(lock);
    TariffListT res;
    for ( TariffMapT::iterator it = tmap.begin(); it != tmap.end(); it++ ) {
        if ( owner.empty() || ( it->second.hasOption( "ownerid" ) && ( it->second.getOption( "ownerid" ) == owner ) ) ) {
            res.push_back( it->first );
        }
    }
    return res;
}
