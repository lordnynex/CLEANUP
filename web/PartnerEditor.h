#ifndef PARTNEREDITOR_H
#define PARTNEREDITOR_H

#include <Wt/WContainerWidget>
#include <Wt/WTable>
#include <Wt/WLabel>
#include <Wt/WInPlaceEdit>
#include <Wt/WLineEdit>
#include <Wt/WCheckBox>
#include <Wt/WTreeView>
#include <Wt/WStandardItemModel>
#include <Wt/WBorderLayout>
#include <Wt/WHBoxLayout>
#include <Wt/WComboBox>
#include <Wt/WSpinBox>



class WCustomInPlaceEdit: public Wt::WInPlaceEdit {
public:
    WCustomInPlaceEdit( Wt::WString ss, Wt::WString es, Wt::WContainerWidget* parent = 0 ): Wt::WInPlaceEdit( ss, parent ) {
        setEmptyText( es );
        setButtonsEnabled( false );
//        setMaximumSize( Wt::WLength( 80, Wt::WLength::Percentage ), Wt::WLength::Auto );
        if ( ss.toUTF8().empty() ) {
            setText( Wt::WString::Empty );
        }
    }
};

class PartnerOptions: public Wt::WContainerWidget {
public:
    PartnerOptions( std::string _userid, std::string pid, Wt::WContainerWidget* parent = 0 );

    Wt::Signal< std::string, bool >& updated() { return updated_; }
private:
    std::string pid;
    std::string userid;
    bool isPersonalInfoVisible;
    Wt::Signal< std::string, bool > updated_;

    Wt::WTable* tbl;
    Wt::WLabel* pName;
    Wt::WInPlaceEdit* pNameEditor;
    Wt::WText* pExpand;
    WCustomInPlaceEdit* pCNameEdit;
    WCustomInPlaceEdit* pLastNameEdit;
    WCustomInPlaceEdit* pFirstNameEdit;
    WCustomInPlaceEdit* pMiddleNameEdit;
    WCustomInPlaceEdit* pEmailEdit;
    WCustomInPlaceEdit* pOwnerPhoneEdit;
    WCustomInPlaceEdit* pPhoneEdit;
    WCustomInPlaceEdit* pCompanyEdit;
    WCustomInPlaceEdit* pCAddressEdit;
    WCustomInPlaceEdit* pTimeZoneEdit;
    WCustomInPlaceEdit* pLoginEdit;
    WCustomInPlaceEdit* pPassEdit;
    WCustomInPlaceEdit* pAdminPassEdit;
    WCustomInPlaceEdit* pManagerEdit;
    WCustomInPlaceEdit* pTariffEdit;
    Wt::WComboBox* pTrialEdit;
    Wt::WSpinBox* pPriorityEdit;
    Wt::WComboBox* pPostEdit;
    Wt::WSpinBox* pBalanceEdit;
    Wt::WSpinBox* pCreditEdit;
    Wt::WSpinBox* pLimitEdit;

    void onPersonalShowHide();
    void onSuggestionActivated( Wt::WSuggestionPopup* sugg, int index, WCustomInPlaceEdit* widget );
    void onBtnSave();
};

class PartnerEditor: public Wt::WContainerWidget {
public:
    PartnerEditor( std::string _userid, Wt::WContainerWidget* parent = 0 );

private:
    Wt::WStandardItemModel* model_;
    Wt::WTreeView* treeView_;
    Wt::WBorderLayout* root;
    Wt::WHBoxLayout* opts;

    std::vector< int > columns_width;
    int elements_per_page;
    std::string userid;

    Wt::WTreeView* buildTreeView( Wt::WStandardItemModel * model );
    void resizeTreeView( Wt::WTreeView* tw);
    void buildModel( Wt::WStandardItemModel* data );
    void updateModel( Wt::WStandardItemModel* data );
    void onChangeRoot();
    void onPartnersUpdated( std::string pid, bool );
};

#endif // PARTNEREDITOR_H
