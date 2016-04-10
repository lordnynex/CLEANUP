#include "StatisticsBlock.h"

#include <Wt/WLabel>
#include <Wt/WLineEdit>
#include <Wt/WDatePicker>
#include <Wt/WCalendar>
#include <Wt/WComboBox>
#include <Wt/WSpinBox>
#include <Wt/WPushButton>
#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>
#include <Wt/WDialog>
#include <Wt/WSuggestionPopup>
#include <Wt/WPopupMenu>

#include "Logger.h"
#include "MessageClassifier.h"
#include "SMSMessage.h"
#include "PartnerManager.h"
#include "SendMessageForm.h"
#include "RequestTracker.h"

using namespace Wt;
using namespace std;

PGSql& StatisticsBlock::db( PGSqlConnPoolStats::get_mutable_instance().getdb() );

WStatPageHeader::WStatPageHeader( StatisticsBlock* ppage ) {
    this->ppage = ppage;

}

int WStatPageHeader::getTotalLines() {
    return 2;
}

void WStatPageHeader::execute( int lnl, int lnr, RowList &data ) {
    data.clear();
    WLabel* report_status = new WLabel();
    report_status->clicked().connect( ppage, &StatisticsBlock::onSummaryShow );

    for ( int line = lnl; line <= lnr; line++ ) {
        if ( line == 0 ) {
            Row r;
            r.reserve( 7 );
            WLineEdit* pid;
            WLineEdit* phone;
            WDatePicker* date_from, *date_to;
            WLabel* newMessage;
            WLineEdit* text;
            WComboBox* status;
            WTable* date;
            WTable* options;
            WCheckBox* auto_refresh;
            WCheckBox* update_order;
            WLineEdit* country;
            WPushButton* reportbtn;
            WContainerWidget* controlBlock;
            WTable* controlBlockTable;
            WLayout* controlBlockLayout;
            WLabel* next;
            WLabel* prev;
            WSpinBox* page;
            WLayout* lnext;
            WLayout* lprev;
            WLayout* lpage;

            if ( ppage->isAdmin ) {
                //Pid input field
                pid = new WLineEdit(); pid->setMaximumSize(  WLength( 1, WLength::Centimeter ), WLength::Auto  );
            } else {
                pid = NULL;
            }
            //Phone input field
            phone = new WLineEdit();
            //Date input field
            date_from = new WDatePicker(); date_from->setDate( WDate::currentDate().addDays( -7 ) );
            date_from->calendar()->setSingleClickSelect( true );
            date_to = new WDatePicker(); date_to->setDate( WDate::currentDate() );
            date_to->calendar()->setSingleClickSelect( true );
            date = new WTable();
            date->elementAt(0, 0)->addWidget( new WLabel( WString::fromUTF8("С") ) );
            date->elementAt(0, 1)->addWidget( date_from );
            date->elementAt(1, 0)->addWidget( new WLabel( WString::fromUTF8("По") ) );
            date->elementAt(1, 1)->addWidget( date_to );
            date->addStyleClass("datetable");
            //New message link
            newMessage = new WLabel( WString::fromUTF8( "Отправить сообщение" ) );
            newMessage->addStyleClass( "link" );
            newMessage->clicked().connect( ppage, &StatisticsBlock::onNewMessage );
            //Auto update option
            auto_refresh = new WCheckBox( WString::fromUTF8( "Обновлять" ) );
            //Last event update option
            update_order = new WCheckBox( WString::fromUTF8( "По изменению" ) );
            options = new WTable();
            options->elementAt(0, 0)->addWidget( newMessage );
            options->elementAt(1, 0)->addWidget( auto_refresh );
            options->elementAt(2, 0)->addWidget( update_order );
            options->addStyleClass("datetable");
            //Message text field
            text = new WLineEdit();
            text->setMinimumSize( WLength( 80, WLength::Percentage ), WLength::Auto );
            //Delivery status field
            status = new WComboBox();
            status->addItem(WString::fromUTF8("Любой"));
            status->addItem(WString::fromUTF8("Доставлено"));
            status->addItem(WString::fromUTF8("Отправлено"));
            status->addItem(WString::fromUTF8("Не доставлено"));
            status->addItem(WString::fromUTF8("Неверный номер"));
            status->addItem(WString::fromUTF8("Счет выставлен"));
            status->addItem(WString::fromUTF8("Оплачено"));
            status->addItem(WString::fromUTF8("Отказ оплаты"));

            //Country input field
            country = new WLineEdit(); country->setMinimumSize(  WLength( 3, WLength::Centimeter ), WLength::Auto  );
            Wt::WSuggestionPopup::Options suggestOptions
            = { "<b>",         // highlightBeginTag
                "</b>",        // highlightEndTag
                ',',           // listSeparator      (for multiple addresses)
                " \\n",        // whitespace
                " ",           // wordSeparators     (within an address)
                ""             // appendReplacedText (prepare next email address)
               };
            WSuggestionPopup* suggest_country = new WSuggestionPopup( suggestOptions, ppage );
            suggest_country->forEdit( country );

            {
                std::vector< std::string > countries;
                MessageClassifier::CountryOperatorMapT comap = MessageClassifier::get_mutable_instance().getCOMap();
                for ( MessageClassifier::CountryOperatorMapT::iterator it = comap.begin(); it != comap.end(); it++ ) {
                    countries.push_back( it->second.cName );
                }
                std::sort( countries.begin(), countries.end() );

                for ( std::vector< std::string >::iterator it = countries.begin(); it != countries.end(); it++ ) {
                    suggest_country->addSuggestion( WString::fromUTF8(*it), WString::fromUTF8(*it) );
                }
            }

            //Report button
            reportbtn = new WPushButton( WString::fromUTF8("Сгенерировать отчет") );

            page = new WSpinBox(); page->setRange( 1, 1 );
            page->setValue(1);
            page->setMaximumSize(  WLength( 1, WLength::Centimeter ), WLength::Auto  );
            page->valueChanged().connect( boost::bind(
                                             &WScrollTable::exactPage,
                                             ppage->statistics,
                                             _1
                                                     ) );

            next = new WLabel( WString::fromUTF8(">>") );
            next->addStyleClass("link");
            next->clicked().connect( boost::bind(
                                        &StatisticsBlock::onPageInc,
                                        ppage,
                                        page
                                                ) );

            prev = new WLabel( WString::fromUTF8("<<") );
            prev->addStyleClass("link");
            prev->clicked().connect( boost::bind(
                                        &StatisticsBlock::onPageDec,
                                        ppage,
                                        page
                                                ) );
            lnext = new WVBoxLayout();
            lnext->addWidget( next );
            lprev = new WVBoxLayout();
            lprev->addWidget( prev );
            lpage = new WVBoxLayout();
            lpage->addWidget( page );

            controlBlockTable = new WTable();
            controlBlockTable->elementAt( 0, 0 )->addWidget( reportbtn );
            controlBlockTable->elementAt( 0, 0 )->setColumnSpan( 3 );
            controlBlockTable->elementAt( 1, 0 )->setLayout( lprev, AlignLeft );
            controlBlockTable->elementAt( 1, 1 )->setLayout( lpage, AlignCenter );
            controlBlockTable->elementAt( 1, 2 )->setLayout( lnext, AlignRight );
            controlBlockTable->setStyleClass("datetable");

            controlBlockLayout = new WVBoxLayout();
            controlBlockLayout->addWidget( controlBlockTable );

            controlBlock = new WContainerWidget();
            controlBlock->setLayout( controlBlockLayout, AlignRight );

            StatisticsBlock::RowInfo info;
            info.pid = pid;
            info.phone = phone;
            info.date_from = date_from;
            info.date_to = date_to;
            info.text = text;
            info.status = status;
            info.country = country;
            info.reportbtn = reportbtn;
            info.page = page;
            info.report_status = report_status;
            info.auto_refresh = auto_refresh;
            info.update_order = update_order;

            reportbtn->clicked().connect(boost::bind(
                                             &StatisticsBlock::onReportBtnClicked,
                                             ppage,
                                             info )
                                         );
            if ( ppage->isAdmin )
                r.push_back( pid );
            r.push_back( phone );
            r.push_back( date );
            r.push_back( text );
            r.push_back( status );
            r.push_back( country );
            r.push_back( options );
            r.push_back( NULL );
            r.push_back( controlBlock );

            data.push_back( r );
        }

        if ( line == 1 ) {
            Row r;
            r.reserve( 7 );
            if ( ppage->isAdmin )
                r.push_back( new WLabel(WString::fromUTF8("IDP")) );
            r.push_back( new WLabel(WString::fromUTF8("Телефон")) );
            r.push_back( new WLabel(WString::fromUTF8("Дата")) );
            r.push_back( new WLabel(WString::fromUTF8("Текст")) );
            r.push_back( new WLabel(WString::fromUTF8("Статус")) );
            r.push_back( new WLabel(WString::fromUTF8("Страна")) );
            r.push_back( new WLabel(WString::fromUTF8("Регион")) );
            r.push_back( new WLabel(WString::fromUTF8("Цена")) );
            r.push_back( report_status );

            data.push_back( r );
        }
    }
}

