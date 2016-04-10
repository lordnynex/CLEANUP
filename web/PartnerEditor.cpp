#include "PartnerEditor.h"
#include "PartnerManager.h"
#include "Tariff.h"

#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WStandardItem>
#include <Wt/WRegExpValidator>
#include <Wt/WIntValidator>
#include <Wt/WSuggestionPopup>
#include <vector>
#include <iostream>

using namespace Wt;

PartnerOptions::PartnerOptions( std::string _userid, std::string _pid, Wt::WContainerWidget *parent ): WContainerWidget( parent ), updated_( this ) {
    userid = _userid;
    pid = _pid;
    PartnerInfo pi, user;

    user = PartnerManager::get_mutable_instance().findById( userid );
    try {
        pi = PartnerManager::get_mutable_instance().findById( pid );        
    } catch ( ... ) {}
    WString uv = WString::fromUTF8( "Значение не задано" );
    WString up = WString::fromUTF8( "Значение скрыто" );

    int cl = 0;

    WPushButton* saveBtn;
    if ( pid.empty() )
        saveBtn = new WPushButton( WString::fromUTF8( "Создать" ) );
    else
        saveBtn = new WPushButton( WString::fromUTF8( "Сохранить" ) );
    saveBtn->clicked().connect( this, &PartnerOptions::onBtnSave );

    tbl = new WTable();
    tbl->setStyleClass( "restable" );
    tbl->setHeaderCount( 1 );

    pCNameEdit = new WCustomInPlaceEdit( WString::fromUTF8( pi.pCName ), uv );
    tbl->elementAt( cl, 0 )->addWidget( pCNameEdit );
    tbl->elementAt( cl, 1 )->setContentAlignment( AlignRight | AlignMiddle );
    tbl->elementAt( cl++, 1 )->addWidget( saveBtn );

    pLastNameEdit = new WCustomInPlaceEdit( WString::fromUTF8( pi.pLastName ), uv );
    tbl->elementAt( cl, 0 )->addWidget( new WLabel( WString::fromUTF8( "Фамилия" ) ) );
    tbl->elementAt( cl++, 1 )->addWidget( pLastNameEdit );

    pFirstNameEdit = new WCustomInPlaceEdit( WString::fromUTF8( pi.pFirstName ), uv );
    tbl->elementAt( cl, 0 )->addWidget( new WLabel( WString::fromUTF8( "Имя" ) ) );
    tbl->elementAt( cl++, 1 )->addWidget( pFirstNameEdit );

    pMiddleNameEdit = new WCustomInPlaceEdit( WString::fromUTF8( pi.pMiddleName ), uv );
    tbl->elementAt( cl, 0 )->addWidget( new WLabel( WString::fromUTF8( "Отчество" ) ) );
    tbl->elementAt( cl++, 1 )->addWidget( pMiddleNameEdit );

    pEmailEdit = new WCustomInPlaceEdit( WString::fromUTF8( pi.pEmail ), uv );
    std::string email_match = "^[A-Z0-9._%-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}$";
    WRegExpValidator* pEmailValidator = new WRegExpValidator( email_match );
    pEmailValidator->setFlags( MatchCaseInsensitive );
    pEmailValidator->setMandatory( true );
    pEmailEdit->lineEdit()->setValidator( pEmailValidator );
    tbl->elementAt( cl, 0 )->addWidget( new WLabel( WString::fromUTF8( "Электронная почта" ) ) );
    tbl->elementAt( cl++, 1 )->addWidget( pEmailEdit );

    pOwnerPhoneEdit = new WCustomInPlaceEdit( WString::fromUTF8( pi.phone ), uv );
    std::string phone_match = "^[1-9]{1}[0-9]{7,14}$";
    WRegExpValidator* pPhoneValidator = new WRegExpValidator( phone_match );
    pPhoneValidator->setFlags( MatchCaseInsensitive );
    pPhoneValidator->setMandatory( true );
    pOwnerPhoneEdit->lineEdit()->setValidator( pPhoneValidator );
    tbl->elementAt( cl, 0 )->addWidget( new WLabel( WString::fromUTF8( "Контрольный телефон" ) ) );
    tbl->elementAt( cl++, 1 )->addWidget( pOwnerPhoneEdit );

    pPhoneEdit = new WCustomInPlaceEdit( WString::fromUTF8( pi.pContact ), uv );
    pPhoneEdit->lineEdit()->setValidator( pPhoneValidator );
    tbl->elementAt( cl, 0 )->addWidget( new WLabel( WString::fromUTF8( "Контактный телефон" ) ) );
    tbl->elementAt( cl++, 1 )->addWidget( pPhoneEdit );

    pCompanyEdit = new WCustomInPlaceEdit( WString::fromUTF8( pi.pCompanyName ), uv );
    tbl->elementAt( cl, 0 )->addWidget( new WLabel( WString::fromUTF8( "Название компании" ) ) );
    tbl->elementAt( cl++, 1 )->addWidget( pCompanyEdit );

    pCAddressEdit = new WCustomInPlaceEdit( WString::fromUTF8( pi.pCompanyAddress ), uv );
    tbl->elementAt( cl, 0 )->addWidget( new WLabel( WString::fromUTF8( "Адрес сайта компании" ) ) );
    tbl->elementAt( cl++, 1 )->addWidget( pCAddressEdit );

    pTimeZoneEdit = new WCustomInPlaceEdit( WString::fromUTF8( boost::lexical_cast< std::string >( pi.tzone ) ), uv );
    WIntValidator* pTimeZoneValidator = new WIntValidator( -12, 12 );
    pTimeZoneEdit->lineEdit()->setValidator( pTimeZoneValidator );
    tbl->elementAt( cl, 0 )->addWidget( new WLabel( WString::fromUTF8( "Часовой пояс ( UTC )" ) ) );
    tbl->elementAt( cl++, 1 )->addWidget( pTimeZoneEdit );

    pExpand = new WText( WString::fromUTF8( "⇑Скрыть личную инфомацию⇑" ) );
    pExpand->setStyleClass( "link" );
    pExpand->clicked().connect( this, &PartnerOptions::onPersonalShowHide );
    tbl->elementAt( cl, 0 )->setColumnSpan( 2 );
    tbl->elementAt( cl, 0 )->addWidget( pExpand );
    tbl->elementAt( cl, 0 )->setContentAlignment( AlignCenter | AlignMiddle );
    cl++;

    pLoginEdit = new WCustomInPlaceEdit( WString::fromUTF8( pi.pName ), uv );
    tbl->elementAt( cl, 0 )->addWidget( new WLabel( WString::fromUTF8( "Логин" ) ) );
    tbl->elementAt( cl++, 1 )->addWidget( pLoginEdit );

    pPassEdit = new WCustomInPlaceEdit( WString::fromUTF8( "" ), up );
    tbl->elementAt( cl, 0 )->addWidget( new WLabel( WString::fromUTF8( "Пароль" ) ) );
    tbl->elementAt( cl++, 1 )->addWidget( pPassEdit );

    pAdminPassEdit = new WCustomInPlaceEdit( WString::fromUTF8( "" ), up );
    tbl->elementAt( cl, 0 )->addWidget( new WLabel( WString::fromUTF8( "Админский пароль" ) ) );
    tbl->elementAt( cl++, 1 )->addWidget( pAdminPassEdit );

    {
        Wt::WSuggestionPopup::Options suggestOptions
        = { "<b>",         // highlightBeginTag
            "</b>",        // highlightEndTag
            ',',           // listSeparator      (for multiple addresses)
            " \\n",        // whitespace
            " ",           // wordSeparators     (within an address)
            ""             // appendReplacedText (prepare next email address)
           };

        pManagerEdit = new WCustomInPlaceEdit( WString::fromUTF8( pi.pManager ), uv );
        WSuggestionPopup* pManagerSuggest = new WSuggestionPopup( suggestOptions, this );
        std::list< PartnerInfo > lst = PartnerManager::get_mutable_instance().getAll( user.ownerId.empty()? "": user.pId );
        pManagerSuggest->forEdit( pManagerEdit->lineEdit(), WSuggestionPopup::Editing | WSuggestionPopup::DropDownIcon );
        pManagerSuggest->activated().connect( boost::bind( &PartnerOptions::onSuggestionActivated, this, pManagerSuggest, _1, pManagerEdit ) );
        std::set< std::string > lst_unique;
        for ( std::list< PartnerInfo >::iterator it = lst.begin(); it != lst.end(); it++ ) {
            lst_unique.insert( it->pManager );
        }
        for ( std::set< std::string >::iterator it = lst_unique.begin(); it != lst_unique.end(); it++ ) {
            pManagerSuggest->addSuggestion( WString::fromUTF8( *it ), WString::fromUTF8( *it ) );
        }
        tbl->elementAt( cl, 0 )->addWidget( new WLabel( WString::fromUTF8( "Менеджер" ) ) );
        tbl->elementAt( cl, 1 )->addWidget( pManagerSuggest );
        tbl->elementAt( cl++, 1 )->addWidget( pManagerEdit );
    }

    {
        Wt::WSuggestionPopup::Options suggestOptions
        = { "<b>",         // highlightBeginTag
            "</b>",        // highlightEndTag
            ',',           // listSeparator      (for multiple addresses)
            " \\n",        // whitespace
            " ",           // wordSeparators     (within an address)
            ""             // appendReplacedText (prepare next email address)
           };

        pTariffEdit = new WCustomInPlaceEdit( WString::fromUTF8( pi.tariff.getName() ), uv );
        TariffManager::TariffListT tariffs = TariffManager::get_mutable_instance().tariffs_list(user.ownerId.empty()? "": userid);
        WSuggestionPopup* pTariffSuggest = new WSuggestionPopup( suggestOptions, this );
        pTariffSuggest->forEdit( pTariffEdit->lineEdit(), WSuggestionPopup::Editing | WSuggestionPopup::DropDownIcon );
        pTariffSuggest->activated().connect( boost::bind( &PartnerOptions::onSuggestionActivated, this, pTariffSuggest, _1, pTariffEdit ) );
        std::set< std::string > tariffs_unique( tariffs.begin(), tariffs.end() );
        for ( std::set< std::string >::iterator it = tariffs_unique.begin(); it != tariffs_unique.end(); it++ ) {
            pTariffSuggest->addSuggestion( WString::fromUTF8( *it ), WString::fromUTF8( *it ) );
        }
        tbl->elementAt( cl, 0 )->addWidget( new WLabel( WString::fromUTF8( "Тариф" ) ) );
        tbl->elementAt( cl, 1 )->addWidget( pTariffSuggest );
        tbl->elementAt( cl++, 1 )->addWidget( pTariffEdit );
    }

    pTrialEdit = new WComboBox();
    pTrialEdit->insertItem( 0, WString::fromUTF8( "Нет" ) );
    pTrialEdit->insertItem( 1, WString::fromUTF8( "Да" ) );
    pi.pIsTrial ? pTrialEdit->setCurrentIndex( 1 ): pTrialEdit->setCurrentIndex( 0 );
    tbl->elementAt( cl, 0 )->addWidget( new WLabel( WString::fromUTF8( "Тестовый аккаунт" ) ) );
    tbl->elementAt( cl, 0 )->setContentAlignment( AlignMiddle );
    tbl->elementAt( cl, 1 )->setContentAlignment( AlignCenter );
    tbl->elementAt( cl++, 1 )->addWidget( pTrialEdit );


    pPriorityEdit = new WSpinBox();
    pPriorityEdit->setSingleStep( 1.0 );
    pPriorityEdit->setMinimum( 0 );
    pPriorityEdit->setMaximum( 99 );
    tbl->elementAt( cl, 0 )->addWidget( new WLabel( WString::fromUTF8( "Приоритет" ) ) );
    tbl->elementAt( cl, 0 )->setContentAlignment( AlignMiddle );
    tbl->elementAt( cl, 1 )->setContentAlignment( AlignCenter );
    tbl->elementAt( cl++, 1 )->addWidget( pPriorityEdit );

    pPostEdit = new WComboBox();
    pPostEdit->insertItem( 0, WString::fromUTF8( "Нет" ) );
    pPostEdit->insertItem( 1, WString::fromUTF8( "Да" ) );
    pi.pPostPay ? pPostEdit->setCurrentIndex( 1 ): pPostEdit->setCurrentIndex( 0 );
    tbl->elementAt( cl, 0 )->addWidget( new WLabel( WString::fromUTF8( "Постоплата" ) ) );
    tbl->elementAt( cl, 0 )->setContentAlignment( AlignMiddle );
    tbl->elementAt( cl, 1 )->setContentAlignment( AlignCenter );
    tbl->elementAt( cl++, 1 )->addWidget( pPostEdit );

    pBalanceEdit = new WSpinBox();
    pBalanceEdit->setSingleStep( 0.01 );
    pPriorityEdit->setMinimum( -1e9 );
    pPriorityEdit->setMaximum( 1e9 );
    pBalanceEdit->setValue( pi.pBalance );
    tbl->elementAt( cl, 0 )->addWidget( new WLabel( WString::fromUTF8( "Баланс" ) ) );
    tbl->elementAt( cl, 0 )->setContentAlignment( AlignMiddle );
    tbl->elementAt( cl, 1 )->setContentAlignment( AlignCenter );
    tbl->elementAt( cl++, 1 )->addWidget( pBalanceEdit );

    pCreditEdit = new WSpinBox();
    pCreditEdit->setSingleStep( 0.01 );
    pCreditEdit->setMinimum( 0 );
    pCreditEdit->setMaximum( 1e9 );
    pCreditEdit->setValue( pi.pCredit );
    tbl->elementAt( cl, 0 )->addWidget( new WLabel( WString::fromUTF8( "Кредитный лимит" ) ) );
    tbl->elementAt( cl, 0 )->setContentAlignment( AlignMiddle );
    tbl->elementAt( cl, 1 )->setContentAlignment( AlignCenter );
    tbl->elementAt( cl++, 1 )->addWidget( pCreditEdit );

    pLimitEdit = new WSpinBox();
    pLimitEdit->setSingleStep( 0.1 );
    pLimitEdit->setMinimum( 0 );
    pLimitEdit->setMaximum( 1e5 );
    pLimitEdit->setValue( pi.pLimit );
    tbl->elementAt( cl, 0 )->addWidget( new WLabel( WString::fromUTF8( "Пропускная способность" ) ) );
    tbl->elementAt( cl, 0 )->setContentAlignment( AlignMiddle );
    tbl->elementAt( cl, 1 )->setContentAlignment( AlignCenter );
    tbl->elementAt( cl++, 1 )->addWidget( pLimitEdit );

    if ( pid.empty() ) {
        isPersonalInfoVisible = true;
    } else
        isPersonalInfoVisible = false;
    onPersonalShowHide();

    WVBoxLayout* lyt = new WVBoxLayout();
    lyt->addWidget( tbl, AlignCenter | AlignMiddle );

    setLayout( lyt, AlignCenter | AlignMiddle );
}


