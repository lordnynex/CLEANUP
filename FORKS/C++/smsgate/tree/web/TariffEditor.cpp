#include "TariffEditor.h"
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

TariffEditor::TariffEditor( std::string _userid, WContainerWidget* parent ): WContainerWidget( parent ) {
    userid = _userid;
    columns_width.push_back(270);
    columns_width.push_back(70);
    columns_width.push_back(90);
    columns_width.push_back(60);
    columns_width.push_back(70);
    elements_per_page = 25;

    model_ = new WStandardItemModel();
    buildModel( model_, tariff );
    treeView_ = buildTreeView( model_ );

    exportBtn = new WPushButton( WString::fromUTF8( "Экспорт" ) );
    exportBtn->clicked().connect( this, &TariffEditor::exportToCsv );

    importBtn = new WPushButton( WString::fromUTF8( "Импорт" ) );
    importBtn->clicked().connect( this, &TariffEditor::importFromCsv);

    tlistBox = new WComboBox();
    tlistRebuild();

    WPushButton* loadBtn = new WPushButton( WString::fromUTF8( "Загрузить" ) );
    loadBtn->setMinimumSize( WLength( 2, WLength::Centimeter ), WLength::Auto );
    loadBtn->clicked().connect( this, &TariffEditor::onChangeRoot );
    loadBtn->clicked().connect( this, &TariffEditor::onTariffLoad );

    WPushButton* removeBtn = new WPushButton( WString::fromUTF8( "Удалить" ) );
    removeBtn->setMinimumSize( WLength( 2, WLength::Centimeter ), WLength::Auto );
    removeBtn->clicked().connect( this, &TariffEditor::onTariffRemove );

    nameBox = new WLineEdit();
    nameBox->setMinimumSize( WLength( 3, WLength::Centimeter ), WLength::Auto );

    WPushButton* saveBtn = new WPushButton( WString::fromUTF8( "Сохранить" ) );
    saveBtn->clicked().connect( this, &TariffEditor::onTariffSave );

    WPushButton* clearBtn = new WPushButton( WString::fromUTF8( "Очистить" ) );
    clearBtn->clicked().connect( this, &TariffEditor::onChangeRoot );
    clearBtn->clicked().connect( this, &TariffEditor::onTariffClear );

    WPushButton* updateBtn = new WPushButton( WString::fromUTF8( "Обновить" ) );
    updateBtn->clicked().connect( this, &TariffEditor::onChangeRoot );
    updateBtn->clicked().connect( this, &TariffEditor::onTariffUpdate );

    currencyEditor = new CurrencyEditor( &tariff );
    unknownPolicy = new UnknownPolicyEditor( &tariff );
    paidStatuses = new PaidStatusesEditor( &tariff );

    WGridLayout* loadSaveLayout = new WGridLayout();
    loadSaveLayout->addWidget( tlistBox, 0, 0 );
    loadSaveLayout->addWidget( loadBtn, 0, 1 );
    loadSaveLayout->addWidget( removeBtn, 0, 2 );
    loadSaveLayout->addWidget( nameBox, 1, 0 );
    loadSaveLayout->addWidget( saveBtn, 1, 1 );
    loadSaveLayout->addWidget( clearBtn, 1, 2 );
    loadSaveLayout->addWidget( exportBtn, 2, 0 );
    loadSaveLayout->addWidget( importBtn, 2, 1 );
    loadSaveLayout->addWidget( updateBtn, 2, 2 );

    WGroupBox* loadSaveBox = new WGroupBox( WString::fromUTF8( "Загрузить/Сохранить" ) );
    loadSaveBox->setLayout( loadSaveLayout, AlignCenter | AlignMiddle );

    WGridLayout* tariffOptionsLayout = new WGridLayout();
    tariffOptionsLayout->addLayout( currencyEditor, 0, 0 );
    tariffOptionsLayout->addLayout( unknownPolicy, 0, 1 );
    tariffOptionsLayout->addLayout( paidStatuses, 1, 0 );

    WGroupBox* tariffOptionsBox = new WGroupBox( WString::fromUTF8( "Тарифные опции" ) );
    tariffOptionsBox->setLayout( tariffOptionsLayout, AlignCenter | AlignMiddle);

    WBorderLayout* root = new WBorderLayout();
    root->addWidget( treeView_, WBorderLayout::West );
    root->addWidget( tariffOptionsBox, WBorderLayout::Center );
    root->addWidget( loadSaveBox, WBorderLayout::East );

    setLayout( root );
    resizeTreeView( treeView_ );
    onChangeRoot();
}

