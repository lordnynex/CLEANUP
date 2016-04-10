#ifndef DATASOURCE_H
#define DATASOURCE_H

#include "PGSql.h"

template < class RowType >
class WDataSource {
public:
    typedef std::vector< RowType > RowList;
    WDataSource();
    virtual int getTotalLines() = 0;

    RowList getLineRange( int lnl, int lnr );
    void releaseCache();
protected:
    virtual void execute( int lnl, int lnr, RowList& data ) = 0;
    RowList cache;
};

template < class RowType >
WDataSource< RowType >::WDataSource() {
}

template < class RowType >
typename WDataSource< RowType >::RowList
WDataSource< RowType >::getLineRange( int lnl, int lnr ) {
    releaseCache();
    execute( lnl, lnr, cache );
    return cache;
}

template < class RowType >
void WDataSource< RowType >::releaseCache() {
    cache.resize( 0 );
}


#endif // DATASOURCE_H
