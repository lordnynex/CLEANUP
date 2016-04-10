#include "RoutingEditor.h"

#include "Route.h"
#include "MessageClassifier.h"
#include "utils.h"
#include "PartnerManager.h"

#include <string>
#include <algorithm>
#include <boost/bind.hpp>
#include <iostream>
#include <fstream>


using namespace Wt;
using namespace std;

RouteEditor::RouteEditor( std::string _userid, WContainerWidget* parent ): WContainerWidget( parent ) {
    userid = _userid;
    PartnerInfo user = PartnerManager::get_mutable_instance().findById( userid );
    columns_width.push_back(270);
    columns_width.push_back(70);
    columns_width.push_back(90);
    if ( user.ownerId.empty() ) {
        columns_width.push_back(90);
        columns_width.push_back(90);
    }
    elements_per_page = 25;

    model_ = new WStandardItemModel();
    buildModel( model_, route );
    treeView_ = buildTreeView( model_ );

    tlistBox = new WComboBox();
    tlistRebuild();

    WPushButton* loadBtn = new WPushButton( WString::fromUTF8( "Загрузить" ) );
    loadBtn->setMinimumSize( WLength( 2, WLength::Centimeter ), WLength::Auto );
    loadBtn->clicked().connect( this, &RouteEditor::onChangeRoot );
    loadBtn->clicked().connect( this, &RouteEditor::onRouteLoad );

    WPushButton* removeBtn = new WPushButton( WString::fromUTF8( "Удалить" ) );
    removeBtn->setMinimumSize( WLength( 2, WLength::Centimeter ), WLength::Auto );
    removeBtn->clicked().connect( this, &RouteEditor::onRouteRemove );

    nameBox = new WLineEdit();
    nameBox->setMinimumSize( WLength( 3, WLength::Centimeter ), WLength::Auto );

    WPushButton* saveBtn = new WPushButton( WString::fromUTF8( "Сохранить" ) );
    saveBtn->clicked().connect( this, &RouteEditor::onRouteSave );

    WPushButton* clearBtn = new WPushButton( WString::fromUTF8( "Очистить" ) );
    clearBtn->clicked().connect( this, &RouteEditor::onChangeRoot );
    clearBtn->clicked().connect( this, &RouteEditor::onRouteClear );

    WPushButton* updateBtn = new WPushButton( WString::fromUTF8( "Обновить" ) );
    updateBtn->clicked().connect( this, &RouteEditor::onChangeRoot );
    updateBtn->clicked().connect( this, &RouteEditor::onRouteUpdate );

    if ( user.ownerId.empty() ) {
        recendEditor = new RecendEditor( &route );
        firstgwEditor = new FirstgwEditor( &route );
        secondgwEditor = new SecondgwEditor( &route );
        thirdgwEditor = new ThirdgwEditor( &route );
        firstSlippageEditor = new FirstGwSlippageEditor( &route );
        secondSlippageEditor = new SecondGwSlippageEditor( &route );
    } else {
        ufirstgwEditor = new UserFirstgwEditor( &route );
    }

    WGridLayout* loadSaveLayout = new WGridLayout();
    loadSaveLayout->addWidget( tlistBox, 0, 0 );
    loadSaveLayout->addWidget( loadBtn, 0, 1 );
    loadSaveLayout->addWidget( removeBtn, 0, 2 );
    loadSaveLayout->addWidget( nameBox, 1, 0 );
    loadSaveLayout->addWidget( saveBtn, 1, 1 );
    loadSaveLayout->addWidget( clearBtn, 1, 2 );
    loadSaveLayout->addWidget( updateBtn, 2, 2 );

    WGroupBox* loadSaveBox = new WGroupBox( WString::fromUTF8( "Загрузить/Сохранить" ) );
    loadSaveBox->setLayout( loadSaveLayout, AlignCenter | AlignMiddle );

    WGridLayout* routeOptionsLayout = new WGridLayout();
    if ( user.ownerId.empty() ) {
        routeOptionsLayout->addLayout( recendEditor, 0, 0 );
        routeOptionsLayout->addLayout( firstgwEditor, 1, 0 );
        routeOptionsLayout->addLayout( secondgwEditor, 0, 1 );
        routeOptionsLayout->addLayout( thirdgwEditor, 1, 1 );
        routeOptionsLayout->addLayout( firstSlippageEditor, 0, 2 );
        routeOptionsLayout->addLayout( secondSlippageEditor, 1, 2 );
    } else {
        routeOptionsLayout->addLayout( ufirstgwEditor, 0, 0 );
    }

    WGroupBox* routeOptionsBox = new WGroupBox( WString::fromUTF8( "Тарифные опции" ) );
    routeOptionsBox->setLayout( routeOptionsLayout, AlignCenter | AlignMiddle);

    WBorderLayout* root = new WBorderLayout();
    root->addWidget( treeView_, WBorderLayout::West );
    root->addWidget( routeOptionsBox, WBorderLayout::Center );
    root->addWidget( loadSaveBox, WBorderLayout::East );

    setLayout( root );
    resizeTreeView( treeView_ );
    onChangeRoot();
}

