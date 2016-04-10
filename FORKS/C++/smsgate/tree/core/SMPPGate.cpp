/*
 * File:   SMPPGate.cpp
 * Author: mohtep
 *
 * Created on 27 Февраль 2010 г., 20:02
 */

#include "SMPPGate.h"
#include "utils.h"
#include <sstream>

namespace sms {

    SMPPGate::SMPPGate( string gName, string uName, string uPass, int gPort, int gPriority, bool gEnabled, string gRule, string gOptions ) {
        reinit( gName, uName, uPass, gPort, gPriority, gEnabled, gRule, gOptions );
    }

    SMPPGate::SMPPGate() {
        this->_enabled = true;
        _suspended = false;
        _busy = false;
    }

    void SMPPGate::reinit(string gName, string uName, string uPass, int gPort, int gPriority, bool gEnabled, string gRule, string gOptions) {
        this->_gateName = gName;
        this->_userName = uName;
        this->_userPass = uPass;
        this->_gatePort = gPort;
        this->_gatePriority = gPriority;
        this->_enabled = gEnabled;
        this->_gateRule = gRule;
        utils::splitArgs( gOptions, this->_gateProperties );
        _suspended = false;
        _busy = false;

        if ( optionExists( "Tariff" ) ) {
            try {
                this->tariff = TariffManager::get_mutable_instance().loadTariff( getOption<std::string>("Tariff") );
            } catch ( std::exception& err ) {
                Logger::get_mutable_instance().smslogerr( err.what() );
            }
        }
    }

    void SMPPGate::reinit(string gName, string uName, string uPass, int gPort, int gPriority, bool gEnabled, string gRule, map< string, string > gOptions) {
        this->_gateName = gName;
        this->_userName = uName;
        this->_userPass = uPass;
        this->_gatePort = gPort;
        this->_gatePriority = gPriority;
        this->_enabled = gEnabled;
        this->_gateRule = gRule;
        this->_gateProperties = gOptions;
        _suspended = false;
        _busy = false;

        if ( optionExists( "Tariff" ) ) {
            try {
                this->tariff = TariffManager::get_mutable_instance().loadTariff( getOption<std::string>("Tariff") );
            } catch ( std::exception& err ) {
                Logger::get_mutable_instance().smslogerr( err.what() );
            }
        }
    }

    SMPPGate SMPPGate::loadFromDb( PGSql& db, string gName ) {
        SMPPGate gate;
        std::ostringstream req;

        PGSql::ConnectionHolder cHold( db );
        ConnectionPTR conn = cHold.get();
        TransactionPTR tr = db.openTransaction( conn, "SMPPGate::loadFromDb" );
        req     << "SELECT \"gName\", \"uName\", \"uPass\", \"gPort\", \"gPriority\", \"gEnabled\", \"gRule\", \"gOptions\" FROM gateways "
                << "WHERE \"gName\"='" << tr->esc(gName) << "';";
        Result res = tr->exec( req.str() );
        tr->commit();
        if ( res.size() > 0 ) {
            gate = SMPPGate (
                    res[0][0].as<string>(),
                    res[0][1].as<string>(),
                    res[0][2].as<string>(),
                    res[0][3].as<int>(),
                    res[0][4].as<int>(),
                    res[0][5].as<bool>(),
                    res[0][6].as<string>(),
                    res[0][7].is_null() ? "" : res[0][7].as<string>()
                    );
        } else {
            throw PGSqlError( "Empty dataset" );
        }

        return gate;
    }

    void SMPPGate::store2Db( PGSql& db ) {
        std::ostringstream req_exists, req_update, req_store;
        bool exists = false;

        PGSql::ConnectionHolder cHold( db );
        ConnectionPTR conn = cHold.get();
        TransactionPTR tr = db.openTransaction( conn, "SMPPGate::store2Db" );
        req_exists  << "SELECT \"gName\", \"uName\", \"uPass\", \"gPort\", \"gPriority\", \"gEnabled\", \"gRule\", \"gOptions\" FROM gateways "
                << "WHERE \"gName\"='" << tr->esc( _gateName ) << "';";
        Result res = tr->exec( req_exists.str() );
        tr->commit();
        if ( res.size() > 0 )
            exists = true;

        if ( exists ) {
            PGSql::ConnectionHolder cHold( db );
            ConnectionPTR conn = cHold.get();
            TransactionPTR tr = db.openTransaction( conn, "SMPPGate::store2Db" );
            req_update  << "UPDATE gateways SET "
                        << "\"gName\"='" << tr->esc( _gateName ) << "', "
                        << "\"uName\"='" << tr->esc( _userName ) << "', "
                        << "\"uPass\"='" << tr->esc( _userPass ) << "', "
                        << "\"gPort\"='" << _gatePort << "', "
                        << "\"gPriority\"='" << _gatePriority << "', "
                        << "\"gEnabled\"='" << _enabled << "' "
                        << "\"gRule\"='" << tr->esc( _gateRule ) << "' "
                        << "\"gOptions\"='" << tr->esc( utils::joinArgs( _gateProperties ) ) << "' "
                        << "WHERE 'gName'='" << tr->esc( _gateName ) << "';";
            Result res = tr->exec( req_update.str() );
            tr->commit();
        } else {
            PGSql::ConnectionHolder cHold( db );
            ConnectionPTR conn = cHold.get();
            TransactionPTR tr = db.openTransaction( conn, "SMPPGate::store2Db" );
            req_store   << "INSERT INTO gateways "
                        << "( \"gName\", \"uName\", \"uPass\", \"gPort\", \"gPriority\", \"gEnabled\", \"gRule\", \"gOptions\" ) VALUES ('"
                        << "'" << tr->esc( _gateName ) << "', "
                        << "'" << tr->esc( _userName ) << "', "
                        << "'" << tr->esc( _userPass ) << "', "
                        << "'" << _gatePort << "', "
                        << "'" << _gatePriority << "', "
                        << "'" << _enabled << "', "
                        << "'" << tr->esc( _gateRule ) << "', "
                        << "'" << tr->esc( utils::joinArgs( _gateProperties ) ) << "'); ";
            Result res = tr->exec( req_store.str() );
            tr->commit();
        }
    }


}
