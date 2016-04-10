#include "PersonalPage.h"
#include "PartnerManager.h"

#include <Wt/WTabWidget>

using namespace Wt;

WApplication *createPersonalPage(const WEnvironment& env) {
  /*
   * You could read information from the environment to decide whether
   * the user has permission to start a new application
   */
  return new PersonalPage(env);
}

PersonalPage::PersonalPage( const WEnvironment& env ):
    WApplication( env )
{   
    if ( ( env.hostName() == "vs-sms.ru" ) || ( env.hostName() == "www.vs-sms.ru" ) ) {
        setTitle( WString::fromUTF8("Vistream | SMS рассылка в г.Саратов") );
    } else {
        setTitle( WString::fromUTF8("GreenSMS: Личный кабинет") );
    }

    setCssTheme("polished");
    this->useStyleSheet( "/resources/css/PersonalPage.css" );

    authorized = false;
    isAdmin = false;

    wLoginBox = new LoginBlock( );
    wLoginBox->onLogin().connect( this, &PersonalPage::onLogin );
    root()->addWidget( wLoginBox );

    wStatBlock = NULL;
    wTariffEditor = NULL;
    wPartnerEditor = NULL;
    wRouteEditor = NULL;
}

void PersonalPage::onLogin(string pId, bool isAdmin) {
    this->pId = pId;
    this->isAdmin = isAdmin;

    root()->removeWidget( wLoginBox );
    buildPersonalPage();
}

WContainerWidget* PersonalPage::buildStatisticsBlock( ) {

    if ( wStatBlock ) {
        return wStatBlock;
    }

    wStatBlock = new StatisticsBlock( pId, isAdmin, this->environment() );
    return wStatBlock;
}

WContainerWidget* PersonalPage::buildTariffEditor( ) {
    if ( wTariffEditor ) {
        return wTariffEditor;
    }

    wTariffEditor= new TariffEditor( pId );
    return wTariffEditor;
}

WContainerWidget* PersonalPage::buildRouteEditor( ) {
    if ( wRouteEditor ) {
        return wRouteEditor;
    }

    wRouteEditor= new RouteEditor( pId );
    return wRouteEditor;
}

WContainerWidget* PersonalPage::buildPartnerEditor( ) {
    if ( wPartnerEditor ) {
        return wPartnerEditor;
    }

    wPartnerEditor= new PartnerEditor( pId );
    return wPartnerEditor;
}

void PersonalPage::buildPersonalPage( ) {

    WTabWidget* wt = new WTabWidget();
    wt->addTab( buildStatisticsBlock(), WString::fromUTF8("Статистика") );
    if ( isAdmin ) {
        wt->addTab( buildTariffEditor(), WString::fromUTF8("Редактор тарифов") );
        wt->addTab( buildRouteEditor(), WString::fromUTF8("Редактор маршрутов") );
        wt->addTab( buildPartnerEditor(), WString::fromUTF8("Редактор партнеров") );
    }

    root()->addWidget( wt );
}
