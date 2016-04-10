#include "SendMessageForm.h"

#include <Wt/WGridLayout>
using namespace Wt;

SendMessageForm::SendMessageForm( Wt::WContainerWidget* parent ): WContainerWidget( parent ) {
    sendBtn = new WPushButton( WString::fromUTF8( "Отправить" ) );
    sendBtn->setMinimumSize( WLength::Auto, WLength( 0.6, WLength::Centimeter ) );
    cancelBtn = new WPushButton( WString::fromUTF8( "Отмена" ) );
    cancelBtn->setMinimumSize( WLength::Auto, WLength( 0.6, WLength::Centimeter ) );
    pidEdit = new WLineEdit(); pidEdit->setEmptyText( WString::fromUTF8( "IDP" ) );
    fromEdit = new WLineEdit(); fromEdit->setEmptyText( WString::fromUTF8( "Отправитель" ) );
    toEdit = new WLineEdit(); toEdit->setEmptyText( WString::fromUTF8( "Получатель" ) );
    textEdit = new WTextArea();
    gateway = new WComboBox(); gateway->addItem( WString::fromUTF8( "По умолчанию" ) );
    gateway->setMinimumSize( WLength::Auto, WLength( 0.6, WLength::Centimeter ) );

    WGridLayout* root = new WGridLayout();
    root->addWidget( toEdit, 0, 0, 1, 3 );
    root->addWidget( gateway, 0, 3 );
    root->addWidget( textEdit, 1, 0, 3, 4 );
    root->addWidget( pidEdit, 4, 0 );
    root->addWidget( fromEdit, 4, 1 );
    root->addWidget( cancelBtn, 4, 2 );
    root->addWidget( sendBtn, 4, 3 );

    setLayout( root );
}
