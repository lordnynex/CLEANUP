#ifndef LOGGER_H
#define LOGGER_H

#include <Wt/WLogger>
#include <boost/serialization/singleton.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/date_time.hpp>

#include "ConfigManager.h"

class Logger: public boost::serialization::singleton< Logger >  {

public:
    Logger() {
        smslogger.addField("datetime", true);
        smslogger.addField("type", false);
        smslogger.addField("message", true);

        dblogger.addField("datetime", true);
        dblogger.addField("type", false);
        dblogger.addField("message", true);

        httplogger.addField("datetime", true);
        httplogger.addField("type", false);
        httplogger.addField("message", true);

        reopen();
    }

    void log( Wt::WLogger& logger, std::string type, std::string msg ) {
        boost::recursive_mutex::scoped_lock( lock );

        Wt::WLogEntry entry = logger.entry();
        entry << Wt::WLogger::timestamp << Wt::WLogger::sep
                << '<' << boost::this_thread::get_id() << '>' << Wt::WLogger::sep
                << '[' << type << ']' << Wt::WLogger::sep
                << msg;
    }

    void smsloginfo( std::string msg ) { log(smslogger, "info", msg); }
    void smslogwarn( std::string msg ) { log(smslogger, "warn", msg); }
    void smslogerr( std::string msg )  { log(smslogger, "err", msg);  }

    void dbloginfo( std::string msg ) { log(dblogger, "info", msg); }
    void dblogwarn( std::string msg ) { log(dblogger, "warn", msg); }
    void dblogerr( std::string msg )  { log(dblogger, "err", msg);  }

    void httploginfo( std::string msg ) { log(httplogger, "info", msg); }
    void httplogwarn( std::string msg ) { log(httplogger, "warn", msg); }
    void httplogerr( std::string msg )  { log(httplogger, "err", msg);  }

    void reopen() {
        std::ostringstream sysdate;
        sysdate << boost::posix_time::second_clock::local_time().date();

        smslog.open( std::string(ConfigManager::Instance()->getProperty< std::string >( "system.msglog" ) + std::string("-") + sysdate.str()).c_str(), std::ios::out | std::ios_base::app );
        dblog.open( std::string(ConfigManager::Instance()->getProperty< std::string >( "system.dblog" ) + std::string("-") + sysdate.str()).c_str(), std::ios::out | std::ios_base::app );
        httplog.open( std::string(ConfigManager::Instance()->getProperty< std::string >( "system.httplog" ) + std::string("-") + sysdate.str()).c_str(), std::ios::out | std::ios_base::app );


        smslogger.setStream( smslog );
        dblogger.setStream( dblog );
        httplogger.setStream( httplog );
}

private:
    Wt::WLogger smslogger;
    Wt::WLogger dblogger;
    Wt::WLogger httplogger;

    std::ofstream smslog;
    std::ofstream dblog;
    std::ofstream httplog;

    boost::mutex lock;
};

#endif // LOGGER_H
