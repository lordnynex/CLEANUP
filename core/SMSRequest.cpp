/*
 * SMSRequest.cpp
 *
 *  Created on: 22.01.2010
 *      Author: mohtep
 */


#include <cctype>
#include <sstream>

#include "SMSRequest.h"
#include "utils.h"
#include "ConfigManager.h"
#include "PartnerManager.h"
#include "MessageClassifier.h"
#include <boost/thread/xtime.hpp>

namespace sms {

    using namespace utils;

    std::list< SMSRequest::ID > SMSRequest::id_cache;
    boost::mutex SMSRequest::lock;

    bool SMSRequest::checkStr(const std::string& str, const std::string& mask, bool csense) const {
        const char* strdata = str.c_str();
        int datalen = str.length();

        const char* maskdata = mask.c_str();
        int masklen = mask.length();

        for (int i = 0; i < datalen; i++) {
            char c = strdata[i];
            bool found = false;

            for (int j = 0; j < masklen; j++) {
                char k = maskdata[j];
                if ((c == k) || (csense && (tolower(c) == tolower(k)))) {
                    found = true;
                    break;
                }
            }

            if (!found) return false;

        }

        return true;
    }

    SMSError SMSRequest::checkUname(const std::string& str) const {
        char allowed[] = "abcdefghijklmnopqrstuvwxyz0123456789_";

        if (str.length() == 0) {
            return SMSError(ERR_PARAM, "Empty user name");
        }

        if (str.length() > 64) {
            return SMSError(ERR_PARAM, "User name is loo long (max 64)");
        }

        if (!checkStr(str, allowed, true)) {
            return SMSError(ERR_SYNTAX, "Illegal username");
        }

        return SMSError(ERR_OK);
    }

    SMSError SMSRequest::checkPass(const std::string& str) const {
        char allowed[] = "abcdefghijklmnopqrstuvwxyz0123456789_~()*@+";

        if (str.length() == 0) {
            return SMSError(ERR_PARAM, "Empty password");
        }

        if (str.length() > 64) {
            return SMSError(ERR_PARAM, "Password is too long (max 64)");
        }

        if (!checkStr(str, allowed, true)) {
            return SMSError(ERR_SYNTAX, "Illegal password");
        }

        return SMSError(ERR_OK);
    }

    SMSError SMSRequest::checkTo(const to_vec& to) const {
        char allowed[] = "0123456789";

        if (to.size() == 0) {
            return SMSError(ERR_PARAM, "Empty numbers list");
        }

        if (to.size() > 200) {
            return SMSError(ERR_PARAM, "Too much numbers in a list (max 200)");
        }

        to_vec::const_iterator it;
        for (it = to.begin(); it != to.end(); it++) {
            if (!checkStr(*it, allowed)) {
                return SMSError(ERR_SYNTAX, "Illegal number: " + *it);
            }
            if (it->length() > 22) {
                return SMSError(ERR_SYNTAX, "Too long number: " + *it + " (max 22)");
            }
        }

        return SMSError(ERR_OK);
    }

    SMSError SMSRequest::checkMsg(const std::string& str) const {
        if (str.length() == 0) {
            return SMSError(ERR_PARAM, "Empty message");
        }

        if ( hex == "1") {
            char allowed[] = "abcdef0123456789";

            if (str.length() > 64) {
                return SMSError(ERR_PARAM, "Too long message for binary sms ( max 320 characters )");
            }

            if (!checkStr(str, allowed, true)) {
                return SMSError(ERR_SYNTAX, "Illegal letter for binary sms");
            }

        }

        if (str.length() > 4096) {
            return SMSError(ERR_PARAM, "Too long message (max 2048)");
        }

        return SMSError(ERR_OK);
    }

    SMSError SMSRequest::checkTid(const std::string& str) const {
        char allowed[] = "abcdefghijklmnopqrstuvwxyz0123456789.";

        if (str.length() > 64) {
            return SMSError(ERR_PARAM, "Too long tid (max 64)");
        }

        if (!checkStr(str, allowed, true)) {
            return SMSError(ERR_SYNTAX, "Illegal tid");
        }

        return SMSError(ERR_OK);
    }

