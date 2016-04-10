#include "NumberInfoPage.h"
#include "MessageClassifier.h"
#include "StatManager.h"
#include "SMPPGateManager.h"

#include <boost/lexical_cast.hpp>

WApplication *createNumberInfoPage(const WEnvironment& env) {
    return new NumberInfoPage( env );
}

NumberInfoPage::NumberInfoPage( const WEnvironment& env ):WApplication( env ) {
    setTitle( "SMSGate countries-gates stats page" );

    this->setCssTheme("polished");

    phoneInput = new Wt::WLineEdit( WString::fromUTF8("Номер абонента") );
    phoneInput->setMinimumSize( WLength( 6, WLength::Centimeter ), WLength::Auto );
    phoneInput->focussed().connect( SLOT( this, NumberInfoPage::onPhoneEditFocus ) );
    phoneInput->blurred().connect( SLOT( this, NumberInfoPage::onPhoneEditFocusLost ) );
    phoneInput->keyWentDown().connect( SLOT( this, NumberInfoPage::onPhonePrintInfo ) );

    phoneBtn = new Wt::WPushButton( WString::fromUTF8("ОК") );
    phoneBtn->setMinimumSize( WLength( 3, WLength::Centimeter ), WLength::Auto );
    phoneBtn->setMaximumSize( WLength( 3, WLength::Centimeter ), WLength::Auto );
    phoneBtn->clicked().connect( SLOT( this, NumberInfoPage::onPhonePrintInfo ) );

    phoneResult = new Wt::WTextArea();
    phoneResult->setMinimumSize( Wt::WLength( 14, WLength::Centimeter ), Wt::WLength( 8, WLength::Centimeter ) );
    phoneResult->setReadOnly( true );


    WVBoxLayout* bv = new Wt::WVBoxLayout();
    WHBoxLayout* bh = new Wt::WHBoxLayout();

    WContainerWidget* fl = new Wt::WContainerWidget();
    WContainerWidget* sl = new Wt::WContainerWidget();

    bh->addWidget( phoneInput );
    bh->addWidget( phoneBtn );
    fl->setLayout( bh, AlignTop | AlignJustify );
    bv->addWidget( fl );
    bv->addWidget( phoneResult );
    sl->setLayout( bv, AlignTop  );

    root()->addWidget( sl );
}

void NumberInfoPage::onPhoneEditFocus() {
    if ( phoneInput->text() == WString::fromUTF8("Номер абонента") ) {
        phoneInput->setText( "" );
    }
}
void NumberInfoPage::onPhoneEditFocusLost() {
    if ( phoneInput->text() == "" ) {
        phoneInput->setText( WString::fromUTF8("Номер абонента") );
    }
}

void NumberInfoPage::onPhonePrintInfo() {
    if ( phoneInput->text() == "" ) return;
    if ( phoneInput->text() == WString::fromUTF8("Номер абонента") ) return;

    std::ostringstream out;
    out.precision(2);
    sms::MessageClassifier::CountryInfo msg = sms::MessageClassifier::get_mutable_instance().getMsgClass( phoneInput->text().toUTF8() );

    out << "Код страны: " <<  msg.cCode << std::endl;
    out << "Страна: " <<  msg.cName << std::endl;

    if ( !msg.operators.empty() ) {
        out << "Оператор: " <<  msg.operators.begin()->second.getName() << std::endl;
        out << "Код оператора: " <<  msg.operators.begin()->second.getCode() << std::endl;
        out << "Регион: " << msg.operators.begin()->second.opRegion << std::endl << std::endl;
    }

    phoneResult->setText( WString::fromUTF8(out.str()) );
}

NumberInfoPage::~NumberInfoPage() {

}