void RouteEditor::buildModel( WStandardItemModel* data, Route& route, bool clear ) {
    sms::MessageClassifier::CountryOperatorMapT comap = sms::MessageClassifier::get_mutable_instance().getCOMap();
    PartnerInfo user = PartnerManager::get_mutable_instance().findById( userid );

    if ( clear ) {
        data->clear();
        if ( user.ownerId.empty() ) {
            data->insertColumns(0, 5);
        } else {
             data->insertColumns(0, 3);
        }

        data->setHeaderData(0, Horizontal, WString::fromUTF8("Страна/Оператор"));
        data->setHeaderData(1, Horizontal, WString::fromUTF8("MCC/MNC"));
        data->setHeaderData(2, Horizontal, WString::fromUTF8("1й шлюз"));
        if ( user.ownerId.empty() ) {
            data->setHeaderData(3, Horizontal, WString::fromUTF8("2й шлюз"));
            data->setHeaderData(4, Horizontal, WString::fromUTF8("3й шлюз"));
        }

        for( sms::MessageClassifier::CountryOperatorMapT::iterator it = comap.begin(); it != comap.end(); it++ ) {
            sms::MessageClassifier::CountryInfo cinfo = it->second;
            WStandardItem *country;
            WStandardItem* mcc;
            WStandardItem* firstgw;
            WStandardItem* secondgw;
            WStandardItem* thirdgw;

            country = new WStandardItem( string( "resources/flags/" ) + cinfo.cCode + ".png", WString::fromUTF8( cinfo.cName ) );
            mcc = new WStandardItem( WString::fromUTF8( cinfo.mcc ) );
            if ( user.ownerId.empty() ) {
                firstgw = new WStandardItem( WString::fromUTF8( route.getOption<Route::RouteFirstGate>( cinfo.mcc ).getValue() ) );
                secondgw = new WStandardItem( WString::fromUTF8( route.getOption<Route::RouteSecondGate>( cinfo.mcc ).getValue() ) );
                thirdgw = new WStandardItem( WString::fromUTF8( route.getOption<Route::RouteThirdGate>( cinfo.mcc ).getValue() ) );
            } else {
                firstgw = new WStandardItem( WString::fromUTF8( route.getOption<Route::UserRouteFirstGate>( cinfo.mcc ).getValue() ) );
            }

            std::vector< WStandardItem* > row;
            row.push_back( country );
            row.push_back( mcc );
            row.push_back( firstgw );
            if ( user.ownerId.empty() ) {
                row.push_back( secondgw );
                row.push_back( thirdgw );
            }

            for ( sms::MessageClassifier::CountryInfo::OperatorMapT::iterator gt = cinfo.operators.begin(); gt != cinfo.operators.end(); gt++ ) {
                sms::MessageClassifier::OperatorInfo info = gt->second;

                WStandardItem* op;
                WStandardItem* code;
                WStandardItem* firstgw;
                WStandardItem* secondgw;
                WStandardItem* thirdgw;

                op = new WStandardItem( WString::fromUTF8( info.getName() ) );
                code = new WStandardItem( WString::fromUTF8( info.getCode() ) );
                if ( user.ownerId.empty() ) {
                    firstgw = new WStandardItem( WString::fromUTF8( route.getOption<Route::RouteFirstGate>( info.mcc, info.mnc ).getValue() ) );
                    secondgw = new WStandardItem( WString::fromUTF8( route.getOption<Route::RouteSecondGate>( info.mcc, info.mnc ).getValue() ) );
                    thirdgw = new WStandardItem( WString::fromUTF8( route.getOption<Route::RouteThirdGate>( info.mcc, info.mnc ).getValue() ) );
                } else {
                    firstgw = new WStandardItem( WString::fromUTF8( route.getOption<Route::UserRouteFirstGate>( info.mcc, info.mnc ).getValue() ) );
                }

                std::vector< WStandardItem* > row;
                row.push_back( op );
                row.push_back( code );
                row.push_back( firstgw );
                if ( user.ownerId.empty() ) {
                    row.push_back( secondgw );
                    row.push_back( thirdgw );
                }

                country->appendRow( row );
            }

            data->appendRow( row );
        }

    } else
    for ( int row = 0; row < data->rowCount(); row++ ) {
        WStandardItem* root_item = data->item( row, 0 );
        {
            WStandardItem* imsi_item;
            WStandardItem* firstgw_item;
            WStandardItem* secondgw_item;
            WStandardItem* thirdgw_item;

            imsi_item = data->item( row, 1 );
            firstgw_item = data->item( row, 2 );
            if ( user.ownerId.empty() ) {
                secondgw_item = data->item( row, 3 );
                thirdgw_item = data->item( row, 4 );
            }

            std::string imsi = imsi_item->text().toUTF8();
            std::string mcc = imsi.substr( 0, 3 );

//            boost::logic::tribool hasPrice = route.hasOption< Route::Price >( mcc );


//            if ( !hasPrice ) {
//                ruprice_item->setIcon( "resources/attention.png" );
//            } else {
//                ruprice_item->setText( WString::fromUTF8( price_rur_text ) );
//            }

            if ( user.ownerId.empty() ) {
                firstgw_item->setText( WString::fromUTF8( route.getOption<Route::RouteFirstGate>( mcc ).getValue() ) );
                secondgw_item->setText( WString::fromUTF8( route.getOption<Route::RouteSecondGate>( mcc ).getValue() ) );
                thirdgw_item->setText( WString::fromUTF8( route.getOption<Route::RouteThirdGate>( mcc ).getValue() ) );
            } else {
                firstgw_item->setText( WString::fromUTF8( route.getOption<Route::UserRouteFirstGate>( mcc ).getValue() ) );
            }
        }

        for ( int subrow = 0; subrow < root_item->rowCount(); subrow++ ) {
            WStandardItem* imsi_item;
            WStandardItem* firstgw_item;
            WStandardItem* secondgw_item;
            WStandardItem* thirdgw_item;

            imsi_item = root_item->child( subrow, 1 );
            firstgw_item = root_item->child( subrow, 2 );
            if ( user.ownerId.empty() ) {
                secondgw_item = root_item->child( subrow, 3 );
                thirdgw_item = root_item->child( subrow, 4 );
            }

            std::string imsi = imsi_item->text().toUTF8();
            std::string mcc = imsi.substr( 0, 3 );
            std::string mnc = imsi.substr( 3, imsi.length() - 3 );

//            boost::logic::tribool hasPrice = route.hasOption< Route::Price >( mcc, mnc );

//            if ( !hasPrice ) {
//                ruprice_item->setIcon( "resources/attention.png" );
//            } else {
//                ruprice_item->setText( WString::fromUTF8( price_rur_text ) );
//            }

            if ( user.ownerId.empty() ) {
                firstgw_item->setText( WString::fromUTF8( route.getOption<Route::RouteFirstGate>( mcc, mnc ).getValue() ) );
                secondgw_item->setText( WString::fromUTF8( route.getOption<Route::RouteSecondGate>( mcc, mnc ).getValue() ) );
                thirdgw_item->setText( WString::fromUTF8( route.getOption<Route::RouteThirdGate>( mcc, mnc ).getValue() ) );
            } else {
                firstgw_item->setText( WString::fromUTF8( route.getOption<Route::UserRouteFirstGate>( mcc, mnc ).getValue() ) );
            }
        }
    }

}