    SMSError SMSRequest::checkFrom(const std::string& str) const {
        char allowed[] = "abcdefghijklmnopqrstuvwxyz0123456789~!@#$%^&*-()_+|#$^\"\'.:;<>= ";

        if (str.length() > 11) {
            return SMSError(ERR_PARAM, "Too long From (max 11)");
        }

        if (!checkStr(str, allowed, true)) {
            return SMSError(ERR_SYNTAX, "Illegal From");
        }

        return SMSError(ERR_OK);
    }

    SMSError SMSRequest::checkUtf(const std::string& str) const {
        if (str.empty())
            return SMSError(ERR_OK);

        if (str == "0")
            return SMSError(ERR_OK);

        if (!(str == "1"))
            return SMSError(ERR_PARAM, string("Utf field <") + str + "> can contain \"1\" or nothing");

        return SMSError(ERR_OK);
    }

    SMSError SMSRequest::checkSubPref(const std::string& str) const {
        char allowed[] = "abcdefghijklmnopqrstuvwxyz0123456789";

        if (str.length() > 64) {
            return SMSError(ERR_PARAM, "Too long Subpref (max 64)");
        }

        if (!checkStr(str, allowed, true)) {
            return SMSError(ERR_SYNTAX, "Illegal Subpref");
        }

        return SMSError(ERR_OK);
    }

    SMSError SMSRequest::checkHex(const std::string& str, const std::string& str2) const {
        if (str.empty())
            return SMSError(ERR_OK);

        if (str == "0")
            return SMSError(ERR_OK);


        if (!(str == "1"))
            return SMSError(ERR_PARAM, string("Hex field <") + str + "> can contain \"1\" or nothing");

        char allowed[] = "abcdef0123456789";

        if (str2.length() > 320) {
            return SMSError(ERR_PARAM, "Binary text too long (max 320)");
        }

        if (!checkStr(str2, allowed, true)) {
            return SMSError(ERR_SYNTAX, "Illegal binary text");
        }


        return SMSError(ERR_OK);
    }

    SMSError SMSRequest::checkUdh(const std::string& str) const {
        char allowed[] = "abcdefgh0123456789";

        if (str == "0")
            return SMSError(ERR_OK);

        if (str.length() > 14) {
            return SMSError(ERR_PARAM, "Too long Udh (max 11)");
        }

//        if (!checkStr(str, allowed, true)) {
//            return SMSError(ERR_SYNTAX, "Illegal UDH");
//        }

        return SMSError(ERR_OK);
    }

    SMSError SMSRequest::checkDelay(const std::string& str) const {
        char allowed[] = "0123456789";

        if (str == "0")
            return SMSError(ERR_OK);

        if (str.length() > 8) {
            return SMSError(ERR_PARAM, "Too long Delay field (max 8)");
        }

        if (!checkStr(str, allowed, true)) {
            return SMSError(ERR_SYNTAX, "Illegal Delay");
        }

        return SMSError(ERR_OK);
    }

    SMSError SMSRequest::checkDlr(const std::string& str) const {
        if (str == "0")
            return SMSError(ERR_OK);

        if (!(str.empty() || (str == "1")))
            return SMSError(ERR_PARAM, "Dlr field can contain \"1\" or nothing");

        return SMSError(ERR_OK);
    }

    SMSError SMSRequest::checkPid(const std::string& str) const {
        char allowed[] = "0123456789";

        if (str.length() > 16) {
            return SMSError(ERR_PARAM, "Too long PID field (max 4)");
        }

        if (!checkStr(str, allowed, true)) {
            return SMSError(ERR_SYNTAX, "Illegal PID");
        }

        return SMSError(ERR_OK);
    }

    SMSError SMSRequest::checkPriority(const std::string& str) const {
        char allowed[] = "0123456789";

        if (str == "0")
            return SMSError(ERR_OK);

        if (str.length() > 2) {
            return SMSError(ERR_PARAM, "Too long Priority field (max 4)");
        }

        if (!checkStr(str, allowed, true)) {
            return SMSError(ERR_SYNTAX, "Illegal Priority");
        }

        return SMSError(ERR_OK);
    }

    SMSError SMSRequest::checkGarant(const std::string& str) const {
        if (str == "0")
            return SMSError(ERR_OK);

        if (!(str.empty() || (str == "1")))
            return SMSError(ERR_PARAM, "Dlr field can contain \"1\" or nothing");

        return SMSError(ERR_OK);
    }

