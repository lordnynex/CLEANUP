/*
 * File:   PersonalCabinet.cpp
 * Author: mohtep
 *
 * Created on 1 Март 2010 г., 0:57
 */

#include <Wt/WLineEdit>

#include "PersonalCabinet.h"
#include "Logger.h"

PGSql& PersonalCabinet::db( PGSqlConnPoolStats::get_mutable_instance().getdb() );

WApplication *createPersonalCabinet(const WEnvironment& env) {
  /*
   * You could read information from the environment to decide whether
   * the user has permission to start a new application
   */
  return new PersonalCabinet(env);
}


PersonalCabinet::PersonalCabinet( const WEnvironment& env ): WApplication( env ) {
    setTitle( WString::fromUTF8("SMSGate admin page") );

    this->setCssTheme("polished");
    this->useStyleSheet( "/resources/css/resp_table.css" );

    tbl = new WTable();
    tbl->setHeaderCount(2);
    reportbtn = new WPushButton( WString::fromUTF8("Сгенерировать отчет") );
    reportbtn->clicked().connect(SLOT(this, PersonalCabinet::onReportGenerate));

    reportstatus = new WLabel();

    pid = new WLineEdit(); pid->setMaximumSize(  WLength( 1, WLength::Centimeter ), WLength::Auto  );
    phone = new WLineEdit();
    date_from = new WDatePicker(); date_from->setDate( WDate::currentDate().addDays( -7 ) );
    date_to = new WDatePicker(); date_to->setDate( WDate::currentDate() );
    text = new WLineEdit();
    text->setMinimumSize( WLength( 6, WLength::Centimeter ), WLength::Auto );
    status = new WComboBox();
    status->addItem(WString::fromUTF8("Любой"));
    status->addItem(WString::fromUTF8("Доставлено"));
    status->addItem(WString::fromUTF8("Не доставлено"));
    tbl->elementAt( 0, 1 )->addWidget( phone );
    tbl->elementAt( 0, 2 )->addWidget( new WLabel( WString::fromUTF8("С") ) );
    tbl->elementAt( 0, 2 )->addWidget( date_from );
    tbl->elementAt( 0, 2 )->addWidget( new WLabel( WString::fromUTF8("По") ) );
    tbl->elementAt( 0, 2 )->addWidget( date_from );
    tbl->elementAt( 0, 2 )->addWidget( date_to );
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

PersonalCabinet::MsgidList PersonalCabinet::genMsgIds( const std::string& _idp, const std::string& phone, const std::string& _ldate, const std::string& _rdate, const std::string& _text, int page = 0 ) {
    PGSql::ConnectionHolder cHold( db );
    ConnectionPTR conn = cHold.get();
    TransactionPTR tr = db.openTransaction( conn, "PersonalCabinet::genMsgIds" );

    std::ostringstream req;
    req << "SELECT \"REQUESTID\", \"TO\", \"WHEN\" FROM smsrequest WHERE TRUE ";
    if ( !phone.empty() ) {
        req << "AND \"TO\" LIKE '%" << phone << "%' ";
    }
    if ( !( _ldate == "Null" ) ) {
        boost::gregorian::date date = boost::gregorian::from_string( _ldate );
        boost::gregorian::date orig( 1970, boost::gregorian::Jan, 1 );
        boost::posix_time::ptime from( date, boost::posix_time::hours(0) );
        boost::posix_time::ptime begin( orig, boost::posix_time::hours(0) );

        boost::posix_time::time_period lv( begin, from );
        req << "AND \"WHEN\" > '" << lv.length().total_seconds() << "' ";
    }

    if ( !( _rdate == "Null" ) ) {
        boost::gregorian::date date = boost::gregorian::from_string( _rdate );
        boost::gregorian::date orig( 1970, boost::gregorian::Jan, 1 );
        boost::posix_time::ptime from( date, boost::posix_time::hours(0) );
        boost::posix_time::ptime to = from + boost::posix_time::hours(24);
        boost::posix_time::ptime begin( orig, boost::posix_time::hours(0) );

        boost::posix_time::time_period rv( begin, to );
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

PersonalCabinet::ReqResp PersonalCabinet::genReq( const MsgidList& list, int status ) {

    MsgidList::const_iterator it;

    ReqResp r;

    for ( it = list.begin(); it != list.end(); it++ ) {
        PGSql::ConnectionHolder cHold( db );
        ConnectionPTR conn = cHold.get();
        TransactionPTR tr = db.openTransaction( conn, "PersonalCabinet::genReq" );
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

void PersonalCabinet::onReportGenerate() {

    reportbtn->disable();
    reportstatus->setText(WString::fromUTF8("Идет генерация данных"));

    while ( tbl->rowCount() > 2) {
        //tbl->rowAt( 2 )->
        tbl->deleteRow( 2 );
    }
    this->processEvents();

    try {
        std::string _phone = phone->text().toUTF8();
        std::string _ldate = date_from->date().toString("yyyy/MM/dd").toUTF8();
        std::string _rdate = date_to->date().toString("yyyy/MM/dd").toUTF8();
        std::string _text = text->text().toUTF8();
        std::string _pid = "101";

        int _status = status->currentIndex();

        MsgidList list = genMsgIds( _pid, _phone, _ldate,_rdate, _text );

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

            WText* tr = new WText( WString::fromUTF8(req[i][4]) );
            tbl->elementAt( i+2, 3 )->addWidget( tr );

            //tbl->elementAt( i+2, 3 )->addWidget( new WText( req[i][4] ) );
            tr = new WInfoText( WString::fromUTF8(req[i][5]) );
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

PersonalCabinet::~PersonalCabinet() {
}

