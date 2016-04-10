/* 
 * File:   AdminPage.h
 * Author: mohtep
 *
 * Created on 1 Март 2010 г., 0:57
 */

#ifndef _ADMINPAGE_H
#define	_ADMINPAGE_H

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WTable>
#include <Wt/WLabel>
#include <Wt/WOverlayLoadingIndicator>
#include <Wt/WComboBox>
#include <Wt/WText>
#include <Wt/WPushButton>
#include <Wt/WLength>
#include <Wt/WDatePicker>
#include <Wt/WCalendar>

#include "RequestTracker.h"
#include "PGSql.h"

using namespace Wt;

class AdminPage: public WApplication {
public:
    AdminPage( const WEnvironment& env );
    virtual ~AdminPage();
private:
    typedef std::vector< std::pair< sms::SMSMessage::ID, std::string > > MsgidList;
    typedef std::vector< std::vector<std::string> > ReqResp;
    
    MsgidList genMsgIds( const std::string& _idp, const std::string& phone, const std::string& _date, const std::string& _text, int page );
    ReqResp genReq( const MsgidList& list, int status );
    void onReportGenerate();
    //void onReportGenerate();

    WTable* tbl;
    WPushButton* reportbtn;
    WLabel* reportstatus;

    WLineEdit* pid;
    WLineEdit* phone;
    WDatePicker* date;
    WLineEdit* text;
    WComboBox* status;

    static PGSql& db;

    friend class WInfoText;
};

WApplication *createAdminPage(const WEnvironment& env);

class WInfoText: public WText {
public:
    WInfoText(WContainerWidget *parent = 0): WText( parent ) {
        restore = false;
    }
    WInfoText(const WString& text, WContainerWidget *parent = 0): WText( text, parent ) { 
        restore = false;
    }
    void setApp( AdminPage* p ) {
        app = p;
    }

    void setMsg( const std::string& id ) {
        msgid = id;
    }

    void setPos( const int& h, const int& w ) {
        row = h;
        column = w;
    }

    void setTextFull( const std::string& t ) {
        textfull = t;
    }

    void onHistoryDetail();
    void onTextFull();

private:
    bool restore;
    std::list< WWidget* > added;
    AdminPage* app;
    std::string msgid;
    int row;
    int column;
    std::string textfull;
};


#endif	/* _ADMINPAGE_H */