void TariffEditor::buildModel( WStandardItemModel* data, Tariff& tariff, bool clear ) {
    sms::MessageClassifier::CountryOperatorMapT comap = sms::MessageClassifier::get_mutable_instance().getCOMap();

    if ( clear ) {
        data->clear();
        data->insertColumns(0, 5);

        data->setHeaderData(0, Horizontal, WString::fromUTF8("Страна/Оператор"));
        data->setHeaderData(1, Horizontal, WString::fromUTF8("MCC/MNC"));
        data->setHeaderData(2, Horizontal, WString::fromUTF8("Цена,RUR"));
        data->setHeaderData(3, Horizontal, WString::fromUTF8("Цена"));
        data->setHeaderData(4, Horizontal, WString::fromUTF8("Валюта"));

        for( sms::MessageClassifier::CountryOperatorMapT::iterator it = comap.begin(); it != comap.end(); it++ ) {
            sms::MessageClassifier::CountryInfo cinfo = it->second;
            WStandardItem *country;
            WStandardItem* mcc;
            WStandardItem* ruprice;
            WStandardItem* price;
            WStandardItem* currency;

            double price_rur = tariff.costs( cinfo.mcc );
            string price_rur_text = ( price_rur == Tariff::INVALID_VALUE )? "-.---": double2string( price_rur );
            string currency_text = tariff.getOption< Tariff::TariffCurrency >( cinfo.mcc ).getValue();
            string price_text = ( price_rur == Tariff::INVALID_VALUE )?
                        "-.---":
                        double2string( tariff.currencyConvert( Tariff::TariffCurrency(), tariff.getOption< Tariff::TariffCurrency >( cinfo.mcc ), price_rur ) );

            country = new WStandardItem( string( "resources/flags/" ) + cinfo.cCode + ".png", WString::fromUTF8( cinfo.cName ) );
            mcc = new WStandardItem( WString::fromUTF8( cinfo.mcc ) );
            ruprice = new WStandardItem( WString::fromUTF8( price_rur_text ) );
            price = new WStandardItem( WString::fromUTF8( price_text ) );
            currency = new WStandardItem( WString::fromUTF8( currency_text ) );

            std::vector< WStandardItem* > row;
            row.push_back( country );
            row.push_back( mcc );
            row.push_back( ruprice );
            row.push_back( price );
            row.push_back( currency );

            for ( sms::MessageClassifier::CountryInfo::OperatorMapT::iterator gt = cinfo.operators.begin(); gt != cinfo.operators.end(); gt++ ) {
                sms::MessageClassifier::OperatorInfo info = gt->second;

                WStandardItem* op;
                WStandardItem* code;
                WStandardItem* ruprice;
                WStandardItem* price;
                WStandardItem* currency;

                double price_rur = tariff.costs( cinfo.mcc, info.mnc );
                string price_rur_text = ( price_rur == Tariff::INVALID_VALUE )? "-.---": double2string( price_rur );
                string currency_text = tariff.getOption< Tariff::TariffCurrency >( cinfo.mcc, info.mnc ).getValue();
                string price_text = ( price_rur == Tariff::INVALID_VALUE )?
                            "-.---":
                            double2string( tariff.currencyConvert( Tariff::TariffCurrency(), tariff.getOption< Tariff::TariffCurrency >( cinfo.mcc, info.mnc ), price_rur ) );

                op = new WStandardItem( WString::fromUTF8( info.getName() ) );
                code = new WStandardItem( WString::fromUTF8( info.getCode() ) );
                ruprice = new WStandardItem( WString::fromUTF8( price_rur_text ) );
                price = new WStandardItem( WString::fromUTF8( price_text ) );
                currency = new WStandardItem( WString::fromUTF8( currency_text ) );

                std::vector< WStandardItem* > row;
                row.push_back( op );
                row.push_back( code );
                row.push_back( ruprice );
                row.push_back( price );
                row.push_back( currency );

                country->appendRow( row );
            }

            data->appendRow( row );
        }

    } else
    for ( int row = 0; row < data->rowCount(); row++ ) {
        WStandardItem* root_item = data->item( row, 0 );
        {
            WStandardItem* imsi_item = data->item( row, 1 );
            WStandardItem* ruprice_item = data->item( row, 2 );
            WStandardItem* price_item = data->item( row, 3 );
            WStandardItem* currency_item = data->item( row, 4 );

            std::string imsi = imsi_item->text().toUTF8();
            std::string mcc = imsi.substr( 0, 3 );

//            boost::logic::tribool hasPrice = tariff.hasOption< Tariff::Price >( mcc );
            double price_rur = tariff.costs( mcc );
            string price_rur_text = ( price_rur == Tariff::INVALID_VALUE )? "-.---": double2string( price_rur );
            string currency_text = tariff.getOption< Tariff::TariffCurrency >( mcc ).getValue();
            string price_text = ( price_rur == Tariff::INVALID_VALUE )?
                        "-.---":
                        double2string( tariff.currencyConvert( Tariff::TariffCurrency(), tariff.getOption< Tariff::TariffCurrency >( mcc ), price_rur ) );

//            if ( !hasPrice ) {
//                ruprice_item->setIcon( "resources/attention.png" );
//            } else {
//                ruprice_item->setText( WString::fromUTF8( price_rur_text ) );
//            }

            ruprice_item->setText( WString::fromUTF8( price_rur_text ) );
            price_item->setText( WString::fromUTF8( price_text ) );
            currency_item->setText( WString::fromUTF8( currency_text ) );

        }

        for ( int subrow = 0; subrow < root_item->rowCount(); subrow++ ) {
            WStandardItem* imsi_item = root_item->child( subrow, 1 );
            WStandardItem* ruprice_item = root_item->child( subrow, 2 );
            WStandardItem* price_item = root_item->child( subrow, 3 );
            WStandardItem* currency_item = root_item->child( subrow, 4 );

            std::string imsi = imsi_item->text().toUTF8();
            std::string mcc = imsi.substr( 0, 3 );
            std::string mnc = imsi.substr( 3, imsi.length() - 3 );

            double price_rur = tariff.costs( mcc, mnc );
//            boost::logic::tribool hasPrice = tariff.hasOption< Tariff::Price >( mcc, mnc );
            string price_rur_text = ( price_rur == Tariff::INVALID_VALUE )? "-.---": double2string( price_rur );
            string currency_text = tariff.getOption< Tariff::TariffCurrency >( mcc, mnc ).getValue();
            string price_text = ( price_rur == Tariff::INVALID_VALUE )?
                        "-.---":
                        double2string( tariff.currencyConvert( Tariff::TariffCurrency(), tariff.getOption< Tariff::TariffCurrency >( mcc, mnc ), price_rur ) );

//            if ( !hasPrice ) {
//                ruprice_item->setIcon( "resources/attention.png" );
//            } else {
//                ruprice_item->setText( WString::fromUTF8( price_rur_text ) );
//            }

            ruprice_item->setText( WString::fromUTF8( price_rur_text ) );
            price_item->setText( WString::fromUTF8( price_text ) );
            currency_item->setText( WString::fromUTF8( currency_text ) );
        }
    }

}

