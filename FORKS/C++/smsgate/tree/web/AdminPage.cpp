/*
 * File:   AdminPage.cpp
 * Author: mohtep
 *
 * Created on 1 Март 2010 г., 0:57
 */

#include <Wt/WLineEdit>

#include "AdminPage.h"
#include "Logger.h"

PGSql& AdminPage::db( PGSqlConnPoolStats::get_mutable_instance().getdb() );

WApplication *createAdminPage(const WEnvironment& env) {
  /*
   * You could read information from the environment to decide whether
   * the user has permission to start a new application
   */
  return new AdminPage(env);
}


AdminPage::AdminPage( const WEnvironment& env ): WApplication( env ) {
    setTitle( WString::fromUTF8("SMSGate admin page") );

    this->setCssTheme("polished");
    this->useStyleSheet( "/resources/css/resp_table.css" );

    tbl = new WTable();
    tbl->setHeaderCount(2);
    reportbtn = new WPushButton( WString::fromUTF8("Сгенерировать отчет") );
    reportbtn->clicked().connect(SLOT(this, AdminPage::onReportGenerate));

    reportstatus = new WLabel();

    pid = new WLineEdit(); pid->setMaximumSize(  WLength( 1, WLength::Centimeter ), WLength::Auto  );
    phone = new WLineEdit();
    date = new WDatePicker();
    text = new WLineEdit();
    text->setMinimumSize( WLength( 6, WLength::Centimeter ), WLength::Auto );
    status = new WComboBox();
    status->addItem(WString::fromUTF8("Любой"));
    status->addItem(WString::fromUTF8("Доставлено"));
    status->addItem(WString::fromUTF8("Не доставлено"));

    tbl->elementAt( 0, 0 )->addWidget( pid );
    tbl->elementAt( 0, 1 )->addWidget( phone );
    tbl->elementAt( 0, 2 )->addWidget( date );
    tbl->elementAt( 0, 3 )->addWidget( text );
    tbl->elementAt( 0, 4 )->addWidget( status );
    tbl->elementAt( 0, 5 )->addWidget( reportbtn );

    tbl->elementAt( 1, 0 )->addWidget( new WLabel(WString::fromUTF8("IDP")) );
    tbl->elementAt( 1, 1 )->addWidget( new WLabel(WString::fromUTF8("Телефон")) );
    tbl->elementAt( 1, 2 )->addWidget( new WLabel(WString::fromUTF8("Дата")) );
    tbl->elementAt( 1, 3 )->addWidget( new WLabel(WString::fromUTF8("Текст")) );
    tbl->elementAt( 1, 4 )->addWidget( new WLabel(WString::fromUTF8("Статус")) );
    tbl->elementAt( 1, 5 )->addWidget( reportstatus );

    root()->addWidget( tbl );
}

AdminPage::MsgidList AdminPage::genMsgIds( const std::string& _idp, const std::string& phone, const std::string& _date, const std::string& _text, int page = 0 ) {
    PGSql::ConnectionHolder cHold( db );
    ConnectionPTR conn = cHold.get();
    TransactionPTR tr = db.openTransaction( conn, "AdminPage::genMsgIds" );

    std::ostringstream req;
    req << "SELECT \"REQUESTID\", \"TO\", \"WHEN\" FROM smsrequest WHERE TRUE ";
    if ( !phone.empty() ) {
        req << "AND \"TO\" LIKE '%" << phone << "%' ";
    }
    if ( !( _date == "Null" ) ) {
        boost::gregorian::date date = boost::gregorian::from_string( _date );
        boost::gregorian::date orig( 1970, boost::gregorian::Jan, 1 );
        boost::posix_time::ptime from( date, boost::posix_time::hours(0) );
        boost::posix_time::ptime to = from + boost::posix_time::hours(24);
        boost::posix_time::ptime begin( orig, boost::posix_time::hours(0) );

        boost::posix_time::time_period lv( begin, from );
        boost::posix_time::time_period rv( begin, to );
        req << "AND \"WHEN\" > '" << lv.length().total_seconds() << "' ";
        req << "AND \"WHEN\" < '" << rv.length().total_seconds() << "' ";
    }

    if ( !_text.empty() ) {
        req << "AND \"TXT\" LIKE '%" << utils::escapeString( tr->esc( _text ), "%_", "\\" ) << "%' ";
        req << "ESCAPE '\\\\' ";

    }

    if ( !_idp.empty() ) {
        req << "AND \"PID\"='" << tr->esc( _idp ) << "' ";
    }

    req << "ORDER BY \"WHEN\" DESC limit 1000; ";

    std::string a = req.str();


    MsgidList l;

    Result res = tr->exec( req.str() );
    tr->commit();
    for ( Result::const_iterator it = res.begin(); it != res.end(); it++ ) {
        std::string to;
        sms::to_vec tov;
        bool found;

        std::cout << (*it)[1].to( to );
        sms::utils::Tokenize(to, tov, ",");

        for (unsigned int i = 0; i < tov.size(); i++) {
            found = tov[i].find(phone);
            if (found != std::string::npos) {
                l.push_back(std::make_pair(sms::SMSMessage::ID((*it)[0].as<long long>(), i), tov[i]));
            }
        }
    }

    return l;
}