WTreeView* RouteEditor::buildTreeView( Wt::WStandardItemModel * model ) {

    WTreeView* tw = new WTreeView();
    tw->setModel( model );
    tw->setSelectionMode( Wt::ExtendedSelection );
    tw->setAlternatingRowColors( true );
    tw->sortByColumn( 0, AscendingOrder);

    tw->clicked().connect( this, &RouteEditor::onChangeRoot );

    return tw;
}

void RouteEditor::resizeTreeView( WTreeView* tw) {
    int columns_total_width = 0;
    int scroll_width = 10 + columns_width.size()*10;
    for ( int i = 0; i < columns_width.size(); columns_total_width += columns_width[i++] ) {}


    tw->resize(
                WLength( scroll_width + columns_total_width, WLength::Pixel ),
                WLength( elements_per_page * tw->rowHeight().toPixels() + tw->headerHeight().toPixels(), WLength::Pixel )
                );

    for ( int i = 0; i < columns_width.size(); i++, columns_total_width++ ) {
        tw->setColumnWidth( i, WLength( columns_width[i], WLength::Pixel) );
    }
}

double RouteEditor::sdouble2double( string v, double defval ) {
    double val;
    try {
        val = boost::lexical_cast< double >( v );
    } catch ( ... ) {
        val =  defval;
    }
    return val;
}

