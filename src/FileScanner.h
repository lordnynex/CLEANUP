/*
 * FileScanner.h
 *
 *  Created on: Apr 12, 2015
 *      Author: oozie
 */

#ifndef FILESCANNER_H_
#define FILESCANNER_H_

#include <sys/inotify.h>
#include <libgen.h>
#include <unistd.h>
#include <string.h>

#include <thread>
#include <chrono>
#include <string>
#include <iostream>
#include <fstream>
#include <glog/logging.h>
#include <json/json.h>
#include <errno.h>
#include <regex>

#include "MetricCounter.h"
// TODO: inherit from a scanner interface.

struct LogPattern {
    LogPattern(std::string name, std::regex regex, MetricCounter counter)
        :
        name(name),
        regex(regex),
        counter(counter) { };
    std::string name;
    std::regex regex;
    MetricCounter counter;
};

class FileScanner {

public:
    FileScanner(std::string filename);

    void scan();
    void setPatterns(std::vector<LogPattern>);
private:
    std::string filename_;
    char *dirname_;
    int filewatch_qid_ = -1;
    int dirwatch_qid_ = -1;
    int file_ret_ = -1;

    bool shutdown_ = false;
    void handleCreateEvent();
    void reopenStream();
    std::thread reopenStreamTask_;

    std::ifstream logfile_;

    std::vector<LogPattern> patterns_;
};

#endif /* FILESCANNER_H_ */