void PartnerOptions::onSuggestionActivated( WSuggestionPopup* sugg, int index, WCustomInPlaceEdit* widget ) {
    WString data = boost::any_cast< WString >( sugg->model()->data( index, 1 ) );
    widget->setText( data );
}

void PartnerOptions::onPersonalShowHide() {
    if ( isPersonalInfoVisible ) {
        pExpand->setText( WString::fromUTF8( "⇑Скрыть личную инфомацию⇑" ) );
        for ( int i = 1; i < 10; i++ ) {
            tbl->rowAt( i )->show();
        }
    } else {
        pExpand->setText( WString::fromUTF8( "⇓Показать личную инфомацию⇓" ) );
        for ( int i = 1; i < 10; i++ ) {
            tbl->rowAt( i )->hide();
        }
    }
    isPersonalInfoVisible = !isPersonalInfoVisible;
}

void PartnerOptions::onBtnSave() {
    if ( pLoginEdit->text().toUTF8().empty() )
        return;

    bool isNew = true;
    PartnerInfo pi, user;
    user = PartnerManager::get_mutable_instance().findById( userid );
    try {
            pi = PartnerManager::get_mutable_instance().findById( pid );
            isNew = false;
    } catch ( ... ) {}

    pi.pCName = pCNameEdit->text().toUTF8();
    pi.pLastName = pLastNameEdit->text().toUTF8();
    pi.pFirstName = pFirstNameEdit->text().toUTF8();
    pi.pMiddleName = pMiddleNameEdit->text().toUTF8();
    pi.pEmail = pEmailEdit->text().toUTF8();
    pi.phone = pOwnerPhoneEdit->text().toUTF8();
    pi.pContact = pPhoneEdit->text().toUTF8();
    pi.pCompanyName = pCompanyEdit->text().toUTF8();
    pi.pCompanyAddress = pCAddressEdit->text().toUTF8();
    pi.ownerId = user.ownerId.empty()? "system": user.pId;

    try {
        pi.tzone = boost::lexical_cast< int >( pTimeZoneEdit->text().toUTF8() );
    } catch ( ... ) {
        pi.tzone = 4;
    }

    pi.pName = pLoginEdit->text().toUTF8();
    if ( !pPassEdit->text().toUTF8().empty() )
        pi.pPass = pPassEdit->text().toUTF8();
    if ( !pAdminPassEdit->text().toUTF8().empty() )
        pi.pAdminPass = pAdminPassEdit->text().toUTF8();
    pi.pManager = pManagerEdit->text().toUTF8();
    pi.tariff = TariffManager::get_mutable_instance().loadTariff( pTariffEdit->text().toUTF8() );
    pi.pIsTrial = ( pTrialEdit->currentIndex() == 1 );
    pi.pPriority = pPriorityEdit->value();
    pi.pPostPay = ( pPostEdit->currentIndex() == 1 );
    pi.pBalance = pBalanceEdit->value();
    pi.pCredit = pCreditEdit->value();
    pi.pLimit = pLimitEdit->value();

    PartnerManager::get_mutable_instance().updateToDb( pi );
    updated_.emit( pid, isNew );
}

