/*
 * SMSRequest.h
 *
 *  Created on: 22.01.2010
 *      Author: mohtep
 */

#ifndef SMSREQUEST_H_
#define SMSREQUEST_H_

#include <string>
#include <vector>
#include <list>

#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <list>

namespace sms {

    typedef std::vector< std::string > to_vec;

    enum SMSErrorCode {
        ERR_OK,
        ERR_SYNTAX,
        ERR_AUTH,
        ERR_SYSTEM,
        ERR_PARAM,
        ERR_CREDIT
    };

    enum SMSEncoding {
        ENC_CP1251,
        ENC_UTF8
    };

    class SMSError {
        SMSErrorCode code;
        std::string descr;

    public:

        SMSError() : code(ERR_SYSTEM), descr(std::string("SYSTEM ERROR")) {}

        SMSError(SMSErrorCode _code, std::string _descr = std::string("")) : code(_code) {
            switch (code) {
                case ERR_OK:
                    descr = _descr.empty() ? "OK" : "OK: " + _descr;
                    break;
                case ERR_SYNTAX:
                    descr = _descr.empty() ? "SYNTAX ERROR" : "SYNTAX ERROR: " + _descr;
                    break;
                case ERR_AUTH:
                    descr = _descr.empty() ? "AUTH ERROR" : "AUTH ERROR: " + _descr;
                    break;
                case ERR_SYSTEM:
                    descr = _descr.empty() ? "SYSTEM ERROR" : "SYSTEM ERROR: " + _descr;
                    break;
                case ERR_PARAM:
                    descr = _descr.empty() ? "PARAM ERROR" : "PARAM ERROR: " + _descr;
                    break;
                case ERR_CREDIT:
                    descr = _descr.empty() ? "CREDIT ERROR" : "CREDIT ERROR: " + _descr;
                    break;

            }
        };

        const std::string getDescr() const {
            return descr;
        }

        SMSErrorCode getCode() const {
            return code;
        }
    };

    class SMSRequest {
    public:
#if defined(_MSC_VER) || defined(__BORLANDC__)
        typedef unsigned __int64 RequestID;
#else
        typedef unsigned long long ID;
#endif

    private:

        SMSError state;

        SMSError checkUname(const std::string& str) const;
        SMSError checkPass(const std::string& str) const;
        SMSError checkTo(const to_vec& to) const;
        SMSError checkMsg(const std::string& str) const;
        SMSError checkTid(const std::string& str) const;
        SMSError checkFrom(const std::string& str) const;
        SMSError checkUtf(const std::string& str) const;
        SMSError checkSubPref(const std::string& str) const;
        SMSError checkHex(const std::string& str, const std::string& str2) const;
        SMSError checkUdh(const std::string& str) const;
        SMSError checkDelay(const std::string& str) const;
        SMSError checkDlr(const std::string& str) const;
        SMSError checkPid(const std::string& str) const;
        SMSError checkPriority(const std::string& str) const;
        SMSError checkGarant(const std::string& str) const;
        
        static SMSRequest::ID genID();
        static boost::mutex lock;
        static std::list< SMSRequest::ID > id_cache;


        bool checkStr(const std::string& str, const std::string& mask, bool csense = false) const;


    public:
        typedef boost::shared_ptr< SMSRequest > PTR;

        SMSRequest() {};

        SMSRequest(const std::string _uname, const std::string _pass, const to_vec _to, const std::string _msg,
                const std::string _tid,
                const std::string _from,
                const std::string _utf,
                const std::string _subpref,
                const std::string _hex,
                const std::string _udh,
                const std::string _delay,
                const std::string _dlr,
                const std::string _pid,
                const int _priority,
                const std::string _garant,
                const int _when
                    ) {
            parse(_uname, _pass, _to, _msg,
                    _tid,
                    _from,
                    _utf,
                    _subpref,
                    _hex,
                    _udh,
                    _delay,
                    _dlr,
                    _pid,
                    _priority,
                    _garant,
                    _when );
                    }

        void parse(const std::string _uname, const std::string _pass, const to_vec _to, const std::string _msg,
                const std::string _tid,
                const std::string _from,
                const std::string _utf,
                const std::string _subpref,
                const std::string _hex,
                const std::string _udh,
                const std::string _delay,
                const std::string _dlr,
                const std::string _pid,
                const int _priority,
                const std::string _garant,
                const int _when
                    );

        const SMSError& getErr() const {
            return state;
        }

        SMSError checkAuth(const std::string& _uname, const std::string& _pass) const;

        void parseRequest(const std::string req) const;
        std::string genReport();
        std::string genReportSMST();

        void setID(SMSRequest::ID _id);

        SMSRequest::ID getID() {
            return id;
        }

        void setState(SMSErrorCode c, std::string d) {
            state = SMSError(c, d);
        }

        const std::string genRequestURL(std::string to, int mid) const;
        const std::list< std::string > genRequestURLs() const;

        std::string uname;
        std::string pass;
        to_vec to;
        to_vec to_orig;
        std::string msg;

        std::string tid;
        std::string from;
        std::string utf;
        std::string subpref;
        std::string hex;
        std::string udh;
        std::string delay;
        std::string dlr;
        std::string pid;
        time_t when;
        int priority;
        std::string garant;
        int parts;
        SMSRequest::ID id;

    };

}
#endif /* SMSREQUEST_H_ */
