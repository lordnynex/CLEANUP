/*
 * File:   PersonalCabinet.h
 * Author: mohtep
 *
 * Created on 1 Март 2010 г., 0:57
 */

#ifndef _PERSONALCABINET_H
#define	_PERSONALCABINET_H

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
#include "AdminPage.h"

using namespace Wt;

class PersonalCabinet: public WApplication {
public:
    PersonalCabinet( const WEnvironment& env );
    virtual ~PersonalCabinet();
private:
    typedef std::vector< std::pair< sms::SMSMessage::ID, std::string > > MsgidList;
    typedef std::vector< std::vector<std::string> > ReqResp;

    MsgidList genMsgIds( const std::string& _idp, const std::string& phone, const std::string& _ldate, const std::string& _rdate, const std::string& _text, int page );
    ReqResp genReq( const MsgidList& list, int status );
    void onReportGenerate();
    //void onReportGenerate();

    WTable* tbl;
    WPushButton* reportbtn;
    WLabel* reportstatus;

    WLineEdit* pid;
    WLineEdit* phone;
    WDatePicker* date_from, *date_to;
    WLineEdit* text;
    WComboBox* status;

    static PGSql& db;

    friend class WInfoText;
};

WApplication *createPersonalCabinet(const WEnvironment& env);

#endif	/* _PERSONALCABINET_H */