WStatPageData::WStatPageData( StatisticsBlock* _ppage ) {
    initialized = false;
    __total_lines = 0;
    ppage = _ppage;

    view_name = string("v") + ppage->env.sessionId();
    res_name =string("p") + ppage->env.sessionId();
}

WStatPageData::~WStatPageData( ) {
    PGSql& db = ppage->db;

    // Drop temp table if exists
    if ( initialized ) {
        try {
            PGSql::ConnectionHolder cHold( db );
            ConnectionPTR conn = cHold.get();
            TransactionPTR tr = db.openTransaction( conn, "WStatPageData::~WStatPageData ( drop result table ) " );

            std::ostringstream req;
            req     <<  "SELECT drop_matview( '" << res_name << "' );";

            tr->exec( req.str() );
            tr->commit();

        } catch ( ... ) {
            initialized = false;
        }
    }

}

void WStatPageData::prepareRequest( ) {
    PGSql& db = ppage->db;

    // Drop temp table if exists
    if ( initialized ) {
        try {
            PGSql::ConnectionHolder cHold( db );
            ConnectionPTR conn = cHold.get();
            TransactionPTR tr = db.openTransaction( conn, "WStatPageData::prepareRequest ( drop result table ) " );

            std::ostringstream req;
            req     <<  "SELECT drop_matview( '" << res_name << "' );";

            tr->exec( req.str() );
            tr->commit();

        } catch ( ... ) {
            initialized = false;
        }

        try {
            PGSql::ConnectionHolder cHold( db );
            ConnectionPTR conn = cHold.get();
            TransactionPTR tr = db.openTransaction( conn, "WStatPageData::prepareRequest ( drop result view ) " );

            std::ostringstream req;
            req     <<  "DROP VIEW " << view_name << ";";

            tr->exec( req.str() );
            tr->commit();

        } catch ( ... ) {
            initialized = false;
        }
    }

    initialized = true;

    PartnerInfo user = PartnerManager::get_mutable_instance().findById( ppage->pId );
    // create temp view according setup filters
    try {
        PGSql::ConnectionHolder cHold( db );
        ConnectionPTR conn = cHold.get();
        TransactionPTR tr = db.openTransaction( conn, "WStatPageData::prepareRequest ( create temp view ) " );

        std::ostringstream req;
        req     <<  "CREATE OR REPLACE TEMP VIEW " << view_name << " AS ";
        req     <<  "SELECT message_status.\"REQUESTID\", message_status.\"MESSAGEID\", \"TXT\", \"FROM\", \"PID\", message_status.\"WHEN\" AS REQUESTDATE, "
                <<  "\"STATUS\", message_status.\"TO\", \"PARTS\", \"COUNTRY\", \"COUNTRYCODE\", \"OPERATOR\", \"OPERATORCODE\", \"REGION\", "
                <<  "message_status.\"WHEN\" AS DELIVERYDATE, \"PARTNERPRICE\", \"OURPRICE\" "
                <<  "FROM smsrequest, message_status ";
        if ( !( user.ownerId.empty() || ( user.pId == pid_value) ) )
            req << ", partners ";
        if ( sort_by_updated )
            req << ", message_history ";
        req     << "WHERE smsrequest.\"REQUESTID\"=message_status.\"REQUESTID\" ";
        if ( sort_by_updated ) {
            req << "AND message_history.\"REQUESTID\"=message_status.\"REQUESTID\" ";
            req << "AND message_history.\"MESSAGEID\"=message_status.\"MESSAGEID\" ";
        }
        if ( !( user.ownerId.empty() || (user.pId == pid_value) ) )
            req <<  "AND \"PID\"=partners.pid ";
        if ( pid_filter )
            req <<  "AND \"PID\"='" << tr->esc( pid_value ) << "' ";
        if ( phone_filter )
            req <<  "AND message_status.\"TO\"='" << tr->esc( phone_value ) << "' ";
        if ( date_from_filter )
            req <<  "AND smsrequest.\"WHEN\">'" << date_from_value << "' ";
        if ( date_to_filter )
            req <<  "AND smsrequest.\"WHEN\"<'" << date_to_value << "' ";
        if ( text_filter )
            req << "AND \"TXT\" LIKE '%" << utils::escapeString( tr->esc( text_value ), "%_", "\\" ) << "%' ESCAPE E'\\\\' ";
        if ( country_filter )
            req <<  "AND \"COUNTRY\"='" << tr->esc(country_value) << "' ";
        if ( status_filter )
            req <<  "AND \"STATUS\"='" << status_value() << "' ";
        if ( !( user.ownerId.empty() || (user.pId == pid_value) ) )
            req << "AND partners.ownerid='" << tr->esc( ppage->pId ) << "' ";
        if ( sort_by_updated )
            req << "GROUP BY message_status.\"REQUESTID\", message_status.\"MESSAGEID\", \"TXT\", \"FROM\", \"PID\", message_status.\"WHEN\", "
                << "\"STATUS\", message_status.\"TO\", \"PARTS\", \"COUNTRY\", \"COUNTRYCODE\", \"OPERATOR\", \"OPERATORCODE\", \"REGION\", "
                << "message_status.\"WHEN\", \"PARTNERPRICE\", \"OURPRICE\" "
                << "ORDER BY MAX(message_history.\"WHEN\") DESC ";
        else
            req << "ORDER BY smsrequest.\"WHEN\" DESC;";


        {
            boost::xtime from, to;
            boost::xtime_get( &from, boost::TIME_UTC_ );
            tr->exec( req.str() );
            boost::xtime_get( &to, boost::TIME_UTC_ );
            req << " for " << to.sec - from.sec << " seconds";
            Logger::get_mutable_instance().dbloginfo( req.str() );
        }

        std::ostringstream req2;
        req2    <<  "SELECT create_matview( '" << res_name << "', '" << view_name << "' );";

        {
            boost::xtime from, to;
            boost::xtime_get( &from, boost::TIME_UTC_ );
            tr->exec( req2.str() );
            tr->commit();
            boost::xtime_get( &to, boost::TIME_UTC_ );
            req2 << " for " << to.sec - from.sec << " seconds";
            Logger::get_mutable_instance().dbloginfo( req2.str() );
        }


    } catch ( ... ) {
        initialized = false;
        throw;
    }

    // calculate total_lines_value
    try {
        PGSql::ConnectionHolder cHold( db );
        ConnectionPTR conn = cHold.get();
        TransactionPTR tr = db.openTransaction( conn, "WStatPageData::prepareRequest ( fill result table ) " );

        std::ostringstream req;
        req     <<  "SELECT count(*) from " << res_name << ";";


        {
            boost::xtime from, to;
            boost::xtime_get( &from, boost::TIME_UTC_ );
            Result res = tr->exec( req.str() );
            tr->commit();

            for ( Result::const_iterator it = res.begin(); it != res.end(); it++ ) {
                __total_lines = (*it)[0].as<int>();
            }
            boost::xtime_get( &to, boost::TIME_UTC_ );
            req << " for " << to.sec - from.sec << " seconds";
            Logger::get_mutable_instance().dbloginfo( req.str() );
        }

    } catch ( ... ) {
        initialized = false;
        throw;
    }

}