    SMSError SMSRequest::checkAuth(const std::string& _uname, const std::string& _pass) const {
        try {
            PartnerInfo ptrn = PartnerManager::get_mutable_instance().findByName( _uname );
            if ( _pass == ptrn.pPass ) {
                return SMSError(ERR_OK);
            }
        } catch ( PartnerNotFoundError& e ) {}
        return SMSError(ERR_AUTH, "Incorrect username or password");
    }

    void SMSRequest::parse(const std::string _uname, const std::string _pass, to_vec _to, const std::string _msg,
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

        state = checkUname(_uname);
        if (state.getCode() != ERR_OK) {
            return;
        }
        uname = _uname;

        state = checkPass(_pass);
        if (state.getCode() != ERR_OK) {
            return;
        }
        pass = _pass;

        to_orig = _to;
        to = _to;

        {
            // Remove leading "+"
            to_vec::iterator it;
            for (it = to.begin(); it != to.end(); it++) {
               if ( (*it)[0] == '+' )
                    (*it).replace(0, 1, "");
            }
        }

        {
            // Convert "89" to 79
            to_vec::iterator it;
            for (it = to.begin(); it != to.end(); it++) {
               if ( ( (*it)[0] == '8' ) && ( (*it)[1] == '9' ) )
                    (*it).replace(0, 2, "79");
            }
        }

        {
            // Convert city to mobile
            to_vec::iterator it;
            int i = 0;
            for (it = to.begin(), i = 0; it != to.end(); it++, i++) {
                (*it) = MessageClassifier::get_mutable_instance().applyReplace( *it );
                std::ostringstream out;
                out << "Number " << to_orig[ i ] << " converted to " << *it;
                if ( to_orig[ i ] != *it )
                    Logger::get_mutable_instance().smsloginfo( out.str() );
            }
        }

        state = checkTo(to);
        if (state.getCode() != ERR_OK) {
            return;
        }



        state = checkUtf(_utf);
        if (state.getCode() != ERR_OK) {
            return;
        }
        utf = _utf;

        state = checkHex(_hex, _msg);
        if (state.getCode() != ERR_OK) {
            return;
        }
        hex = _hex;

        state = checkMsg(_msg);
        if (state.getCode() != ERR_OK) {
            return;
        }
        msg = _msg;
        if ( ( utf != "1" ) && ( hex != "1" ) )
            msg = StringCp1251ToUtf8(msg);

        state = checkTid(_tid);
        if (state.getCode() != ERR_OK) {
            return;
        }
        tid = _tid;

        state = checkFrom(_from);
        if (state.getCode() != ERR_OK) {
            return;
        }
        from = _from;

        state = checkSubPref(_subpref);
        if (state.getCode() != ERR_OK) {
            return;
        }
        subpref = _subpref;

        state = checkUdh(_udh);
        if (state.getCode() != ERR_OK) {
            return;
        }
        udh = _udh;

        state = checkDelay(_delay);
        if (state.getCode() != ERR_OK) {
            return;
        }
        delay = _delay;

        state = checkDlr(_dlr);
        if (state.getCode() != ERR_OK) {
            return;
        }
        dlr = _dlr;

        state = checkPid(_pid);
        if (state.getCode() != ERR_OK) {
            return;
        }
        pid = _pid;

        priority = _priority;

        state = checkGarant(_garant);
        if (state.getCode() != ERR_OK) {
            return;
        }
        garant = _garant;

	try {
	        ( hex == "1" ) ? parts = 1: parts = utils::getGsmParts( msg );
	} catch ( ... ) {
		state = SMSError(ERR_PARAM, "Invalid charset");
		return;
	}

        id = genID();
        state = checkAuth(uname, pass);

        when = _when;
    }

