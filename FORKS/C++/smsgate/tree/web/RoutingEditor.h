#ifndef ROUTINGEDITOR_H
#define ROUTINGEDITOR_H

#include "MessageClassifier.h"
#include "Route.h"
#include "RouteOptionEditor.h"

#include <Wt/WStandardItemModel>
#include <Wt/WStandardItem>
#include <Wt/WBorderLayout>
#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>
#include <Wt/WGridLayout>
#include <Wt/WContainerWidget>
#include <Wt/WComboBox>
#include <Wt/WCheckBox>
#include <Wt/WLabel>
#include <Wt/WDialog>
#include <Wt/WTable>
#include <Wt/WPushButton>
#include <Wt/WLineEdit>
#include <Wt/WSpinBox>
#include <Wt/WDoubleValidator>
#include <Wt/WIntValidator>
#include <Wt/WApplication>
#include <Wt/WFileUpload>
#include <Wt/WProgressBar>
#include <Wt/WTreeView>
#include <Wt/WFileResource>
#include <Wt/WTextArea>

#include <Wt/WGroupBox>

#include <vector>
#include <string>
#include <ostream>

typedef roe::RouteOptionEditor< Route::RouteResendOptions, roe::MultiValueAdaptor< Route::RouteResendOptions > > RecendEditor;
typedef roe::RouteOptionEditor< Route::RouteFirstGate, roe::MultiChoiseAdaptor< Route::RouteFirstGate> > FirstgwEditor;
typedef roe::RouteOptionEditor< Route::RouteSecondGate, roe::MultiChoiseAdaptor< Route::RouteSecondGate> > SecondgwEditor;
typedef roe::RouteOptionEditor< Route::RouteThirdGate, roe::MultiChoiseAdaptor< Route::RouteThirdGate> > ThirdgwEditor;

typedef roe::RouteOptionEditor< Route::FirstGwSlippage, roe::MultiChoiseAdaptor< Route::FirstGwSlippage> > FirstGwSlippageEditor;
typedef roe::RouteOptionEditor< Route::SecondGwSlippage, roe::MultiChoiseAdaptor< Route::SecondGwSlippage> > SecondGwSlippageEditor;

typedef roe::RouteOptionEditor< Route::UserRouteFirstGate, roe::MultiChoiseAdaptor< Route::UserRouteFirstGate> > UserFirstgwEditor;

class RouteEditor: public Wt::WContainerWidget {
public:
    RouteEditor( std::string _userid, WContainerWidget* parent = 0 );
private:
    Wt::WStandardItemModel* model_;
    Wt::WTreeView* treeView_;
    Wt::WComboBox* tlistBox;
    Wt::WLineEdit* nameBox;

    RecendEditor* recendEditor;
    FirstgwEditor* firstgwEditor;
    SecondgwEditor* secondgwEditor;
    ThirdgwEditor* thirdgwEditor;
    FirstGwSlippageEditor* firstSlippageEditor;
    SecondGwSlippageEditor* secondSlippageEditor;
    UserFirstgwEditor* ufirstgwEditor;
    std::string userid;

    std::vector< int > columns_width;
    int elements_per_page;
    Route route;

    void buildModel( Wt::WStandardItemModel*, Route& route, bool clear = true );
    Wt::WTreeView* buildTreeView( Wt::WStandardItemModel* );
    void resizeTreeView( Wt::WTreeView* );

    void changeItemText( Wt::WModelIndex, Wt::WSpinBox* );
    void changeItemTextRecursive( Wt::WModelIndex, int column, Wt::WSpinBox* );

    void onRouteLoad();
    void onRouteRemove();
    void onRouteSave();
    void onRouteClear();
    void onRouteUpdate();
    void tlistRebuild();

    void onChangeRoot();

    std::string sdouble2string( std::string v, std::string def_val = "" );
    double sdouble2double( std::string v, double defval );
    std::string double2string( double v );
};

#endif // ROUTINGEDITOR_H
