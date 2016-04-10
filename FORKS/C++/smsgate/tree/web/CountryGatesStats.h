#ifndef COUNTRYGATESSTATS_H
#define COUNTRYGATESSTATS_H

#include <Wt/WApplication>
#include <Wt/WTable>
#include <Wt/WLabel>
#include <Wt/WBreak>
#include <Wt/WPopupMenu>

using namespace Wt;

class CountryGatesStats : public WApplication {
    Wt::WTable* tbl;

    bool popup_shown;
public:
    CountryGatesStats( const WEnvironment& env );
    virtual ~CountryGatesStats();

    void onExpand(int from, int to );
    void onMoreInfo(const WMouseEvent& e, int row, int column );
    void popup_destroyed();
};

WApplication *createGatesStatsPage(const WEnvironment& env);

#endif // COUNTRYGATESSTATS_H
