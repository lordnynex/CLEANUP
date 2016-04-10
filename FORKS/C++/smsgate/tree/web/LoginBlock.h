#ifndef LOGINBLOCK_H
#define LOGINBLOCK_H

#include <string>

#include <Wt/WContainerWidget>
#include <Wt/WLabel>
#include <Wt/WPushButton>
#include <Wt/WImage>

#include <Wt/WLineEdit>

class LoginBlock: public Wt::WContainerWidget {
public:
    LoginBlock( Wt::WContainerWidget* parent = 0 );
    Wt::Signal<std::string, bool>& onLogin() { return onLogin_; };
private:
    Wt::WLabel* failedMsgLabel;
    Wt::WLineEdit* loginBox;
    Wt::WLineEdit* passBox;
    Wt::Signal<std::string, bool> onLogin_;

    bool authorized;
    bool isAdmin;

    void onLoginEvent();
};

#endif // LOGINBLOCK_H
