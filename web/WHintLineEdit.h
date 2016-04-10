#ifndef WHINTLINEEDIT_H
#define WHINTLINEEDIT_H

#include <Wt/WLineEdit>

class WHintLineEdit: public Wt::WLineEdit {
public:
    WHintLineEdit( Wt::WContainerWidget *parent=0 ): Wt::WLineEdit( parent ) {}
    WHintLineEdit( const Wt::WString &content, Wt::WContainerWidget *parent=0 ): Wt::WLineEdit( content ) {
        hint = content;

        focussed().connect( SLOT( this, WHintLineEdit::onFocus ) );
        blurred().connect( SLOT( this, WHintLineEdit::onFocusLost ) );
    }
private:
    Wt::WString hint;

    void onFocus() {
        if ( text() == hint ) {
            setText( "" );
        }
    }

    void onFocusLost() {
        if ( text() == "" ) {
            setText( hint );
        }
    }
};

#endif // WHINTLINEEDIT_H
