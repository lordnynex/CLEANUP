/*
 * FileScanner.cpp
 *
 *  Created on: Apr 12, 2015
 *      Author: oozie
 */

#include "FileScanner.h"

#define MAX_EVENTS 4
#define LEN_NAME 0x100
#define EVENT_SIZE     ( sizeof (struct inotify_event) )
#define IEVENT_BUF_LEN ( MAX_EVENTS * ( EVENT_SIZE + LEN_NAME ))

FileScanner::FileScanner(std::string filename) :
    filename_(filename) {
    dirname_ = dirname(strdup(filename_.c_str()));
}

void FileScanner::handleCreateEvent() {
    char event_buffer[IEVENT_BUF_LEN];

    LOG(INFO) << "watching dir " << dirname_;
    while(!shutdown_) {
        if (read(dirwatch_qid_, event_buffer, IEVENT_BUF_LEN)) {
            reopenStream();
            LOG(INFO) << filename_ << " recreated";
            inotify_rm_watch(filewatch_qid_, file_ret_);
            file_ret_ = inotify_add_watch(filewatch_qid_, filename_.c_str(), IN_MODIFY);
            // TODO: only reopen if it's the monitored filename
        }
    }
}

int add_watch_handle_errors(int watch_qid, std::string fsobj, int event_mask, bool is_directory) {
    int ret = inotify_add_watch(watch_qid, fsobj.c_str(), event_mask);
    if (ret == -1) {
        // It's okay if we're still waiting for a file, die in any other case.
        if (!(errno == ENOENT && !is_directory)) {
            LOG(FATAL) << strerror(errno) << ": " << fsobj;
        }
    }
    return ret;
}

void FileScanner::scan() {
    char event_buffer[IEVENT_BUF_LEN];
    std::string logline;

    // Allocate watch queues.
    filewatch_qid_ = inotify_init();
    dirwatch_qid_ = inotify_init();

    add_watch_handle_errors(dirwatch_qid_, dirname_, IN_CREATE, true);
    file_ret_ = add_watch_handle_errors(filewatch_qid_, filename_, IN_MODIFY, false);

    // Open the file.
    reopenStream();
    reopenStreamTask_ = std::thread(&FileScanner::handleCreateEvent, this);

    // TODO: Scan and populate a rate counter.
    LOG(INFO) << "scanning " << filename_;
    int curpos = -1;
    while(!shutdown_) {
        // TODO: read multiple lines per update.
        read(filewatch_qid_, event_buffer, IEVENT_BUF_LEN);
        getline(logfile_, logline);
        curpos = logfile_.tellg();
        if (curpos == -1) { // File truncated.
            // TODO: Don't drop first line after truncation.
            reopenStream();
        } else {
            LOG(INFO) << "CUR: " << curpos << " "
                      << "LOGLINE: "<<  logline;
        }
    };
    reopenStreamTask_.join();
}

void FileScanner::reopenStream() {
    if (logfile_.is_open()) {
        logfile_.close();
    }
    logfile_.open(filename_.c_str(), std::ios::in);
    logfile_.seekg(0, logfile_.end);
}

void FileScanner::setPatterns(std::vector<LogPattern> patterns) {
    patterns_ = patterns;
}
