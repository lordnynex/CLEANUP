/*
 * File:   SMPPGate.h
 * Author: mohtep
 *
 * Created on 27 Февраль 2010 г., 20:02
 */

#ifndef _SMPPGATE_H
#define	_SMPPGATE_H

#include <map>
#include <boost/lexical_cast.hpp>

#include "PGSql.h"
#include "LimitQueue.h"
#include "SMSMessage.h"
#include "SMSRequest.h"
#include "HttpClient.h"
#include "Tariff.h"

namespace sms {
    using std::string;
    using std::map;

    class SMPPGate {
    public:
        SMPPGate( string gName, string uName, string uPass, int gPort, int gPriority, bool gEnabled, string gRule, string gOptions );
        SMPPGate();

        void reinit( string gName, string uName, string uPass, int gPort, int gPriority, bool gEnabled, string gRule, string gOptions );
        void reinit( string gName, string uName, string uPass, int gPort, int gPriority, bool gEnabled, string gRule, map< string, string > gOptions );
        string gateName() { return _gateName; }
        string userName() { return _userName; }
        string userPass() { return _userPass; }
        int gatePort() { return _gatePort; }
        int gatePriority() { return _gatePriority; }
        void setPriority( int _priority ) { _gatePriority = _priority; }
        bool enabled() { return _enabled; }
        void suspend( ) { _enabled = false; _suspended = true; }
        bool suspended( ) { return _suspended; }
        bool isBusy( ) { return _busy; }
        bool setBusy( bool what ) { _busy = what; }
        string gateRule() { return _gateRule; }
        bool optionExists( string oName ) { return ( _gateProperties.find( oName ) != _gateProperties.end() ); }
        Tariff& getTariff() { return tariff; }
        template < class T >
        T getOption( string oName );
        map< string, string > gateProperties() { return _gateProperties; }

        void store2Db( PGSql& db );
        static SMPPGate loadFromDb(  PGSql& db, string gName );

    private:

        string _gateName;
        string _userName;
        string _userPass;
        int _gatePort;
        int _gatePriority;
        bool _enabled;
        string _gateRule;
        bool _suspended;
        bool _busy;
        map< string, string > _gateProperties;
        Tariff tariff;

    };

    template < class T >
    T SMPPGate::getOption( string oName ) {
        return boost::lexical_cast< T >( _gateProperties[ oName ] );
    }

}
#endif	/* _SMPPGATE_H */