void WStatPageData::resetFilter( ) {
    initialized = true;
    pid_filter = false;
    phone_filter = false;
    date_from_filter = false;
    date_to_filter = false;
    text_filter = false;
    country_filter = false;
    status_filter = false;
    sort_by_updated = false;
}

void WStatPageData::setPidFilter( string pid ) {
    pid_filter = true;
    pid_value = pid;
}

void WStatPageData::setPhoneFilter( string phone ) {
    phone_filter = true;
    phone_value = phone;
}

void WStatPageData::setDateFromFilter( long date_from ) {
    date_from_filter = true;
    date_from_value = date_from;
}

void WStatPageData::setDateToFilter( long date_to ) {
    date_to_filter = true;
    date_to_value = date_to;
}

void WStatPageData::setTextFilter( string text ) {
    text_filter = true;
    text_value = text;
}

void WStatPageData::setCountryFilter( string country ) {
    country_filter = true;
    country_value = country;
}

void WStatPageData::setStatusFilter( SMSMessage::Status status ) {
    status_filter = true;
    status_value = status;
}

void WStatPageData::setModifiedSearchOrder( ) {
    sort_by_updated = true;
}

int WStatPageData::getTotalLines() {
    if ( !initialized ) {
        return 0;
    }
    return __total_lines;
}

