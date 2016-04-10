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
        // Kannel
        ("kannel.server", value<std::string>(), "Kannel server hostname")
        ("kannel.port", value<std::string>(), "Kannel server port")
        ("kannel.scriptname", value<std::string>(), "Kannel server script name")
        // SMSGate
        ("smsgate.server", value<std::string>(), "SMSGate server hostname")
        ("smsgate.scriptname", value<std::string>(), "SMSGate receive script name")
        ("smsgate.port", value<std::string>(), "SMSGate server port")
        ("smsgate.rcvpass", value<std::string>(), "SMSGate server delivery password")
        ("system.opcodes", value<std::string>(), "Operators-numbers database file")
        ("system.replacesmap", value<std::string>(), "Operators-numbers2number replaces map")
        // Logging
        ("system.dblog", value<std::string>(), "System DB Log file")
        ("system.msglog", value<std::string>(), "System Messages Log file")
        ("system.httplog", value<std::string>(), "System Http Log file")
        // System threads
        ("system.threadsnum", value<std::string>()->default_value("1"), "Number of worker threads")
        // System queue
        ("system.queue.size", value<std::string>()->default_value("1000000"), "System queue max size")
        ("system.cache.size", value<std::string>()->default_value("1000000"), "System cache max size")
        // System timeout
        ("system.resendtimeout", value<std::string>()->default_value("120"), "Resend timeout if gates are disabled")
        ("system.acktimeout", value<std::string>()->default_value("120"), "Gate ack timeout")
        ("system.deliverytimeout", value<std::string>()->default_value("300"), "Gate delivery timeout")
        ("system.undeliveredtimeout", value<std::string>()->default_value("86400"), "Message is marked is undelivered")
        // Database
        ("system.database.connectstring", value<std::string>(), "Database Connect String")
        ("system.database.connections", value<std::string>()->default_value("40"), "Simultaneous connections to DB")
        ("system.database.admconnections", value<std::string>()->default_value("20"), "Simultaneous connections to DB from Admin Page")
        ;
    store(parse_config_file( ifs, options, true ), values );
}