PartnerEditor::PartnerEditor( std::string _userid, Wt::WContainerWidget* parent ):WContainerWidget( parent ) {
    userid = _userid;
    root = new WBorderLayout();
    opts = new WHBoxLayout();
    WPushButton* refresh= new WPushButton( WString::fromUTF8("Обновить") );
    refresh->clicked().connect( boost::bind( &PartnerEditor::onPartnersUpdated, this, "", false ) );
    refresh->setMaximumSize( WLength::Auto, WLength( 1, WLength::Centimeter ) );

    columns_width.push_back(150);
    columns_width.push_back(70);
    elements_per_page = 25;

    model_ = new WStandardItemModel();
    buildModel( model_ );
    treeView_ = buildTreeView( model_ );
    resizeTreeView( treeView_ );

    PartnerOptions* popt = new PartnerOptions( userid, "" );
    popt->updated().connect( this, &PartnerEditor::onPartnersUpdated );
    opts->addWidget( popt );

    root->addWidget( treeView_, WBorderLayout::West );
    root->addWidget( refresh, WBorderLayout::East );
    root->add( opts, WBorderLayout::Center );
    setLayout( root );
}

WTreeView* PartnerEditor::buildTreeView( Wt::WStandardItemModel * model ) {

    WTreeView* tw = new WTreeView();
    tw->setModel( model );
    tw->setSelectionMode( Wt::ExtendedSelection );
    tw->setAlternatingRowColors( true );
    tw->clicked().connect( this, &PartnerEditor::onChangeRoot );

    tw->sortByColumn( 1, AscendingOrder );

    return tw;
}