void WStatPageData::evaluateSummary( 	double &price, 
					int &total, int &total_parts, 
					int &delivered, int &delivered_parts, 
					int &rejected, int &rejected_parts, 
					int &undelivered, int &undelivered_parts,
					int &buffered, int &buffered_parts ) {
    PGSql& db = ppage->db;

    price = 0;
    total = 0;		total_parts = 0;
    delivered = 0;	delivered_parts = 0;
    rejected = 0;	rejected_parts = 0;
    undelivered = 0;	undelivered_parts = 0;
    buffered = 0;	buffered_parts = 0;
    try {
        PGSql::ConnectionHolder cHold( db );
        ConnectionPTR conn = cHold.get();
        TransactionPTR tr = db.openTransaction( conn, "WStatPageData::evaluateSummary ( summary results ) " );

        std::ostringstream req;
            req     <<  "select \"STATUS\", sum(\"PARTS\"), sum(\"PARTNERPRICE\"), count(\"PARTS\") from " << res_name << " GROUP BY \"STATUS\";";

        Result res = tr->exec( req.str() );
        tr->commit();

        for ( Result::const_iterator it = res.begin(); it != res.end(); it++ ) {
            SMSMessage::Status status = SMSMessage::Status( (*it)[0].as<int>() );

            if ( status == SMSMessage::Status::ST_BUFFERED ) {
                buffered += (*it)[1].as<int>();
                buffered_parts += (*it)[3].as<int>();
            }

            if ( status == SMSMessage::Status::ST_REJECTED ) {
                rejected += (*it)[1].as<int>();
                rejected_parts += (*it)[3].as<int>();
            }

            if ( status == SMSMessage::Status::ST_DELIVERED ) {
                delivered += (*it)[1].as<int>();
                delivered_parts += (*it)[3].as<int>();
            }

            if ( status == SMSMessage::Status::ST_NOT_DELIVERED ) {
                undelivered += (*it)[1].as<int>();
                undelivered_parts += (*it)[3].as<int>();
            }

            total += (*it)[1].as<int>();
            total_parts += (*it)[3].as<int>();
            price += (*it)[2].as<double>();
        }

    } catch ( ... ) {
    }
}

