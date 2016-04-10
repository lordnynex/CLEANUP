#ifndef STATISTICSBLOCK_H
#define STATISTICSBLOCK_H

#include <vector>
#include <string>

#include <Wt/WEnvironment>
#include <Wt/WContainerWidget>
#include <Wt/WTable>
#include <Wt/WLabel>
#include <Wt/WLineEdit>
#include <Wt/WDatePicker>
#include <Wt/WCalendar>
#include <Wt/WComboBox>
#include <Wt/WSpinBox>
#include <Wt/WPushButton>
#include <Wt/WCheckBox>

#include "WScrollTable.h"
#include "DataSource.h"
#include "SMSMessage.h"

class StatisticsBlock;

class WStatPageHeader: public WDataSource< std::vector< Wt::WWidget* > > {
public:
    typedef std::vector< Wt::WWidget* > Row;
    typedef WDataSource< std::vector< Wt::WWidget* > >::RowList RowList;

    WStatPageHeader( StatisticsBlock* _ppage );

    virtual int getTotalLines();
private:
    virtual void execute( int lnl, int lnr, RowList &data );
    StatisticsBlock* ppage;
};

class WStatPageFooter: public WDataSource< std::vector< Wt::WWidget* > > {
public:
    typedef std::vector< Wt::WWidget* > Row;
    typedef WDataSource< std::vector< Wt::WWidget* > >::RowList RowList;

    WStatPageFooter( StatisticsBlock* _ppage ) { ppage = _ppage; }

    virtual int getTotalLines() { return 0; }
private:
    virtual void execute( int lnl, int lnr, RowList &data ) { }

    StatisticsBlock* ppage;
};

class WStatPageData: public WDataSource< std::vector< Wt::WWidget* > > {
public:
    typedef std::vector< Wt::WWidget* > Row;
    typedef WDataSource< std::vector< Wt::WWidget* > >::RowList RowList;

    WStatPageData( StatisticsBlock* _ppage );
    ~WStatPageData( );

    void prepareRequest( );
    void resetFilter( );
    void setPidFilter( std::string pid );
    void setPhoneFilter( std::string phone );
    void setDateFromFilter( long date_from );
    void setDateToFilter( long date_to );
    void setTextFilter( std::string text );
    void setCountryFilter( std::string country );
    void setStatusFilter( SMSMessage::Status status );
    void setModifiedSearchOrder( );

    void evaluateSummary( 	double &price, 
				int &total, int &totalp,
				int &delivered, int &deliveredp,
				int &rejected, int &rejectedp,
				int &undelivered, int &undeliveredp,
				int &buffered, int &bufferedp );

    virtual int getTotalLines();
private:
    virtual void execute( int lnl, int lnr, RowList &data );

    StatisticsBlock* ppage;
    bool initialized;
    bool pid_filter; std::string pid_value;
    bool phone_filter; std::string phone_value;
    bool date_from_filter; long date_from_value;
    bool date_to_filter; long date_to_value;
    bool text_filter; std::string text_value;
    bool country_filter; std::string country_value;
    bool status_filter; SMSMessage::Status status_value;
    bool sort_by_updated;

    std::string view_name;
    std::string res_name;
    int __total_lines;
};

class StatisticsBlock: public Wt::WContainerWidget {
public:
    struct RowInfo {
        Wt::WLineEdit* pid;
        Wt::WLineEdit* phone;
        Wt::WDatePicker* date_from, *date_to;
        Wt::WLineEdit* text;
        Wt::WComboBox* status;
        Wt::WLineEdit* country;
        Wt::WPushButton* reportbtn;
        Wt::WSpinBox* page;
        Wt::WLabel* report_status;
        Wt::WCheckBox* auto_refresh;
        Wt::WCheckBox* update_order;
    };

    StatisticsBlock( std::string pId, bool isAdmin, const Wt::WEnvironment& env, Wt::WContainerWidget* parent = 0 );
private:
    static PGSql& db;

    WScrollTable* statistics;
    bool isAdmin;
    std::string pId;
    const Wt::WEnvironment env;
    WStatPageHeader header;
    WStatPageData data;
    WStatPageFooter footer;

    void onMoreInfo(const Wt::WMouseEvent& e, SMSMessage::ID msgid );
    void onSummaryShow();
    void onNewMessage();
    void onPageUpdate( Wt::WSpinBox* page );
    void onPageInc( Wt::WSpinBox* page );
    void onPageDec( Wt::WSpinBox* page );
    void onReportBtnClicked( RowInfo info );

    friend class WStatPageHeader;
    friend class WStatPageData;
    friend class WStatPageFooter;
};

#endif // STATISTICSBLOCK_H
