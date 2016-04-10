#ifndef STATMANAGER_H
#define STATMANAGER_H

#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>

#include <boost/serialization/nvp.hpp>

#include "SMSMessage.h"
#include "PGSql.h"
#include "Timer.h"

using std::string;
using namespace boost::multi_index;

namespace sms {
    struct SMPPGateProperties {
        bool available;
        int requests;
        int acks;
        int responses;
        int deliveres;
        int points;
        long minute1_d;
        long minute5_d;
        long minute15_d;
        double deliverytime;

        template<class Archive>
            void serialize(Archive & ar, const unsigned int) {
                ar & BOOST_SERIALIZATION_NVP(requests);
                ar & BOOST_SERIALIZATION_NVP(acks);
                ar & BOOST_SERIALIZATION_NVP(responses);
                ar & BOOST_SERIALIZATION_NVP(deliveres);
            }
    };

    struct CountryInfo {
        string cname;
        string ccode;
        string region;

        string opcode;
        string opname;

        string gname;
        int requests;
        int acks;
        int deliveres;

        template<class Archive>
            void serialize(Archive & ar, const unsigned int) {
                ar & BOOST_SERIALIZATION_NVP(cname);
                ar & BOOST_SERIALIZATION_NVP(ccode);
                ar & BOOST_SERIALIZATION_NVP(region);
                ar & BOOST_SERIALIZATION_NVP(opcode);
                ar & BOOST_SERIALIZATION_NVP(opname);
                ar & BOOST_SERIALIZATION_NVP(gname);
                ar & BOOST_SERIALIZATION_NVP(requests);
                ar & BOOST_SERIALIZATION_NVP(acks);
                ar & BOOST_SERIALIZATION_NVP(deliveres);
            }
    };

    class StatManager {
    public:
        typedef std::map< std::string, SMPPGateProperties > gNamePropMap;
        typedef std::vector< std::vector< CountryInfo > > TCountryInfoTable;
        StatManager();
        StatManager( const StatManager& orig );
        ~StatManager();

        gNamePropMap onUpdateFromToSMPPGate( long from, long to );
        gNamePropMap onUpdateFromToSMPPGateCountry( string cname, string opname );
        gNamePropMap onUpdateDeltaSMPPGate( long delta );

        void on1SecondUpdateSMPPGate();
        void on1MinuteUpdateSMPPGate();
        void on5MinuteUpdateSMPPGate();
        void on1HourUpdateSMPPGate();
        void on1DayUpdateSMPPGate();

        void onCountryInfoUpdate();
        void onStore();
        void onLoad();

//        void feedback( SMSMessage::ID msgid, SMSMessage::HistoryElement el );

        gNamePropMap get1SecondStatsSMPPGate( );
        gNamePropMap get1MinuteStatsSMPPGate( );
        gNamePropMap get5MinuteStatsSMPPGate( );
        gNamePropMap get1HourStatsSMPPGate( );
        gNamePropMap get1DayStatsSMPPGate( );

        std::string getUptime();

        TCountryInfoTable getCountryInfoLastUpdate( );

        static StatManager* Instance() {
            if (!pInstance_)
                pInstance_ = new StatManager;
            return pInstance_;
        }

        template<class Archive>
            void serialize(Archive & ar, const unsigned int version)
            {
                ar & BOOST_SERIALIZATION_NVP(on1SecondStatsSMPPGate);
                ar & BOOST_SERIALIZATION_NVP(on1MinuteStatsSMPPGate);
                ar & BOOST_SERIALIZATION_NVP(on5MinuteStatsSMPPGate);
                ar & BOOST_SERIALIZATION_NVP(on1HourStatsSMPPGate);
                ar & BOOST_SERIALIZATION_NVP(on1DayStatsSMPPGate);

                ar & BOOST_SERIALIZATION_NVP(countryInfoLastUpdate);

            }

private:
        static StatManager* pInstance_;

        gNamePropMap on1SecondStatsSMPPGate;
        gNamePropMap on1MinuteStatsSMPPGate;
        gNamePropMap on5MinuteStatsSMPPGate;
        gNamePropMap on1HourStatsSMPPGate;
        gNamePropMap on1DayStatsSMPPGate;

        TCountryInfoTable countryInfoLastUpdate;

        int on1SecondHandlerSMPPGate;
        int on1MinuteHandlerSMPPGate;
        int on5MinuteHandlerSMPPGate;
        int on1HourHandlerSMPPGate;
        int on1DayHandlerSMPPGate;
        int onCountryInfoHandler;
        int onStoreHandler;
        boost::xtime uptime;

        PGSql& db;
        boost::recursive_mutex lock;
    };

}

#endif // STATMANAGER_H