void WStatPageData::execute( int lnl, int lnr, RowList &data ) {
    if ( !initialized ) {
        data.clear();
        return;
    }
    PGSql& db = ppage->db;



    // build required widgets
    try {
        PGSql::ConnectionHolder cHold( db );
        ConnectionPTR conn = cHold.get();
        TransactionPTR tr = db.openTransaction( conn, "WStatPageData::execute ( extract results ) " );

        std::ostringstream req;
        req     <<  "SELECT * from " << res_name << " ORDER BY REQUESTDATE DESC limit " << lnr - lnl + 1 << " offset " << lnl << ";";

        Result res = tr->exec( req.str() );
        tr->commit();

        data.clear();
        for ( Result::const_iterator it = res.begin(); it != res.end(); it++ ) {
            SMSMessage::ID msgid = SMSMessage::ID( (*it)[0].as<long long>(), (*it)[1].as<int>() );
            string __pid = (*it)[4].as<string>();
            string __phone = (*it)[7].as<string>();
            long __date_num = (*it)[5].as<long>();
            int ts = PartnerManager::get_mutable_instance().findById( this->ppage->pId ).tzone;

            string __date = utils::ts2datetime( __date_num, ts );
            string __txt = (*it)[2].as<string>();
            string __status = SMSMessage::Status::russianDescr( SMSMessage::Status( (*it)[6].as<int>() ) );
            double price = (*it)[15].as<double>();
            double ourprice = (*it)[16].as<double>();

            char ps[100];
            sprintf( ps, "%0.2f", price );
            string __price = ps;

            string __country = (*it)[9].as<string>();
            string __region = (*it)[13].as<string>();

            Row row;
            if ( ppage->isAdmin )
                row.push_back( new WLabel( WString::fromUTF8( __pid ) ) );

            WLabel* txt_label = new WLabel( WString::fromUTF8( __txt ) );
            txt_label->setWordWrap( true );
            txt_label->setStyleClass("ww");
            WLabel* status_label = new WLabel( WString::fromUTF8( __status ) );
            if ( ( SMSMessage::Status( (*it)[6].as<int>() ) == SMSMessage::Status::ST_DELIVERED ) || ( SMSMessage::Status( (*it)[6].as<int>() ) == SMSMessage::Status::ST_PAID ) ) {
                status_label->setStyleClass("rowOk");
            } else
            if ( ( SMSMessage::Status( (*it)[6].as<int>() )() < 0 ) || ( SMSMessage::Status( (*it)[6].as<int>() ) == SMSMessage::Status::ST_CANCELED ) ) {
                status_label->setStyleClass("rowErr");
            } else {
                status_label->setStyleClass("rowWarn");
            }

            if ( ppage->isAdmin ) {
                status_label->clicked().connect( boost::bind( &StatisticsBlock::onMoreInfo, ppage, _1, msgid ) );
            }

            row.push_back( new WLabel( WString::fromUTF8( __phone ) ) );
            row.push_back( new WLabel( WString::fromUTF8( __date ) ) );
            row.push_back( txt_label );
            row.push_back( status_label );
            row.push_back( new WLabel( WString::fromUTF8( __country ) ) );
            row.push_back( new WLabel( WString::fromUTF8( __region ) ) );
            row.push_back( new WLabel( WString::fromUTF8( __price ) ) );

            PartnerInfo userid = PartnerManager::get_mutable_instance().findById( ppage->pId );
            if ( userid.ownerId.empty() ) {
                sprintf( ps, "%0.2f ( %+0.2f )", ourprice, price - ourprice );
                string __ourprice = ps;

                WLabel* ourprice_label = new WLabel( WString::fromUTF8( __ourprice ) );
                if ( ourprice > price ) {
                    ourprice_label->setStyleClass( "rowErr" );
                }

                row.push_back( ourprice_label );
            }

            data.push_back( row );
        }

    } catch ( ... ) {
        initialized = false;
    }
}

