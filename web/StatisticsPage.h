#ifndef STATISTICSPAGE_H
#define STATISTICSPAGE_H

#include <Wt/WApplication>
#include <Wt/Chart/WCartesianChart>
#include <Wt/WContainerWidget>

using namespace Wt;

class StatisticsPage: public WApplication {
public:
    StatisticsPage( const WEnvironment& env );
};

WApplication *createStatisticsPage(const WEnvironment& env);
#endif // STATISTICSPAGE_H
