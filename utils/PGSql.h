#ifndef PGSQL_H
#define PGSQL_H

#include "ConfigManager.h"

#include <string>
#include <list>
#include <boost/shared_ptr.hpp>
#include <pqxx/pqxx>

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/serialization/singleton.hpp>

typedef pqxx::connection Connection;
typedef boost::shared_ptr< Connection > ConnectionPTR;
typedef pqxx::transaction<> Transaction;
typedef boost::shared_ptr< Transaction > TransactionPTR;
typedef pqxx::result Result;

typedef pqxx::broken_connection PGBrokenConnection;
typedef pqxx::sql_error PGSqlError;

class ConnectionPool {
public:
    ConnectionPool( std::string connect_string, int max_conn );
    ConnectionPool( const ConnectionPool& orig );

    ConnectionPTR getConnection();
    void freeConnection( ConnectionPTR cptr );
private:
    typedef std::list< ConnectionPTR > TConnPool;
    TConnPool connPool;
    int busy_conn;
    int max_conn;
    std::string conn_string;
    boost::mutex getConnMut;

    ConnectionPTR openConnection();
};

class PGSql {
public:
    PGSql( std::string connect_string, int max_conn = 20 );
    PGSql( const PGSql& orig );

    class ConnectionHolder {
    public:
        ConnectionHolder( PGSql& db );
        ~ConnectionHolder();

        ConnectionPTR get();
    private:
        ConnectionPTR holded;
        PGSql& db;
    };

     TransactionPTR openTransaction( ConnectionPTR conn, std::string tr_name );

private:
    std::string connect_string;
    boost::mutex dbLock;

    ConnectionPool pool;
};

class PGSqlConnPoolSystem: public boost::serialization::singleton<PGSqlConnPoolSystem> {
    PGSql db;
public:
    PGSql& getdb() { return db; }
    PGSqlConnPoolSystem():db(ConfigManager::Instance()->getProperty<std::string>( "system.database.connectstring" ),
                             ConfigManager::Instance()->getProperty<int>( "system.database.connections" )) {}
};

class PGSqlConnPoolStats: public boost::serialization::singleton<PGSqlConnPoolStats> {
    PGSql db;
public:
    PGSql& getdb() { return db; }
    PGSqlConnPoolStats():db(ConfigManager::Instance()->getProperty<std::string>( "system.database.connectstring" ),
                             ConfigManager::Instance()->getProperty<int>( "system.database.admconnections" )) {}
};

#endif // PGSQL_H
