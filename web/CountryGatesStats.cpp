#include "CountryGatesStats.h"
#include "StatManager.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>

WApplication *createGatesStatsPage(const WEnvironment& env) {
  /*
   * You could read information from the environment to decide whether
   * the user has permission to start a new application
   */
  return new CountryGatesStats(env);
}


CountryGatesStats::CountryGatesStats( const WEnvironment& env ): WApplication( env ) {
    setTitle( "SMSGate countries-gates stats page" );

    this->setCssTheme("polished");
    this->useStyleSheet( "/resources/css/cgstats.css" );

    sms::StatManager::TCountryInfoTable data = sms::StatManager::Instance()->getCountryInfoLastUpdate();
    tbl = new WTable;
    tbl->setHeaderCount(1);
    tbl->setStyleClass( "cgstatst" );
    popup_shown = false;

    if ( !data.empty() )
    for ( int j = 0; j < data[0].size(); j++ )
        tbl->elementAt( 0, j+1 )->addWidget( new WLabel(  data[0][j].gname) );

    int last_total_index = data.size()-1;
    for ( int i = data.size()-1; i >= 0; i-- ) {
        if ( !data[i].empty() ) {
            if (( data[i][0].opname == "Total" ) && ( i != last_total_index )) {
                WLabel* lbl = new WLabel( WString::fromUTF8( data[i][0].cname + string(":") + data[i][0].opname ) );
                lbl->clicked().connect( boost::bind( &CountryGatesStats::onExpand, this, i+2, last_total_index ) );
                last_total_index = i;
                tbl->elementAt( i+1, 0 )->addWidget( lbl );
            }
            if ( data[i][0].opname != "Total" ) {
                WLabel* lbl = new WLabel( WString::fromUTF8( string( "---" ) + data[i][0].cname + string(":") + data[i][0].opname ) );
                tbl->rowAt( i+1 )->hide();
                tbl->elementAt( i+1, 0 )->addWidget( lbl );
            }
        }

        for ( int j = 0; j < data[i].size(); j++ ) {
            std::ostringstream head, foot;
            double quality = double(data[i][j].deliveres*100) / ( data[i][j].requests == 0 ? 1: data[i][j].requests );
            if ( quality >= 100 ) { quality = 100; }
            foot.setf(std::ios::fixed ,std::ios::floatfield);
            foot.precision(1);

            head << data[i][j].deliveres << ":" << data[i][j].requests;
            foot << quality << "%";
            if ( data[i][j].requests == 0 )
                /* do nothing */ ;
            else if ( quality > 80 )
                tbl->elementAt( i+1, j+1 )->setStyleClass( "rowOk" );
            else if ( quality > 20  )
                tbl->elementAt( i+1, j+1 )->setStyleClass( "rowWarn" );
            else {
                if ( data[i][j].requests < 10 )
                    tbl->elementAt( i+1, j+1 )->setStyleClass( "rowWarn" );
                else
                    tbl->elementAt( i+1, j+1 )->setStyleClass( "rowErr" );
            }

            if ( data[i][j].requests != 0 ) {
                WLabel* lbl = new WLabel( foot.str() );
                lbl->clicked().connect( boost::bind( &CountryGatesStats::onMoreInfo, this, _1, i, j ) );
                tbl->elementAt( i+1, j+1 )->addWidget( lbl );
            }
        }
    }

    root()->addWidget( tbl );
}

void CountryGatesStats::onExpand( int from, int to ) {

    for ( int i = from; i <= to; i++ ) {
        if ( tbl->rowAt( i )->isHidden() )
            tbl->rowAt( i )->show();
        else
            tbl->rowAt( i )->hide();
    }
}

void CountryGatesStats::onMoreInfo( const WMouseEvent& e, int row, int column ) {
    if ( popup_shown == true ) return;

    std::ostringstream req, dlr, qua;
    sms::StatManager::TCountryInfoTable data = sms::StatManager::Instance()->getCountryInfoLastUpdate();
    double quality = double(data[row][column].deliveres*100) / ( data[row][column].requests == 0 ? 1: data[row][column].requests );
    if ( quality >= 100 ) { quality = 100; }
    qua.setf(std::ios::fixed ,std::ios::floatfield);
    qua.precision(1);

    req << "Requests: " << data[row][column].requests;
    dlr << "Delivered: " << data[row][column].deliveres;
    qua << "Quality: " << quality << "%";

    WPopupMenu* popup = new WPopupMenu();

    popup->addItem( req.str() );
    popup->addItem( dlr.str() );
    popup->addItem( qua.str() );

    popup_shown = true;
    popup->aboutToHide().connect( boost::bind( &CountryGatesStats::popup_destroyed, this ) );

    popup->popup( e );
}

void CountryGatesStats::popup_destroyed() {
    popup_shown = false;
}

CountryGatesStats::~CountryGatesStats() {
}