void StatisticsBlock::onReportBtnClicked( RowInfo row ) {

    data.resetFilter();

    if ( row.pid && isAdmin && !row.pid->text().empty() )
        data.setPidFilter( row.pid->text().toUTF8() );

    if ( !isAdmin )
        data.setPidFilter( pId );

    if ( !row.phone->text().empty() )
        data.setPhoneFilter( row.phone->text().toUTF8() );

    if ( !row.text->text().empty() )
        data.setTextFilter( row.text->text().toUTF8() );

    if ( !row.country->text().empty() )
        data.setCountryFilter( row.country->text().toUTF8() );

    std::string _ldate = row.date_from->date().toString("yyyy/MM/dd").toUTF8();
    if ( !_ldate.empty() ) {
        boost::gregorian::date date = boost::gregorian::from_string( _ldate );
        boost::gregorian::date orig( 1970, boost::gregorian::Jan, 1 );
        boost::posix_time::ptime from( date, boost::posix_time::hours(0) );
        boost::posix_time::ptime begin( orig, boost::posix_time::hours(0) );
        boost::posix_time::time_period lv( begin, from );

        data.setDateFromFilter( lv.length().total_seconds()-PartnerManager::get_mutable_instance().findById( pId ).tzone*60*60 );
    }

    std::string _rdate = row.date_to->date().toString("yyyy/MM/dd").toUTF8();
    if ( !_rdate.empty() ) {
        boost::gregorian::date date = boost::gregorian::from_string( _rdate );
        boost::gregorian::date orig( 1970, boost::gregorian::Jan, 1 );
        boost::posix_time::ptime from( date, boost::posix_time::hours(0) );
        boost::posix_time::ptime to = from + boost::posix_time::hours(24);
        boost::posix_time::ptime begin( orig, boost::posix_time::hours(0) );
        boost::posix_time::time_period rv( begin, to );

        data.setDateToFilter( rv.length().total_seconds()-PartnerManager::get_mutable_instance().findById( pId ).tzone*60*60 );
    }

    if ( row.status->currentIndex() == 0 ) {
        // Ничего не делаем
    }

    if ( row.status->currentIndex() == 1 ) {
        data.setStatusFilter( SMSMessage::Status( SMSMessage::Status::ST_DELIVERED ) );
    }

    if ( row.status->currentIndex() == 2 ) {
        data.setStatusFilter( SMSMessage::Status( SMSMessage::Status::ST_BUFFERED ) );
    }

    if ( row.status->currentIndex() == 3 ) {
        data.setStatusFilter( SMSMessage::Status( SMSMessage::Status::ST_NOT_DELIVERED ) );
    }

    if ( row.status->currentIndex() == 4 ) {
        data.setStatusFilter( SMSMessage::Status( SMSMessage::Status::ST_REJECTED ) );
    }

    if ( row.status->currentIndex() == 5 ) {
        data.setStatusFilter( SMSMessage::Status( SMSMessage::Status::ST_BILLED ) );
    }

    if ( row.status->currentIndex() == 6 ) {
        data.setStatusFilter( SMSMessage::Status( SMSMessage::Status::ST_PAID) );
    }

    if ( row.status->currentIndex() == 7 ) {
        data.setStatusFilter( SMSMessage::Status( SMSMessage::Status::ST_CANCELED) );
    }

    if ( row.update_order->isChecked() ) {
        data.setModifiedSearchOrder( );
    }

    row.reportbtn->disable();
    row.report_status->setText(WString::fromUTF8("Обработка"));

    try {
        data.prepareRequest();
    } catch ( PGBrokenConnection& err ) {
        row.reportbtn->enable();
        row.report_status->setText(WString::fromUTF8(string("Ошибка соеднинения с базой") + err.what()));
        return;
    } catch ( PGSqlError& err ) {
        row.reportbtn->enable();
        row.report_status->setText(WString::fromUTF8(string("Ошибка SQL запроса") + err.what()));
        return;
    }

    statistics->rebuildData();

    row.reportbtn->enable();
    row.report_status->setText(WString::fromUTF8(string("Готово: ") + boost::lexical_cast<string>( statistics->getLastPage() + 1 ) + string(" страниц")));
    if ( data.getTotalLines() ) {
        row.report_status->setStyleClass("link");
    } else {
        row.report_status->setStyleClass("");
    }

    onPageUpdate( row.page );
}