AdminPage::ReqResp AdminPage::genReq( const MsgidList& list, int status ) {

    MsgidList::const_iterator it;

    ReqResp r;

    for ( it = list.begin(); it != list.end(); it++ ) {
        PGSql::ConnectionHolder cHold( db );
        ConnectionPTR conn = cHold.get();
        TransactionPTR tr = db.openTransaction( conn, "AdminPage::genReq" );
        std::ostringstream req;
        req     << "SELECT \"smsrequest\".\"PID\", \"message_status\".\"WHEN\", \"smsrequest\".\"TXT\", \"message_status\".\"STATUS\" "
                << "FROM \"smsrequest\", \"message_status\" WHERE \"smsrequest\".\"REQUESTID\" = \"message_status\".\"REQUESTID\" "
                << "AND \"message_status\".\"REQUESTID\" = '" << it->first.req << "' "
                << "AND \"message_status\".\"MESSAGEID\" = '" << it->first.msg_num << "' ;";

        Result res = tr->exec( req.str() );
        tr->commit();
        for ( Result::const_iterator dbr = res.begin(); dbr != res.end(); dbr++ ) {
            std::string s;
            std::vector< std::string > row;

            row.push_back( it->first.to_str() );
            row.push_back((*dbr)[0].as<string>());
            row.push_back(it->second);
            boost::gregorian::date orig( 1970, boost::gregorian::Jan, 1 );
            boost::posix_time::ptime dt( orig, boost::posix_time::seconds( (*dbr)[1].as<long>() ) );

            row.push_back( boost::posix_time::to_simple_string(dt) );
            row.push_back( (*dbr)[2].as<std::string>() );
            int statusid;

            switch ( (*dbr)[3].as<int>() ) {
            case -3:
                if ( status == 1 )
                    continue;
                row.push_back( "Неверный номер" );
                break;
            case -2:
                if ( status == 1 )
                    continue;
                row.push_back( "Таймаут" );
                break;
            case -1:
                if ( status == 1 )
                    continue;
                row.push_back( "Не доставлено" );
                break;
            case 0:
                if ( status == 2 )
                    continue;
                row.push_back( "Доставлено" );
                break;
            default:
                if ( status != 0 )
                    continue;
                row.push_back( "В процессе доставки" );
            }

            r.push_back(row);
        }
    }

    return r;
}

void AdminPage::onReportGenerate() {

    reportbtn->disable();
    reportstatus->setText(WString::fromUTF8("Идет генерация данных"));

    while ( tbl->rowCount() > 2) {
        //tbl->rowAt( 2 )->
        tbl->deleteRow( 2 );
    }
    this->processEvents();

    try {
        std::string _phone = phone->text().toUTF8();
        std::string _date = date->date().toString("yyyy/MM/dd").toUTF8();
        std::string _text = text->text().toUTF8();
        std::string _pid = pid->text().toUTF8();

        int _status = status->currentIndex();

        MsgidList list = genMsgIds( _pid, _phone, _date, _text );

        ReqResp req = genReq( list, _status );

        for ( unsigned int i = 0; i < req.size(); i++ ) {


            if ( req[i][5] == "Доставлено" ) {
                tbl->rowAt( i+2 )->setStyleClass( "rowOk" );
            } else
            if ( req[i][5] == "В процессе доставки" ) {
                tbl->rowAt( i+2 )->setStyleClass( "rowWarn" );
            } else {
                tbl->rowAt( i+2 )->setStyleClass( "rowErr" );
            }

            tbl->elementAt( i+2, 0 )->addWidget( new WText( WString::fromUTF8(req[i][1] )) );
            tbl->elementAt( i+2, 1 )->addWidget( new WText( WString::fromUTF8(req[i][2] )) );
            tbl->elementAt( i+2, 2 )->addWidget( new WText( WString::fromUTF8(req[i][3] )) );

            WString txt = WString::fromUTF8( req[i][4] );

            if ( txt.value().length() > 45 ) {
                WInfoText* tr = new WInfoText( txt.value().substr(0, 45) + WString::fromUTF8("...").value() );
                tr->setStyleClass( "expandable" );
                tr->setApp( this );
                tr->setPos( i+2, 3 );
                tr->setMsg( req[i][0] );
                tr->setTextFull( req[i][4] );
                tr->clicked().connect( SLOT( tr, WInfoText::onTextFull) );
                tbl->elementAt( i+2, 3 )->addWidget( tr );//
            } else {
                WText* tr = new WText( WString::fromUTF8(req[i][4]) );
                tbl->elementAt( i+2, 3 )->addWidget( tr );
            }

            //tbl->elementAt( i+2, 3 )->addWidget( new WText( req[i][4] ) );
            WInfoText* tr = new WInfoText( WString::fromUTF8(req[i][5]) );
            tr->setApp(this);
            tr->setPos(i + 2, 4);
            tr->setMsg(req[i][0]);
            tr->clicked().connect( SLOT( tr, WInfoText::onHistoryDetail) );
            tr->setStyleClass( "expandable" );
            tbl->elementAt( i+2, 4 )->addWidget( tr );
            tbl->elementAt( i+2, 4 )->setColumnSpan( 2 );
        }
    } catch ( PGBrokenConnection& err ) {
        reportbtn->enable();
        reportstatus->setText(WString::fromUTF8(string("Ошибка соеднинения с базой") + err.what()));
        return;
    } catch ( PGSqlError& err ) {
        reportbtn->enable();
        reportstatus->setText(WString::fromUTF8(string("Ошибка SQL запроса") + err.what()));
        return;
    }

    reportbtn->enable();
    reportstatus->setText(WString::fromUTF8("Генерации отчета завершена"));
}

