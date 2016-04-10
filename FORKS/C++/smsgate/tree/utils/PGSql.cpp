#include "PGSql.h"

using std::string;

ConnectionPool::ConnectionPool( std::string connect_string, int max_conn ) {
    this->max_conn = max_conn;
    this->busy_conn = 0;
    conn_string = connect_string;

    for ( int i = 0; i < max_conn; i++ ) {
        connPool.push_back( openConnection() );
    }
}

ConnectionPool::ConnectionPool( const ConnectionPool& orig ) {
    this->max_conn = orig.max_conn;
    this->busy_conn = 0;
    this->conn_string = orig.conn_string;

    for ( int i = 0; i < max_conn; i++ ) {
        connPool.push_back( openConnection() );
    }
}

ConnectionPTR ConnectionPool::openConnection() {
    return ConnectionPTR( new Connection( conn_string ) );
}

void ConnectionPool::freeConnection( ConnectionPTR cptr ) {
    boost::mutex::scoped_lock lock(getConnMut);
    this->busy_conn--;
    connPool.push_back( cptr );
}

ConnectionPTR ConnectionPool::getConnection() { boost:

    while( 1 ) {
        {
            boost::mutex::scoped_lock lock(getConnMut);
            if ( busy_conn < max_conn ) {
                busy_conn++;
                ConnectionPTR cptr = connPool.front();
                connPool.pop_front();

                return cptr;
            }
        }
        boost::xtime xt;
        boost::xtime_get(&xt, boost::TIME_UTC_);
        xt.nsec += 1e8;
        boost::thread::sleep(xt);
    }

}

PGSql::ConnectionHolder::ConnectionHolder( PGSql& _db ): db( _db ) {
    holded = db.pool.getConnection();
}

PGSql::ConnectionHolder::~ConnectionHolder( ) { 
    db.pool.freeConnection( holded );
}

ConnectionPTR PGSql::ConnectionHolder::get() {
    return holded;
}

PGSql::PGSql( std::string conn_string, int max_conn ): pool( conn_string, max_conn ) {
}

PGSql::PGSql( const PGSql& orig ): pool( orig.pool ), connect_string( orig.connect_string ) {
}

TransactionPTR PGSql::openTransaction( ConnectionPTR conn, std::string tr_name ) {
    boost::mutex::scoped_lock lock(dbLock);
    return TransactionPTR( new Transaction( *conn, tr_name ) );
}