void StatisticsBlock::onNewMessage() {
    WDialog summary;
    summary.setWindowTitle( WString::fromUTF8("Создать сообщение") );
    summary.setTitleBarEnabled( true );

    SendMessageForm* msgform = new SendMessageForm( );
    msgform->cancelBtn->clicked().connect( &summary, &WDialog::reject );
    msgform->sendBtn->clicked().connect( &summary, &WDialog::accept );
    PartnerInfo user=PartnerManager::get_mutable_instance().findById( pId );
    if ( !user.ownerId.empty() ) {
        msgform->gateway->hide();
        msgform->pidEdit->hide();
    }

    summary.contents()->addWidget( msgform );

    while ( 1 ) {

        WDialog::DialogCode r=summary.exec();
        if ( r == WDialog::Rejected )
            return;

        boost::xtime now;
        boost::xtime_get( &now, boost::TIME_UTC_ );

        SMSRequest* req = new SMSRequest();
        SMSRequest::PTR reqptr = SMSRequest::PTR( req );
        PartnerInfo p = PartnerManager::get_mutable_instance().findById( pId );
        std::string to=msgform->toEdit->text().toUTF8();
        to_vec tov;
        utils::Tokenize( to, tov, ",");
        try {
            req->parse(  p.pName,
                       p.pPass,
                       tov,
                       msgform->textEdit->text().toUTF8(),
                       "",
                       msgform->fromEdit->text().empty()? "GREENSMS": msgform->fromEdit->text().toUTF8(),
                       "1", "", "0", "0", "0", "0",
                       (msgform->pidEdit->text().empty() || (!isAdmin))? pId: msgform->pidEdit->text().toUTF8(),
                       0, "0", now.sec );
            if ( req->getErr().getCode() == ERR_OK ) {
                RequestTracker::Instance()->registerRequest( reqptr );
                break;
            }
        } catch ( SMSError& err ) {
            msgform->textEdit->setText( WString::fromUTF8( msgform->textEdit->text().toUTF8() + string("\n\nОшибка:") + err.getDescr() ) );
        }
        msgform->textEdit->setText( WString::fromUTF8( msgform->textEdit->text().toUTF8() + string("\n\nОшибка:") + req->getErr().getDescr() ) );

    }
}