void WInfoText::onHistoryDetail() {

    std::vector< std::string > tov;
    utils::Tokenize(msgid, tov, ".");
    SMSMessage::ID msgid(atoll(tov[0].c_str()), atoi(tov[1].c_str()));
    SMSRequest::PTR req;
    try {
        SMSMessage::PTR msg = SMSMessageManager::get_mutable_instance().loadMessage(msgid);
        if ( !restore ) {

            SMSMessage::HistoryType::iterator it;
            WTable* ctbl = new WTable;
            int i;
            SMSMessage::HistoryType hst = msg->getHistory();
            ctbl->elementAt( 0, 0 )->addWidget( new WText( "ID=" + msg->getID().to_str() ) );
            ctbl->elementAt( 0, 0 )->addWidget( new WBreak() );

            RequestTracker* trck = RequestTracker::Instance();
            req = trck->loadRequestFromDb( msgid.req );
            ctbl->elementAt( 0, 0 )->addWidget( new WText( "tid=" + req->tid ) );
            ctbl->elementAt( 0, 0 )->addWidget( new WBreak() );

            for ( i = 1, it = hst.begin(); it != hst.end(); it++, i++ ) {
                std::string d;
                if ( it->op_direction == 0 ) {
                    d = "-->";
                } else {
                    d = "<--";
                }

                std::string k;
                switch ( it->op_result() ) {
                case SMSMessage::Status::ST_UNKNOWN:
                    k = "ST_UNKNOWN";
                    break;
                case SMSMessage::Status::ST_REJECTED:
                    k = "ST_REJECTED";
                    break;
                case SMSMessage::Status::ST_PREPARING:
                    k = "ST_PREPARING";
                    break;
                case SMSMessage::Status::ST_BUFFERED:
                    k = "ST_BUFFERED";
                    break;
                case SMSMessage::Status::ST_ABSENT:
                    k = "ST_ABSENT";
                    break;
                case SMSMessage::Status::ST_DELIVERED:
                    k = "ST_DELIVERED";
                    break;
                case SMSMessage::Status::ST_NOT_DELIVERED:
                    k = "ST_NOT_DELIVERED";
                    break;
                case SMSMessage::Status::ST_EXPIRED:
                    k = "ST_EXPIRED";
                    break;
                default:
                    k = "ST_UNKNOWN";
                    break;
                }

                ctbl->elementAt( i, 0 )->addWidget( new WText(WString::fromUTF8(k)) );
                ctbl->elementAt( i, 0 )->addWidget( new WBreak() );
                ctbl->elementAt( i, 0 )->addWidget( new WText( WString::fromUTF8("["+it->gateway+"]" )) );
                //ctbl->elementAt( i, 1 )->addWidget( new WText(d) );
                boost::gregorian::date orig(1970, boost::gregorian::Jan, 1);
                boost::posix_time::ptime dt(orig, boost::posix_time::seconds( it->when ));
                ctbl->elementAt( i, 1 )->addWidget( new WText( WString::fromUTF8(boost::posix_time::to_simple_string( dt ))) );
            }

            WBreak* br = new WBreak();
            app->tbl->elementAt( row, column )->addWidget( br );
            app->tbl->elementAt( row, column )->addWidget( ctbl );

            added.push_back( br );
            added.push_back( ctbl );

        } else {
            std::list< WWidget* >::iterator it;
            int i;
            for ( i = 0, it = added.begin(); it != added.end(); it++, i++ ) {
                app->tbl->elementAt( row, column )->removeWidget( *it );
            }
            added.clear();
        }
        restore = !restore;
    } catch ( MessageNotFoundError& err ) {
        Logger::get_mutable_instance().smslogwarn( string("AdminPage::genReq") + err.what() );
        return;
    }

}

void WInfoText::onTextFull() {
    if ( !restore ) {
        this->setText( WString::fromUTF8(textfull) );
    } else {
        WString txt = WString::fromUTF8( textfull );
        this->setText( txt.value().substr(0, 45) + WString::fromUTF8("...").value() );
    }
    restore = !restore;

}

AdminPage::~AdminPage() {
}

