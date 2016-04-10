#ifndef WSCROLLTABLE_H
#define WSCROLLTABLE_H

#include <boost/shared_ptr.hpp>

#include <Wt/WTable>
#include "DataSource.h"

class WScrollTable: public Wt::WTable {
public:
    typedef std::vector< Wt::WWidget* > RowType ;
    typedef WDataSource< RowType > Storage;
    WScrollTable( Storage& header, Storage& data, Storage& footer, Wt::WContainerWidget *parent=0 );

    void rebuildData( int offset = 0 );
    void setPageLimit( int limit );

    void buildHeader();
    void buildFooter();
    int getPage() { return skipped / limit; }
    int getLastPage() { return (data.getTotalLines()-1) / limit; }
    void buildData( int offset = 0 );
    void nextPage() {
        if ( skipped + limit < data.getTotalLines() ) {
            rebuildData( skipped + limit );
        }
    }

    void prevPage() {
        if ( skipped - limit >= 0 ) {
            rebuildData( skipped - limit );
        }
    }

    void exactPage( int page ) {
        if ( ( (page-1)*limit < data.getTotalLines() ) &&
             ( (page-1)*limit >= 0 ) )
        {
                rebuildData( (page-1)*limit );
        }
    }
private:
    Storage& header;
    Storage& footer;
    Storage& data;

    int headerlen;
    int footerlen;
    int datalen;

    int limit;
    int skipped;
};

#endif // WSCROLLTABLE_H
