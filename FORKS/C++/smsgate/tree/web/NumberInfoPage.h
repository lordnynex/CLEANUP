#ifndef NUMBERINFOPAGE_H
#define NUMBERINFOPAGE_H

#include <Wt/WApplication>
#include <Wt/WTextArea>
#include <Wt/WPushButton>
#include <Wt/WLineEdit>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>

using namespace Wt;

class NumberInfoPage : public WApplication {
    Wt::WTable* tbl;

    bool popup_shown;
    Wt::WLineEdit* phoneInput;
    Wt::WPushButton* phoneBtn;
    Wt::WTextArea* phoneResult;
public:
    NumberInfoPage( const WEnvironment& env );
    virtual ~NumberInfoPage();

    void onPhoneEditFocus();
    void onPhoneEditFocusLost();
    void onPhonePrintInfo();
};

WApplication *createNumberInfoPage(const WEnvironment& env);

#endif // NUMBERINFOPAGE_H
