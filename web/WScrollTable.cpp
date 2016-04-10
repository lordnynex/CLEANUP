#include "WScrollTable.h"

WScrollTable::WScrollTable( Storage& _header, Storage& _data, Storage& _footer, Wt::WContainerWidget *parent ):
    Wt::WTable( parent ),
    header( _header ),
    footer( _footer ),
    data( _data )
{
    headerlen = 0;
    footerlen = 0;
    datalen = 0;

    skipped = 0;

    limit = 100;
}

void WScrollTable::setPageLimit( int limit ) {
    this->limit = limit;
}

void WScrollTable::buildData( int offset ) {
    WDataSource< RowType >::RowList rows;
    WDataSource< RowType >::RowList::iterator it;
    int rownum;
    int colnum;
    skipped = offset;
    rows = data.getLineRange( offset, (offset + limit) > (data.getTotalLines() - 1)? (data.getTotalLines() - 1): (offset + limit - 1) );
    for ( rownum = header.getTotalLines(), it = rows.begin(); it != rows.end(); it++, rownum++ ) {
        RowType& row = *it;
        insertRow( rownum );
        datalen++;
        for ( colnum = 0; colnum < row.size(); colnum++ ) {
            if ( row[ colnum ] )
                elementAt( rownum, colnum )->addWidget( row[ colnum ] );
        }
    }
}

void WScrollTable::buildFooter() {
    WDataSource< RowType >::RowList rows;
    WDataSource< RowType >::RowList::iterator it;
    int rownum;
    int colnum;
    rows = footer.getLineRange( 0, footer.getTotalLines() - 1 );
    for ( rownum = header.getTotalLines() + data.getTotalLines(), it = rows.begin(); it != rows.end(); it++, rownum++ ) {
        RowType& row = *it;
        insertRow( rownum );
        footerlen++;
        for ( colnum = 0; colnum < row.size(); colnum++ ) {
            if ( row[ colnum ] )
                this->elementAt( rownum, colnum )->addWidget( row[ colnum ] );
        }
    }
}

void WScrollTable::buildHeader() {
    WDataSource< RowType >::RowList rows;
    WDataSource< RowType >::RowList::iterator it;
    int rownum;
    int colnum;
    rows = header.getLineRange( 0, header.getTotalLines() - 1 );
    setHeaderCount( rows.size() );
    for ( rownum = 0, it = rows.begin(); it != rows.end(); it++, rownum++ ) {
        RowType& row = *it;
        insertRow( rownum );
        headerlen++;
        for ( colnum = 0; colnum < row.size(); colnum++ ) {
            if ( row[ colnum ] )
                this->elementAt( rownum, colnum )->addWidget( row[ colnum ] );
        }
    }
}

void WScrollTable::rebuildData( int offset ) {
    for ( int i = 0; i < datalen; i++ ) {
        this->deleteRow( headerlen );
    }

    datalen = 0;
    buildData( offset );
}
