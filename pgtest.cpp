#include "PGSql.h"

#include <iostream>

int main() {

    PGSql db( "dbname=smsgate user=mohtep password=a6sent" );
    ConnectionPTR conn = PGSql::ConnectionHolder( db ).get();
    TransactionPTR tr = db.openTransaction( conn, "tr1" );
    Result res = tr->exec( "SELECT * FROM gateways;" );
    for ( Result::const_iterator it = res.begin(); it != res.end(); it++ ) {
        std::cout << (*it)[0];
    }
    tr->commit();

}