    std::string SMSRequest::genReport() {
        std::string buf;
        std::ostringstream out(buf);

        out << "<?xml version=\"1.0\"?>" << std::endl
                << "<reply>" << std::endl
                << "<result>" << state.getDescr() << "</result>" << std::endl
                << "<code>" << state.getCode() << "</code>" << std::endl;

        if (getErr().getCode() == ERR_OK && dlr == "1") {
            to_vec::const_iterator it;
            int i;
            for (it = to.begin(), i = 0; it != to.end(); it++, i++) {
                if ( to[i] == to_orig[i] )
                    out << "<sms-id phone=\"" << *it << "\">" << id << "." << i << "</sms-id>" << std::endl;
                else {
                    out << "<sms-id phone=\"" << *it << "\" origphone=\"" << to_orig[i] << "\">" << id << "." << i << "</sms-id>" << std::endl;
                }
            }
        }

        out << "</reply>" << std::endl;

        return out.str();
    }

    std::string SMSRequest::genReportSMST() {
        std::string buf;
        std::ostringstream out(buf);

        out << "<?xml version=\"1.0\"?>" << std::endl
                << "<reply>" << std::endl
                << "<result>" << ( ( state.getCode() == ERR_OK ) ? std::string("OK") : std::string("ERROR") ) << "</result>" << std::endl;
        int code = 0;
        switch ( state.getCode() ) {
            case ERR_OK:        code = 0;   break;
            case ERR_AUTH:      code = 411; break;
            case ERR_SYSTEM:    code = 1000;break;
            case ERR_SYNTAX:    code = 404; break;
            case ERR_PARAM:     code = 404; break;
            case ERR_CREDIT:    code = 415; break;
        }
        out     << "<code>" << code << "</code>" << std::endl;
        out     << "<description>";
        state.getCode() == ERR_OK ? ( out << "queued " << to.size() << " messages" ) : ( out << state.getDescr() );
        out     << "</description>" << std::endl;

        out     << "<message_infos>" << std::endl;
        if (getErr().getCode() == ERR_OK && dlr == "1") {
            to_vec::const_iterator it;
            int i;
            for (it = to.begin(), i = 0; it != to.end(); it++, i++) {
                out << "<message_info>" << std::endl;
                out << "<phone>" << *it << "</phone>" << std::endl;
                out << "<sms_id>" << id << "." << i << "</sms_id>" << std::endl;
                out << "</message_info>" << std::endl;
            }
        }
        out     << "</message_infos>" << std::endl;

        out << "</reply>" << std::endl;

        return out.str();
    }

    const std::list < std::string > SMSRequest::genRequestURLs() const {
        std::list< std::string > res;
        to_vec::const_iterator it;
        int i;
        for (i = 0, it = to.begin(); it != to.end(); it++, i++) {
            res.push_front(genRequestURL(*it, i));
        }
        return res;
    }

    const std::string SMSRequest::genRequestURL(std::string to, int mid) const {
        ConfigManager* cm = ConfigManager::Instance();

        std::string kscript = cm->getProperty<std::string>("kannel.scriptname");

        std::ostringstream out;
        out << kscript << "?";

        out << "to=" << UrlEncodeString(to);
        out << "&validity=1400";

        if (hex == "1") {
            out << "&coding=1"
                    << "&text=" << UrlEncodeString(Hex2String(msg));
        } else {

            out << "&coding=2";
            out << "&text=" << UrlEncodeString(StringUtf8ToUcs2be(msg));
        }


        if (!from.empty())
            out << "&from=" << UrlEncodeString(from);

        //TODO subpref

        if ( ( udh != "0" ) && ( !udh.empty() ) )
            out << "&udh=" << UrlEncodeString( utils::Hex2String( udh ) );

        return out.str();
    }

    void SMSRequest::parseRequest(const std::string req) const {
    };

    void SMSRequest::setID(SMSRequest::ID _id) {
        id = _id;
    }

    SMSRequest::ID SMSRequest::genID() {
        boost::xtime xt;
        boost::xtime_get(&xt, boost::TIME_UTC_);

        boost::mutex::scoped_lock scoped_lock(lock);
        SMSRequest::ID res = (xt.sec * 1e6 + xt.nsec / 1e3)*1e2;
        while (false) {
            std::list< SMSRequest::ID >::iterator it;
            bool found = false;
            for (it = id_cache.begin(); it != id_cache.end(); it++) {
                if (*it == res) {
                    found = true;
                    break;
                }
            }

            if (found) {
                res++;
            } else
                break;
        }

        id_cache.push_front(res);
        if (id_cache.size() > 100)
            id_cache.pop_back();

        return res;
    }

}