string RouteEditor::double2string( double v ) {
    char buf[100];
    sprintf( buf, "%0.4f", v );

    return buf;
}

void RouteEditor::onRouteLoad() {
    std::string name = tlistBox->currentText().toUTF8();
    route = RouteManager::get_mutable_instance().loadRoute( name );

    onRouteUpdate();

    nameBox->setText( WString::fromUTF8( name ) );
}

void RouteEditor::onRouteRemove() {
    std::string name = tlistBox->currentText().toUTF8();
    RouteManager::get_mutable_instance().removeRoute( name );

    tlistRebuild();
}

void RouteEditor::onRouteClear() {
    route = Route();

    buildModel( model_, route, false );
}

void RouteEditor::onRouteUpdate() {
    buildModel( model_, route, false );
}

void RouteEditor::onRouteSave() {
    PartnerInfo user = PartnerManager::get_mutable_instance().findById( userid );
    std::string name = nameBox->text().toUTF8();
    route.setName( name );
    RouteManager::get_mutable_instance().saveRoute( name, user.ownerId.empty()? "": userid, route );

    tlistRebuild();
}

void RouteEditor::tlistRebuild() {
    PartnerInfo user = PartnerManager::get_mutable_instance().findById( userid );
    tlistBox->clear();
    std::list< std::string > tlist = RouteManager::get_mutable_instance().routes_list( user.ownerId.empty()? "": userid );

    for ( std::list< std::string >::iterator it = tlist.begin(); it != tlist.end(); it++ ) {
        tlistBox->addItem( WString::fromUTF8( *it ) );
    }
}

void RouteEditor::onChangeRoot() {
    Wt::WModelIndexSet selected = treeView_->selectedIndexes();
    PartnerInfo user = PartnerManager::get_mutable_instance().findById( userid );

    if ( selected.empty() ) {
        if ( user.ownerId.empty() ) {
            recendEditor->setCurPosRoot();
            firstgwEditor->setCurPosRoot();
            secondgwEditor->setCurPosRoot();
            thirdgwEditor->setCurPosRoot();
            firstSlippageEditor->setCurPosRoot();
            secondSlippageEditor->setCurPosRoot();
        } else {
            ufirstgwEditor->setCurPosRoot();
        }
    }

    if ( selected.size() == 1 ) {
        Wt::WModelIndex index = *selected.begin();

        WStandardItem* root = model_->itemFromIndex( index.parent() );
        WStandardItem* item = root->child( index.row(), 1 );

        std::string mccmnc = item->text().toUTF8();

        std::string mcc = mccmnc.substr( 0, 3 );
        std::string mnc = mccmnc.substr( 3, mccmnc.length()-3 );

        if ( mnc.empty() ) {
            if ( user.ownerId.empty() ) {
                recendEditor->setCurPosCountry(mcc);
                firstgwEditor->setCurPosCountry(mcc);
                secondgwEditor->setCurPosCountry(mcc);
                thirdgwEditor->setCurPosCountry(mcc);
                firstSlippageEditor->setCurPosCountry(mcc);
                secondSlippageEditor->setCurPosCountry(mcc);
            } else {
                ufirstgwEditor->setCurPosCountry(mcc);
            }

        } else {
            if ( user.ownerId.empty() ) {
                recendEditor->setCurPosOperator( mcc, mnc );
                firstgwEditor->setCurPosOperator( mcc, mnc );
                secondgwEditor->setCurPosOperator( mcc, mnc );
                thirdgwEditor->setCurPosOperator( mcc, mnc );
                firstSlippageEditor->setCurPosOperator( mcc, mnc );
                secondSlippageEditor->setCurPosOperator( mcc, mnc );
            } else {
                ufirstgwEditor->setCurPosOperator( mcc, mnc );
            }
        }
    }
}
