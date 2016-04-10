#ifndef MESSAGECLASSIFIER_H
#define MESSAGECLASSIFIER_H

#include <boost/serialization/singleton.hpp>
#include <boost/tuple/tuple.hpp>

#include "ConfigManager.h"
#include "PGSql.h"

#include <set>
#include <map>

namespace sms {
    class MessageClassifier: public boost::serialization::singleton< MessageClassifier > {
    public:
        struct OperatorInfo {
            std::string mcc;
            std::string mnc;
            std::string opCompany;
            std::string opName;
            std::string opRegion;

            std::string getName() {
                if ( opName.empty() )
                    return opCompany;

                if ( opCompany.empty() )
                    return opName;

                return opName + std::string("(") + opCompany + std::string(")");
            }

            std::string getCode() {
                return mcc + mnc;
            }
        };

        struct CountryInfo {
            std::string mcc;
            std::string cCode;
            std::string cName;
            std::string cPreffix;

            typedef std::map< std::string, OperatorInfo > OperatorMapT;
            OperatorMapT operators;
        };

        typedef std::map< std::string, std::pair< std::string, std::string > > ReplaceT;
        typedef std::multimap< std::string, boost::tuples::tuple< std::string, std::string, std::string > > PreffixMapT;
        typedef std::map< std::string, CountryInfo > CountryOperatorMapT;
        typedef std::map< std::string, std::string > CountryPreffixMccMapT;

        MessageClassifier();

        CountryInfo getMsgClass( std::string phone );
        std::string applyReplace( std::string phone );
        CountryOperatorMapT getCOMap();

    private:
        ReplaceT replaces;
        CountryOperatorMapT comap;
        CountryPreffixMccMapT coprefmap;
        PreffixMapT preffmap;

        void loadReplacesMap();
        void loadCountryOperatorMap();
        void loadRoutingMap();
    };
}

#endif // MESSAGECLASSIFIER_H