void StatisticsBlock::onSummaryShow() {
    if ( !data.getTotalLines() ) {
        return;
    }

    WDialog summary;
    summary.setWindowTitle( WString::fromUTF8("Итоги") );
    summary.setTitleBarEnabled( true );

    double price;
    int total, delivered, rejected, undelivered, buffered;
    int totalp, deliveredp, rejectedp, undeliveredp, bufferedp;

    WTable report( summary.contents() );
    report.setStyleClass("restable");
    report.elementAt(1, 0)->addWidget( new WLabel( WString::fromUTF8("Всего") ) );
    report.elementAt(2, 0)->addWidget( new WLabel( WString::fromUTF8("Доставлено") ) );
    report.elementAt(3, 0)->addWidget( new WLabel( WString::fromUTF8("Не доставлено") ) );
    report.elementAt(4, 0)->addWidget( new WLabel( WString::fromUTF8("Неверный номер") ) );
    report.elementAt(5, 0)->addWidget( new WLabel( WString::fromUTF8("Отправлено") ) );
    report.elementAt(6, 0)->addWidget( new WLabel( WString::fromUTF8("Общей стоимостью") ) );

    data.evaluateSummary(price, total, totalp, delivered, deliveredp, rejected, rejectedp, undelivered, undeliveredp, buffered, bufferedp );
    char price_str[100];
    sprintf( price_str, "%0.2f", price );
    report.elementAt(0, 1)->addWidget( new WLabel( WString::fromUTF8( "sms" ) ) );
    report.elementAt(1, 1)->addWidget( new WLabel( WString::fromUTF8( boost::lexical_cast<std::string>( total ) ) ) );
    report.elementAt(2, 1)->addWidget( new WLabel( WString::fromUTF8( boost::lexical_cast<std::string>( delivered ) ) ) );
    report.elementAt(3, 1)->addWidget( new WLabel( WString::fromUTF8( boost::lexical_cast<std::string>( undelivered ) ) ) );
    report.elementAt(4, 1)->addWidget( new WLabel( WString::fromUTF8( boost::lexical_cast<std::string>( rejected ) ) ) );
    report.elementAt(5, 1)->addWidget( new WLabel( WString::fromUTF8( boost::lexical_cast<std::string>( buffered ) ) ) );
    report.elementAt(0, 2)->addWidget( new WLabel( WString::fromUTF8( "сообщений" ) ) );
    report.elementAt(1, 2)->addWidget( new WLabel( WString::fromUTF8( boost::lexical_cast<std::string>( totalp ) ) ) );
    report.elementAt(2, 2)->addWidget( new WLabel( WString::fromUTF8( boost::lexical_cast<std::string>( deliveredp ) ) ) );
    report.elementAt(3, 2)->addWidget( new WLabel( WString::fromUTF8( boost::lexical_cast<std::string>( undeliveredp ) ) ) );
    report.elementAt(4, 2)->addWidget( new WLabel( WString::fromUTF8( boost::lexical_cast<std::string>( rejectedp ) ) ) );
    report.elementAt(5, 2)->addWidget( new WLabel( WString::fromUTF8( boost::lexical_cast<std::string>( bufferedp ) ) ) );

    report.elementAt(6, 1)->setColumnSpan( 2 );
    report.elementAt(6, 1)->addWidget( new WLabel( WString::fromUTF8( std::string( price_str ) + " руб" ) ) );

    WPushButton closeBtn( WString::fromUTF8("ОК"), summary.contents() );
    closeBtn.clicked().connect(&summary, &WDialog::accept);

    summary.exec();


}

void StatisticsBlock::onPageUpdate( WSpinBox* page ) {
    page->setRange( 1, statistics->getLastPage() + 1 );
    page->setValue( statistics->getPage() + 1 );
}

void StatisticsBlock::onPageInc( WSpinBox* page ) {
    page->setValue( page->value() + 1 );
    onPageUpdate( page );
}

void StatisticsBlock::onPageDec( WSpinBox* page ) {
    page->setValue( page->value() - 1 );
    onPageUpdate( page );
}

StatisticsBlock::StatisticsBlock( string pId_, bool isAdmin, const Wt::WEnvironment& env_, Wt::WContainerWidget* parent ):
    WContainerWidget( parent ),
    header( this ),
    data( this ),
    footer( this ),
    pId( pId_ ),
    env( env_ )
{
    this->isAdmin = isAdmin;

    statistics = new WScrollTable( header, data, footer );
    statistics->setMinimumSize( WLength( 100, WLength::Percentage), WLength::Auto );
    statistics->setMaximumSize( WLength( 100, WLength::Percentage), WLength::Auto );
    statistics->addStyleClass("restable");
    statistics->buildHeader();
    statistics->buildData();
    statistics->buildFooter();
    addWidget( statistics );
}

void StatisticsBlock::onMoreInfo(const WMouseEvent &e, SMSMessage::ID msgid ) {
    PartnerInfo userid = PartnerManager::get_mutable_instance().findById( pId );
    if ( !userid.ownerId.empty() )
        return;
    WPopupMenu* popup = new WPopupMenu();
    int ts = PartnerManager::get_mutable_instance().findById( pId ).tzone;
    try {
        SMSMessage::HistoryType msg_hist;
        {
            SMSMessage::PTR msg = SMSMessageManager::get_mutable_instance().loadMessage( msgid );
            msg_hist=msg->getHistory();
            std::string item;
            item = utils::ts2datetime( msg->when, ts ) + " << ";
            try {
                item += PartnerManager::get_mutable_instance().findById( msg->pid ).pCName;
            } catch ( ... ) {
                item += "[";
                item += msg->pid;
                item += "]";
            }
            popup->addItem( item );
        }

        for ( SMSMessage::HistoryType::iterator it = msg_hist.begin(); it != msg_hist.end(); it++ ) {
            std::string item;
            item = utils::ts2datetime( it->when, ts ) + " ";

            if ( it->op_direction == 0 ) {
                item += ">> ";
            }

            if ( it->op_direction == 1 ) {
                item += "<< ";
            }

            item += it->gateway + " ";
            if ( it->op_direction == 1 ) {
                item += "[";
                item += SMSMessage::Status::statusDescr( it->op_result );
                item += "]";
            }

            popup->addItem( item );
        }
    } catch ( ... ) {}

    popup->popup( e );
}
