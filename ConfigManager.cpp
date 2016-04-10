/* 
 * File:   ConfigManager.cpp
 * Author: mohtep
 * 
 * Created on 16 Февраль 2010 г., 13:30
 */

#include "ConfigManager.h"
#include "utils.h"

#include <Wt/WApplication>
#include <iostream>
#include <fstream>

using Wt::WApplication;

ConfigManager* ConfigManager::pInstance_ = 0;

using namespace boost::program_options;
using std::string;
using std::vector;
using boost::any_cast;

ConfigManager::ConfigManager() {
    std::ifstream ifs("options.ini");
    options_description options("options.ini");
    options.add_options()
        ("kannel.server", value<std::string>(), "Kannel server hostname")
        ("kannel.port", value<std::string>(), "Kannel server port")
        ("kannel.scriptname", value<std::string>(), "Kannel server script name")
        ("smsgate.server", value<std::string>(), "SMSGate server hostname")
        ("smsgate.scriptname", value<std::string>(), "SMSGate receive script name")
        ("smsgate.port", value<std::string>(), "SMSGate server port")
        ("smsgate.rcvpass", value<std::string>(), "SMSGate server delivery password")
        ("system.opcodes", value<std::string>(), "Operators-numbers database file")
        ("system.threadsnum", value<std::string>()->default_value("1"), "Number of worker threads")
        ("system.queue.size", value<std::string>()->default_value("1000000"), "System queue max size")
        ("system.cache.size", value<std::string>()->default_value("1000000"), "System cache max size")
        ("system.resendtimeout", value<std::string>()->default_value("120"), "Resend timeout if gates are disabled")
        ("system.acktimeout", value<std::string>()->default_value("120"), "Gate ack timeout")
        ("system.deliverytimeout", value<std::string>()->default_value("300"), "Gate delivery timeout")
        ("system.undeliveredtimeout", value<std::string>()->default_value("86400"), "Message is marked is undelivered")
        ("system.database.connectstring", value<std::string>(), "Database Connect String")
        ("system.database.connections", value<std::string>()->default_value("40"), "Simultaneous connections to DB")
        ("system.database.admconnections", value<std::string>()->default_value("20"), "Simultaneous connections to DB from Admin Page")
        ;
    store(parse_config_file( ifs, options, true ), values );
}

