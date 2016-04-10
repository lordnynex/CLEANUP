#ifndef SENDMESSAGEFORM_H
#define SENDMESSAGEFORM_H

#include <Wt/WContainerWidget>
#include <Wt/WPushButton>
#include <Wt/WLineEdit>
#include <Wt/WTextArea>
#include <Wt/WComboBox>

class SendMessageForm : public Wt::WContainerWidget {
public:
    SendMessageForm( Wt::WContainerWidget* parent= 0);

    Wt::WPushButton* sendBtn;
    Wt::WPushButton* cancelBtn;

    Wt::WLineEdit* pidEdit;
    Wt::WTextArea* textEdit;
    Wt::WLineEdit* fromEdit;
    Wt::WLineEdit* toEdit;

    Wt::WComboBox* gateway;
};

#endif // SENDMESSAGEFORM_H
