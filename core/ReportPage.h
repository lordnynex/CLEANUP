#ifndef REPORTPAGE_H
#define REPORTPAGE_H

#include "SMPPGateManager.h"
#include "RequestTracker.h"
#include "PGSql.h"

#include <Wt/WApplication>
#include <Wt/WPushButton>
#include <Wt/WContainerWidget>
#include <Wt/WDialog>
#include <Wt/WSelectionBox>
#include <Wt/WText>
#include <Wt/WBreak>
#include <Wt/WTable>
#include <Wt/WLineEdit>
#include <Wt/WCheckBox>
#include <Wt/WIntValidator>
#include <Wt/WDatePicker>
#include <Wt/WDate>


class ReportPage: public Wt::WApplication {
public:
    ReportPage( const Wt::WEnvironment& env );
    ~ReportPage();

    struct msg_stats {
        long total;
        long delivered;
        long undelivered;
    };

private:
    Wt::WPushButton* nPtnrRprtBtn;
    Wt::WTable* wtbl;

    msg_stats getMsgStats( string idp, Wt::WDate _from, Wt::WDate _to, std::set<string> gateways );
    void buildReport( string idp, Wt::WDate from, Wt::WDate to, std::set<string> gateways, bool adv = false );
    void onPtnrRprtBtnClicked();
};

Wt::WApplication *createReportPage(const Wt::WEnvironment& env);

#endif // REPORTPAGE_H
