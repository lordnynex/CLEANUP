#include "StatisticsPage.h"

WApplication *createStatisticsPage(const WEnvironment& env) {
  /*
   * You could read information from the environment to decide whether
   * the user has permission to start a new application
   */
  return new StatisticsPage(env);
}

StatisticsPage::StatisticsPage( const WEnvironment& env ): WApplication( env ) {
    setTitle( "SMSGate statistics page" );

    Chart::WCartesianChart* chart = new Chart::WCartesianChart();
    chart->axis(Chart::Y2Axis).setVisible( true );
    chart->axis(Chart::Y2Axis).setTitle( "SMSGate Statistics Chart" );
    chart->setBarMargin( 2 );
    //chart->
    root()->addWidget( chart );
}