WTreeView* TariffEditor::buildTreeView( Wt::WStandardItemModel * model ) {

    WTreeView* tw = new WTreeView();
    tw->setModel( model );
    tw->setSelectionMode( Wt::ExtendedSelection );
    tw->setAlternatingRowColors( true );
    tw->sortByColumn( 0, AscendingOrder);

    tw->clicked().connect( this, &TariffEditor::onPriceEdit );
    tw->clicked().connect( this, &TariffEditor::onChangeRoot );

    return tw;
}

void TariffEditor::resizeTreeView( WTreeView* tw) {
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

void TariffEditor::onPriceEdit( Wt::WModelIndex index, Wt::WMouseEvent event ) {

    if ( index.column() != 3 ) {
        return;
    }

    WDialog summary;
    summary.setWindowTitle( WString::fromUTF8("Редактор цены") );
    summary.setTitleBarEnabled( true );
    summary.setPositionScheme( Wt::Absolute );
    summary.setOffsets( WLength( event.document().x, WLength::Pixel ), Wt::Left );
    summary.setOffsets( WLength( event.document().y, WLength::Pixel ), Wt::Top );

    WStandardItem* root = model_->itemFromIndex( index.parent() );
    WStandardItem* item = root->child( index.row(), index.column() );
    WModelIndex capital = model_->indexFromItem( root->child( index.row(), 0 ) );

    WString old_price = item->text();

    WLabel* oldp = new WLabel( old_price );
    WSpinBox* newp = new WSpinBox( );
    newp->setMinimum( 0 );
    newp->setMaxLength(6);
    newp->setMaximumSize( WLength( 2, WLength::Centimeter ), WLength::Auto );
    newp->setSingleStep( 0.001 );
    newp->setText( "0.001" );
    newp->setValidator( new WDoubleValidator( 0, 100 ) );
    newp->enterPressed().connect( &summary, &WDialog::accept );
    newp->enterPressed().connect( boost::bind( &TariffEditor::changeItemText, this, index, newp ) );

    WTable report( summary.contents() );
    report.setStyleClass("restable");
    report.elementAt(0, 0)->addWidget( new WLabel( WString::fromUTF8("Старая цена") ) );
    report.elementAt(1, 0)->addWidget( new WLabel( WString::fromUTF8("Новая цена") ) );

    report.elementAt(0, 1)->addWidget( oldp );
    report.elementAt(1, 1)->addWidget( newp );

    if ( root == model_->invisibleRootItem() ) {
        WPushButton* okAll = new WPushButton( WString::fromUTF8("Изменить все"), summary.contents() );
        okAll->clicked().connect(&summary, &WDialog::accept);
        okAll->clicked().connect( boost::bind( &TariffEditor::changeItemTextRecursive, this, capital, 3, newp ) );
    }

    WPushButton* okBtn = new WPushButton( WString::fromUTF8("Изменить"), summary.contents() );
    okBtn->clicked().connect(&summary, &WDialog::accept);
    okBtn->clicked().connect( boost::bind( &TariffEditor::changeItemText, this, index, newp ) );

    WPushButton* cancelBtn = new WPushButton( WString::fromUTF8("Отмена"), summary.contents() );
    cancelBtn->clicked().connect(&summary, &WDialog::reject);

    newp->setFocus();
    summary.exec();
}

void TariffEditor::changeItemText( Wt::WModelIndex index, WSpinBox* text ) {
    WStandardItem* root = model_->itemFromIndex( index.parent() );
    WStandardItem* item = root->child( index.row(), index.column() );

    std::string mccmnc = root->child( index.row(), 1 )->text().toUTF8();
    std::string mcc = mccmnc.substr( 0, 3 );
    std::string mnc = mccmnc.substr( 3, mccmnc.length() - 3 );

    if ( mnc.empty() ) {
        tariff.setPrice( mcc, text->value() );
    } else {
        tariff.setPrice( mcc, mnc, text->value() );
    }

    item->setText( text->text() );

}

void TariffEditor::changeItemTextRecursive( Wt::WModelIndex index, int column, WSpinBox* text ) {
    WStandardItem* root = model_->itemFromIndex( index.parent() );
    WStandardItem* item = root->child( index.row(), index.column() );

    changeItemText( model_->indexFromItem( root->child( index.row(), column ) ), text );

    for ( int i = 0; i < item->rowCount(); i++ ) {
        changeItemText( index.child( i, column ), text );
    }
}

void TariffEditor::exportToCsv() {
    Wt::WFileResource* csv = new WFileResource( this );

    csv->setFileName( "/tmp/123.csv" );
    csv->setMimeType( "text/txt" );
    csv->suggestFileName( "tariff.csv" );

    sms::MessageClassifier::CountryOperatorMapT comap = sms::MessageClassifier::get_mutable_instance().getCOMap();
    ofstream fout( "/tmp/123.csv" );

    WStandardItem* item = model_->invisibleRootItem();

    fout << utils::StringUtf8ToCp1251( "Страна;Оператор;MCC/MNC;\"Цена, руб\";\"Цена, ориг\";\"Валюта, ориг\"" ) << endl;
    recursivePrintCsv( fout, comap, item );

    fout.close();

    WApplication::instance()->redirect( csv->generateUrl() );
}

void TariffEditor::recursivePrintCsv( std::ostream& out, sms::MessageClassifier::CountryOperatorMapT& map, Wt::WStandardItem* item ) {
    for ( int i = 0; i < item->rowCount(); i++ ) {
        string mccmnc = item->child( i, 1 )->text().toUTF8();
        std::string mcc;
        std::string mnc;
        string price, price_s, price_c;

        if ( mccmnc.size()< 3 ) continue;
        mcc = mccmnc.substr(0, 3);
        mnc = mccmnc.substr(3, mccmnc.length()-3);

        price = sdouble2string( item->child( i, 2 )->text().toUTF8() );
        price_s = sdouble2string( item->child( i, 3 )->text().toUTF8() );
        price_c = item->child( i, 4 )->text().toUTF8();

        if ( price.find( '.' ) != price.npos ) {
            price[ price.find( '.' ) ] = ',';
        }

        if ( price_s.find( '.' ) != price_s.npos ) {
            price_s[ price_s.find( '.' ) ] = ',';
        }

        if ( mnc.empty() ) {
//            sms::MessageClassifier::CountryInfo ci = map[ mcc ];
//            out << "\"" << ci.cName << "\"" << ";";     // Country name
//            out << ";";                                 // Network name
//            out << "\"" << mcc << "\"" << ";";          // MCC
//            out << "\"" << price << "\"" << ";";        // Price
//            out << endl;

            if ( !item->child( i, 0 )->hasChildren() )
                continue;

            recursivePrintCsv( out, map, item->child( i, 0 ) );
            continue;
        }

        sms::MessageClassifier::CountryInfo ci = map[ mcc ];
        sms::MessageClassifier::OperatorInfo oi = map[ mcc ].operators[ mnc ];
        out << "\"" << ci.cName << "\"" << ";";     // Country name
        out << "\"" << oi.getName() << "\"" << ";"; // Network name
        out << "\"" << mcc << mnc << "\"" << ";";   // MCCMNC
        out << "\"" << price << "\"" << ";";        // Price
        out << "\"" << price_s << "\"" << ";";      // Price
        out << "\"" << price_c << "\"" << ";";      // Price
        out << endl;

    }
}

string TariffEditor::sdouble2string( string v, string defval ) {
    double val;
    try {
        val = boost::lexical_cast< double >( v );
    } catch ( ... ) {
        return defval;
    }
    return double2string( val );
}

double TariffEditor::sdouble2double( string v, double defval ) {
    double val;
    try {
        val = boost::lexical_cast< double >( v );
    } catch ( ... ) {
        val =  defval;
    }
    return val;
}

string TariffEditor::double2string( double v ) {
    char buf[100];
    sprintf( buf, "%0.4f", v );

    return buf;
}

void TariffEditor::importFromCsv() {
    WDialog import( WString::fromUTF8( "Испорт из CSV" ) );
    importCtx.importDlg = &import;

    importCtx.fake = new WContainerWidget( importCtx.importDlg->contents() );
    importCtx.fake->hide();

    importCtx.upload = new WFileUpload( );
    importCtx.upload->uploaded().connect( boost::bind( &TariffEditor::importFileDone, this ) );
    importCtx.upload->fileTooLarge().connect( boost::bind( &TariffEditor::importFileTooLargeError, this ) );

    importCtx.uploadBtn = new WPushButton( WString::fromUTF8( "Загрузить" ) );
    importCtx.uploadBtn->clicked().connect( boost::bind( &TariffEditor::importUploadRequest, this ) );

    importCtx.cancelBtn = new WPushButton( WString::fromUTF8( "Отмена" ) );
    importCtx.cancelBtn->clicked().connect( &import, &WDialog::reject );

    importCtx.root = new WContainerWidget();

    importCtx.spacer = new WTable();

    importCtx.spacer->elementAt( 0, 0 )->addWidget( importCtx.upload );
    importCtx.spacer->elementAt( 0, 1 )->addWidget( importCtx.uploadBtn );

    importCtx.spacer->elementAt( 1, 0 )->addWidget( importCtx.cancelBtn );

    importCtx.root->addWidget( importCtx.spacer );

    import.contents()->addWidget( importCtx.root );

    import.exec();
}

void TariffEditor::importFileTooLargeError() {
    importCtx.importDlg->setCaption( WString::fromUTF8( "Испорт из CSV: ошибка ( слишком большой файл )" ) );
}

void TariffEditor::importFileDone() {

    if ( importCtx.upload->empty() ) {
        importCtx.importDlg->setCaption( WString::fromUTF8( "Испорт из CSV: ошибка ( выберите файл )" ) );
        return;
    }

    importCtx.fake->addWidget( importCtx.upload );
    importCtx.fake->addWidget( importCtx.cancelBtn );

    importCtx.spacer->deleteRow( 0 );
    importCtx.spacer->deleteRow( 0 );

    importCtx.netcode_helper = new WLabel( WString::fromUTF8( "Номер столбца с MCC/MNC" ) );
    importCtx.price_helper = new WLabel( WString::fromUTF8( "Номер столбца с ценой" ) );
    importCtx.fieldsep_hepler = new WLabel( WString::fromUTF8( "Разделитель полей" ) );
    importCtx.textsep_helper = new WLabel( WString::fromUTF8( "Ограничитель строк" ) );

    importCtx.netcode = new WSpinBox();
    importCtx.netcode->setValue( 1 );
    importCtx.netcode->setMinimum( 1 );
    importCtx.netcode->setSingleStep( 1 );
    importCtx.netcode->setValidator( new WIntValidator( 1, 1000 ) );

    importCtx.price = new WSpinBox();
    importCtx.price->setValue( 1 );
    importCtx.price->setMinimum( 1 );
    importCtx.price->setSingleStep( 1 );
    importCtx.price->setValidator( new WIntValidator( 1, 1000 ) );

    importCtx.fieldsep = new WLineEdit( ";" );
    importCtx.fieldsep->setMaxLength( 1 );

    importCtx.textsep = new WLineEdit( "\"" );
    importCtx.textsep->setMaxLength( 1 );

    WPushButton* nextBtn = new WPushButton( WString::fromUTF8( "Далее" ) );
    nextBtn->clicked().connect( this, &TariffEditor::importParseCsv );

    importCtx.spacer->elementAt( 0, 0 )->addWidget( importCtx.netcode_helper );
    importCtx.spacer->elementAt( 0, 1 )->addWidget( importCtx.netcode );

    importCtx.spacer->elementAt( 1, 0 )->addWidget( importCtx.price_helper );
    importCtx.spacer->elementAt( 1, 1 )->addWidget( importCtx.price );

    importCtx.spacer->elementAt( 2, 0 )->addWidget( importCtx.fieldsep_hepler );
    importCtx.spacer->elementAt( 2, 1 )->addWidget( importCtx.fieldsep );

    importCtx.spacer->elementAt( 3, 0 )->addWidget( importCtx.textsep_helper );
    importCtx.spacer->elementAt( 3, 1 )->addWidget( importCtx.textsep );

    importCtx.spacer->elementAt( 4, 0 )->addWidget( importCtx.cancelBtn );
    importCtx.spacer->elementAt( 4, 1 )->addWidget( nextBtn );
}

void TariffEditor::importUploadRequest() {
    if ( importCtx.upload->canUpload() ) {
        importCtx.upload->upload();
    } else {
        importCtx.importDlg->setCaption( WString::fromUTF8( "Импорт из CSV: ошибка ( невозможно отправить )" ) );
    }
}

void TariffEditor::importParseCsv() {
    ifstream in( importCtx.upload->spoolFileName().c_str() );

    importCtx.fake->addWidget( importCtx.cancelBtn );

    int col_mccmnc = importCtx.netcode->value()-1;
    int col_price = importCtx.price->value()-1;

    importCtx.spacer->deleteRow( 0 );
    importCtx.spacer->deleteRow( 0 );
    importCtx.spacer->deleteRow( 0 );
    importCtx.spacer->deleteRow( 0 );
    importCtx.spacer->deleteRow( 0 );

    char sep = importCtx.fieldsep->text().toUTF8().c_str()[0];
    char quotes = importCtx.textsep->text().toUTF8().c_str()[0];

    WTextArea* output = new WTextArea();
    output->setMinimumSize( WLength( 10, WLength::Centimeter ), WLength( 10, WLength::Centimeter ) );

    WPushButton* finishBtn = new WPushButton( WString::fromUTF8( "Готово" ) );
    finishBtn->clicked().connect( this, &TariffEditor::importCsvFinish );

    importCtx.spacer->elementAt( 0, 0 )->addWidget( output );
    importCtx.spacer->elementAt( 0, 0 )->setColumnSpan( 2 );
    importCtx.spacer->elementAt( 1, 0 )->addWidget( importCtx.cancelBtn );
    importCtx.spacer->elementAt( 1, 1 )->addWidget( finishBtn );

    tariff = Tariff();

    while ( !in.eof() && !in.fail() ) {
        int blocks_found = 0;
        bool parsing_text = false;
        vector< string > row;
        row.push_back("");
        string line;
        getline( in, line );

        for ( int i = 0; i < line.size(); i++ ) {
            char ch = line[i];

            if ( ch == 13 )
                continue;

            if ( ch == 10 )
                continue;

            if ( ( ch == sep ) && !parsing_text ) {
                blocks_found++;
                row.push_back("");
                continue;
            }
            if ( ch == quotes ) {
                parsing_text = !parsing_text;
                continue;
            }
            row[ blocks_found ] += ch;
        }

        if ( row.size() <= std::max( col_mccmnc, col_price ) ) {
            continue;
        }

        if ( row[ col_mccmnc ].size() < 3 ) {
            continue;
        }

        std::string mcc;
        std::string mnc;
        mcc = row[ col_mccmnc ].substr( 0, 3 );
        mnc = row[ col_mccmnc ].substr( 3, row[ col_mccmnc ].length()-3 );

        double price;
        try {
            std::string price_str = row[ col_price ];
            if ( price_str.find( ',' ) != price_str.npos ) {
                price_str[ price_str.find( ',' ) ] = '.';
            }
            price = boost::lexical_cast< double >( price_str );
        } catch ( ... ) {
            continue;
        }

        if ( mnc.empty() ) {
            tariff.setPrice( boost::lexical_cast< string >( mcc ), price );
        } else {
            tariff.setPrice( boost::lexical_cast< string >( mcc ), boost::lexical_cast< string >( mnc ), price );
        }

        output->setText( output->text() + boost::lexical_cast< string >( mcc ) + string("\t") );
        if ( mnc.empty() ) {
            output->setText( output->text() + boost::lexical_cast< string >( mnc ) + string("\t") );
        } else {
            output->setText( output->text() + string("\t") );
        }
        output->setText( output->text() + double2string( price ) + string("\t") );
        output->setText( output->text() + string("\n") );
    }
}

void TariffEditor::importCsvFinish() {
    onTariffUpdate();
    importCtx.importDlg->accept();
}

void TariffEditor::onTariffLoad() {
    std::string name = tlistBox->currentText().toUTF8();
    tariff = TariffManager::get_mutable_instance().loadTariff( name );

    onTariffUpdate();

    nameBox->setText( WString::fromUTF8( name ) );
}

void TariffEditor::onTariffRemove() {
    std::string name = tlistBox->currentText().toUTF8();
    TariffManager::get_mutable_instance().removeTariff( name );

    tlistRebuild();
}

void TariffEditor::onTariffClear() {
    tariff = Tariff();

    buildModel( model_, tariff, false );
}

void TariffEditor::onTariffUpdate() {
    buildModel( model_, tariff, false );
}

void TariffEditor::onTariffSave() {
    std::string name = nameBox->text().toUTF8();
    tariff.setName( name );
    PartnerInfo user = PartnerManager::get_mutable_instance().findById( userid );
    TariffManager::get_mutable_instance().saveTariff( name, user.ownerId.empty()? "": userid, tariff );

    tlistRebuild();
}

void TariffEditor::tlistRebuild() {
    tlistBox->clear();
    PartnerInfo user = PartnerManager::get_mutable_instance().findById( userid );
    std::list< std::string > tlist = TariffManager::get_mutable_instance().tariffs_list(user.ownerId.empty()? "": userid);

    for ( std::list< std::string >::iterator it = tlist.begin(); it != tlist.end(); it++ ) {
        tlistBox->addItem( WString::fromUTF8( *it ) );
    }
}

void TariffEditor::onChangeRoot() {
    Wt::WModelIndexSet selected = treeView_->selectedIndexes();

    if ( selected.empty() ) {
        paidStatuses->setCurPosRoot();
        unknownPolicy->setCurPosRoot();
        currencyEditor->setCurPosRoot();
    }

    if ( selected.size() == 1 ) {
        Wt::WModelIndex index = *selected.begin();

        WStandardItem* root = model_->itemFromIndex( index.parent() );
        WStandardItem* item = root->child( index.row(), 1 );

        std::string mccmnc = item->text().toUTF8();

        std::string mcc = mccmnc.substr( 0, 3 );
        std::string mnc = mccmnc.substr( 3, mccmnc.length()-3 );

        if ( mnc.empty() ) {
            paidStatuses->setCurPosCountry( mcc );
            unknownPolicy->setCurPosCountry( mcc );
            currencyEditor->setCurPosCountry( mcc );
        } else {
            paidStatuses->setCurPosOperator( mcc, mnc );
            unknownPolicy->setCurPosOperator( mcc, mnc );
            currencyEditor->setCurPosOperator( mcc, mnc );
        }
    }
}