void PartnerEditor::resizeTreeView( WTreeView* tw) {
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

void PartnerEditor::buildModel( WStandardItemModel* data ) {
    PartnerInfo user = PartnerManager::get_mutable_instance().findById( userid );
    data->clear();
    data->insertColumns(0, columns_width.size());

    data->setHeaderData(0, Horizontal, WString::fromUTF8("Партнер"));
    data->setHeaderData(1, Horizontal, WString::fromUTF8("PID"));

    std::list< PartnerInfo > lst = PartnerManager::get_mutable_instance().getAll( user.ownerId.empty()? "": user.pId );
    for ( std::list< PartnerInfo >::iterator it = lst.begin(); it != lst.end(); it++ ) {
        WStandardItem *pName,*pId;
        try {
            PartnerInfo owner = PartnerManager::get_mutable_instance().findById( it->ownerId );
            pName = new WStandardItem( WString::fromUTF8( owner.pName+string(".")+it->pName ) );
        } catch ( ... ) {
            pName = new WStandardItem( WString::fromUTF8( it->pName ) );
        }

        pId = new WStandardItem( WString::fromUTF8( it->pId ) );

        std::vector< WStandardItem* > row;
        row.push_back( pName );
        row.push_back( pId );

        data->appendRow( row );
    }
}

void PartnerEditor::updateModel( WStandardItemModel* data ) {
    PartnerInfo user = PartnerManager::get_mutable_instance().findById( userid );
    std::set< std::string > partners;
    for ( int row = 0; row < data->rowCount(); row++ ) {
        WStandardItem* login_item = data->item( row, 0 );
        WStandardItem* pid_item = data->item( row, 1 );
        partners.insert( pid_item->text().toUTF8() );

        try {
            PartnerInfo pi = PartnerManager::get_mutable_instance().findById( pid_item->text().toUTF8() );
            login_item->setText( pi.pName );
        } catch ( ... ) {}
    }

    std::list< PartnerInfo > lst = PartnerManager::get_mutable_instance().getAll( user.ownerId.empty()? "": user.pId );
    for ( std::list< PartnerInfo >::iterator it = lst.begin(); it != lst.end(); it++ ) {
        if ( partners.find( it->pId ) == partners.end() ) {
            WStandardItem *pName,*pId;
            try {
                PartnerInfo owner = PartnerManager::get_mutable_instance().findById( it->ownerId );
                pName = new WStandardItem( WString::fromUTF8( owner.pName+string(".")+it->pName ) );
            } catch ( ... ) {
                pName = new WStandardItem( WString::fromUTF8( it->pName ) );
            }
            pId = new WStandardItem( WString::fromUTF8( it->pId ) );

            std::vector< WStandardItem* > row;
            row.push_back( pName );
            row.push_back( pId );

            data->appendRow( row );
        }
    }

}

void PartnerEditor::onChangeRoot() {
    Wt::WModelIndexSet selected = treeView_->selectedIndexes();

    while ( opts->count() ) {
        opts->removeItem( opts->itemAt( 0 ) );
    }

    if ( selected.empty() ) {
        PartnerOptions* popt = new PartnerOptions( userid, "" );
        popt->updated().connect( this, &PartnerEditor::onPartnersUpdated );
        opts->addWidget( popt );
    }

    for ( Wt::WModelIndexSet::iterator it = selected.begin(); it != selected.end(); it++ ) {
        Wt::WModelIndex index = *it;

        WStandardItem* iroot = model_->itemFromIndex( index.parent() );
        WStandardItem* item = iroot->child( index.row(), 1 );

        std::string pId = item->text().toUTF8();

        PartnerOptions* popt = new PartnerOptions( userid, pId );
        popt->updated().connect( this, &PartnerEditor::onPartnersUpdated );
        opts->addWidget( popt );
    }

}

void PartnerEditor::onPartnersUpdated( std::string pid, bool isNew ) {
    updateModel( model_ );
    treeView_->sortByColumn( treeView_->sortColumn() , treeView_->sortOrder() );
    onChangeRoot();
}
