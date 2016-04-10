/* 
 * File:   ConfigManager.h
 * Author: mohtep
 *
 * Created on 16 Февраль 2010 г., 13:30
 */

#ifndef _CONFIGMANAGER_H
#define	_CONFIGMANAGER_H

#include <map>
#include <string>
#include <iostream>

#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>

#include "utils.h"
#include "Error.h"

using namespace sms;

class ConfigManager {
public:
    ConfigManager();
    template < typename K>
    K getProperty( std::string req) {
        try {
            return boost::lexical_cast< K >(boost::any_cast< std::string >( values[ req ].value() ));
        } catch ( ... ) {
            std::cerr << "Error while loading config param ["  << req  << "]"  << std::endl;
            BOOST_THROW_EXCEPTION( FailureError()
                                   << throw_descr( std::string( std::string( "Error while loading config param [" ) + req + "]" ).c_str() ));
        }
    }


    static ConfigManager* Instance() {
        if (!pInstance_)
            pInstance_ = new ConfigManager;
        return pInstance_;
    }

private:
    static ConfigManager* pInstance_;
    boost::program_options::variables_map values;
};

#endif	/* _CONFIGMANAGER_H */

