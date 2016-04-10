#ifndef WHINTLINEPASSEDIT_H
#define WHINTLINEPASSEDIT_H

#include <Wt/WLineEdit>

class WHintLinePassEdit: public Wt::WLineEdit {
public:
    WHintLinePassEdit( Wt::WContainerWidget *parent=0 ): Wt::WLineEdit( parent ) {}
    WHintLinePassEdit( const Wt::WString &content, Wt::WContainerWidget *parent=0 ): Wt::WLineEdit( content ) {
        hint = content;

        focussed().connect( SLOT( this, WHintLinePassEdit::onFocus ) );
        blurred().connect( SLOT( this, WHintLinePassEdit::onFocusLost ) );
    }
private:
    Wt::WString hint;

    void onFocus() {
        if ( text() == hint ) {
            setText( "" );
            setEchoMode( WLineEdit::Password );
        }
    }

    void onFocusLost() {
        if ( text() == "" ) {
            setEchoMode( WLineEdit::Normal );
            setText( hint );
        }
    }
};


#endif // WHINTLINEPASSEDIT_H
