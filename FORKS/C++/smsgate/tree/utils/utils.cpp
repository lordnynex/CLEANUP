#include "utils.h"

#include <cctype>
#include <cstring>
#include <cerrno>
#include "iconv.h"
#include <iostream>
#include <ctime>
#include <clocale>
#include <algorithm>

namespace sms {

    using namespace std;

    namespace utils {

        void Tokenize(const string& str, vector<string>& tokens, const string& delimiters) {
            // Skip delimiters at beginning.
            string::size_type lastPos = str.find_first_not_of(delimiters, 0);
            // Find first "non-delimiter".
            string::size_type pos = str.find_first_of(delimiters, lastPos);

            while (string::npos != pos || string::npos != lastPos) {
                // Found a token, add it to the vector.
                tokens.push_back(str.substr(lastPos, pos - lastPos));
                // Skip delimiters.  Note the "not_of"
                lastPos = str.find_first_not_of(delimiters, pos);
                // Find next "non-delimiter"
                pos = str.find_first_of(delimiters, lastPos);
            }
        }

        void splitArgs(const string& s, map<string, string>& m) {
            //o << "Parsing string: " << s << endl;
            vector< string > l;
            Tokenize(s, l, "&");
            vector< string >::iterator it;
            for (it = l.begin(); it != l.end(); it++) {
                vector<string> k;
                Tokenize(*it, k, "=");
                if (k.size() == 2) {
                    m[k[0]] = k[1];
                }
            }
        }

        string joinArgs(const map<string, string>& m) {
            string res;
            map<string, string>::const_iterator it;
            bool first = true;
            for ( it = m.begin(); it != m.end(); it++ ) {
                if ( !first )
                    res += "&";
                res += it->first;
                res += "=";
                res += it->second;

                first = false;
            }
        }


        string char2hex(char dec) {
            char dig1 = (dec & 0xF0) >> 4;
            char dig2 = (dec & 0x0F);
            if (0 <= dig1 && dig1 <= 9) dig1 += 48; //0,48inascii
            if (10 <= dig1 && dig1 <= 15) dig1 += 97 - 10; //a,97inascii
            if (0 <= dig2 && dig2 <= 9) dig2 += 48;
            if (10 <= dig2 && dig2 <= 15) dig2 += 97 - 10;

            string r;
            r.append(&dig1, 1);
            r.append(&dig2, 1);
            return r;
        }

        std::string UrlEncodeString(const std::string &c) {

            string escaped = "";
            int max = c.length();
            for (int i = 0; i < max; i++) {
                if ((48 <= c[i] && c[i] <= 57) || //0-9
                        (65 <= c[i] && c[i] <= 90) || //abc...xyz
                        (97 <= c[i] && c[i] <= 122) || //ABC...XYZ
                        (c[i] == '~' || c[i] == '!' || c[i] == '*' || c[i] == '(' || c[i] == ')')
                        ) {
                    escaped.append(&c[i], 1);
                } else {
                    escaped.append("%");
                    escaped.append(char2hex(c[i])); //converts char 255 to string "ff"
                }
            }
            return escaped;
        }

        std::string String2Hex(const std::string &c) {

            string escaped = "";
            int max = c.length();
            for (int i = 0; i < max; i++) {
                escaped.append(char2hex(c[i])); //converts char 255 to string "ff"
            }
            return escaped;
        }

        unsigned char hex_conv(char t) {
            if ((t >= '0') && (t <= '9'))
                return static_cast<unsigned char> (t) - '0';
            else {
                if ((t >= 'A') && (t <= 'Z'))
                    return static_cast<unsigned char> (t) - 'A' + 0xa;
                else
                    return static_cast<unsigned char> (t) - 'a' + 0xa;
            }
        }

        std::string UrlDecodeString(const std::string& st) {
            std::ostringstream r;
            const char* ptr = st.c_str();
            const char* end = ptr + st.length();
            while (ptr < end) {
                switch (*ptr) {
                    case '%':
                        r << static_cast<char> ((hex_conv(*(ptr + 1)) << 4)+(hex_conv(*(ptr + 2))));
                        ptr += 3;
                        break;
                    case '+':
                        r << ' ';
                        ptr++;
                        break;
                    default:
                        r << *ptr;
                        ptr++;
                }
            }
            return r.str();
        }

        std::string StringRecodeFromTo(std::string src, const std::string from, std::string to) throw ( ParamError) {
            string res;
	    string tmpres;
            int err;
            size_t srcsize = src.size();
            size_t dstsize = srcsize * 3;
            size_t origdst = dstsize;
            char* srcbuf = new char[ srcsize ];
            char* destbuf = new char[ dstsize ];
            memcpy(srcbuf, src.c_str(), srcsize);

            #ifdef _LIBICONV_VERSION
                const char* srcb = srcbuf;
            #else
                char * srcb = srcbuf;
            #endif

            char* dstb = destbuf;

            iconv_t id = iconv_open(to.c_str(), from.c_str());
            if (id == (iconv_t) (-1)) {
                delete [] srcbuf;
                delete [] destbuf;

                switch (errno) {
                    case EINVAL:
                        BOOST_THROW_EXCEPTION(ParamError()
                                << throw_errno(errno)
                                << throw_descr("ICONV: The conversion is not supported by the implementation"));

                    default:
                        BOOST_THROW_EXCEPTION(ParamError()
                                << throw_errno(errno)
                                << throw_descr("ICONV: Unknown Error"));
                }
            }


            err = iconv(id, &srcb, &srcsize, &dstb, &dstsize);
            if (err == -1) {

                iconv_close(id);

                delete [] srcbuf;
                delete [] destbuf;

                switch (errno) {
                        BOOST_THROW_EXCEPTION(ParamError()
                                << throw_errno(errno)
                                << throw_descr("ICONV: An invalid multibyte sequence"));
                    case EINVAL:
                        BOOST_THROW_EXCEPTION(ParamError()
                                << throw_errno(errno)
                                << throw_descr("ICONV: An incomplete multibyte sequence"));
                    default:
                        BOOST_THROW_EXCEPTION(ParamError()
                                << throw_errno(errno)
                                << throw_descr("ICONV: Unknown error"));
                }
            }

            tmpres = std::string(destbuf, origdst - dstsize);
	    res.resize( tmpres.size() );
	    copy( tmpres.begin(), tmpres.end(), res.begin() );

            delete [] srcbuf;
            delete [] destbuf;

            iconv_close(id);

            return res;
        }

        std::string StringUtf8ToCp1251(const std::string src) throw ( ParamError) {
            return StringRecodeFromTo(src, "UTF-8", "CP1251");
        }

        std::string StringCp1251ToUtf8(const std::string src) throw ( ParamError) {
            return StringRecodeFromTo(src, "CP1251", "UTF-8");
        }

        std::string StringUtf8ToUcs2be(const std::string src) throw ( ParamError) {
            return StringRecodeFromTo(src, "UTF-8", "UCS-2BE");
        }

        std::string StringCp1251ToUcs2be(const std::string src) throw ( ParamError) {
            return StringRecodeFromTo(src, "CP1251", "UCS-2BE");
        }

        char HexChar2Dec(char c1) {
            char a = c1;
            if ((a >= 'a') && (a <= 'f'))
                return a - 'a' + 10;
            if ((a >= 'A') && (a <= 'F'))
                return a - 'A' + 10;
            if ((a >= '0') && (a <= '9'))
                return a - '0';
            return 0;
        }

        std::string Hex2String(std::string src) {
            int sl = (src.length() / 2);
            std::string res;
            res.resize( sl );

            for (int i = 0; i < sl; i++) {
                res[ i ] = (HexChar2Dec(src[2*i]) << 4) + HexChar2Dec(src[2*i + 1]);
            }
            return res;
        }

        std::string str_join(const std::vector<std::string> & vec, const std::string & sep) {
            if (vec.size() == 0)
                return "";
            string::size_type size = sep.length() * vec.size();
            for (unsigned int i = 0; i < vec.size(); i++) {
                size += vec[i].size();
            }

            string tmp;
            tmp.reserve(size);
            tmp = vec[0];
            for (unsigned int i = 1; i < vec.size(); i++) {
                tmp = tmp + sep + vec[i];
            }
            return tmp;
        }

        std::string escapeString( std::string where, std::string what, std::string by ) {

            for ( int i = 0; i < what.length(); i++ ) {
                int lp = where.length();
                do {
                    int pos = where.substr(0, lp).find_last_of(what.substr(i, 1));
                    if ( pos < 0 )
                        continue;

                    lp = pos;
                    where.insert(lp, by);
                } while ( ( lp > 0 ) && ( lp < where.length() ) );
            }

            return where;
        }
        wstring widen( const string& str )
        {
              wostringstream wstm ;
              wstm.imbue(std::locale("en_US.UTF-8"));
              const ctype<wchar_t>& ctfacet =
              use_facet< ctype<wchar_t> >( wstm.getloc() ) ;
              for( size_t i=0 ; i<str.size() ; ++i )
              wstm << ctfacet.widen( str[i] ) ;
              return wstm.str() ;
        }

        string narrow( const wstring& str )
        {
              ostringstream stm ;
              stm.imbue(std::locale("en_US"));
              const ctype<char>& ctfacet =
              use_facet< ctype<char> >( stm.getloc() ) ;
              for( size_t i=0 ; i<str.size() ; ++i )
              stm << ctfacet.narrow( str[i], 0 ) ;
              return stm.str() ;
        }

        int getGsmParts( const string& orig ) {
            wchar_t gsm_legal_characters[] = {
                0x0040, 0x00A3, 0x0024, 0x00A5, 0x00E8, 0x00E9, 0x00F9, 0x00EC, 0x00F2, 0x00C7, 0x000A, 0x00D8, 0x00F8, 0x000D, 0x00C5, 0x00E5,
                0x0394, 0x005F, 0x03A6, 0x0393, 0x039B, 0x03A9, 0x03A0, 0x03A8, 0x03A3, 0x0398, 0x039E, 0x001B, 0x00C6, 0x00E6, 0x00DF, 0x00C9,
                0x0020, 0x0021, 0x0022, 0x0023, 0x00A4, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F,
                0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F,
                0x00A1, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F,
                0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, 0x00C4, 0x00D6, 0x00D1, 0x00DC, 0x00A7,
                0x00BF, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x0069, 0x006A, 0x006B, 0x006C, 0x006D, 0x006E, 0x006F,
                0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077, 0x0078, 0x0079, 0x007A, 0x00E4, 0x00F6, 0x00F1, 0x00FC ,0x00E0
            };
            wstring gsm_legal = wstring( gsm_legal_characters );

            wchar_t gsm_advanced_characters[] = {
                0x20AC, 0x000C, 0x005B, 0x005C, 0x005D, 0x005E, 0x007B, 0x007C, 0x007D, 0x007E

            };
            wstring gsm_advanced = wstring( gsm_advanced_characters );
            string su = StringUtf8ToUcs2be( orig );
            wstring sw;
            for ( string::const_iterator it = su.begin(); it != su.end(); it++ ) {
                wchar_t l1 = *it;
                wchar_t l2 = *(++it);
                wchar_t l = l1 *256 + l2;
                sw.push_back( l );
            }

            bool isGsmClear = true;
            int gsmLength = 0;
            for ( wstring::const_iterator it = sw.begin(); it != sw.end(); it++ ) {
                bool gsm_legal_found = false;
                bool gsm_advanced_found = false;
                wchar_t ch = *it;
                if ( ch == wchar_t(0x0000) ) continue;
                if ( gsm_legal.find( *it ) != gsm_legal.npos ) gsm_legal_found = true;
                if ( gsm_advanced.find( *it ) != gsm_advanced.npos ) gsm_advanced_found = true;

                if ( gsm_legal_found ) gsmLength++;
                if ( gsm_advanced_found ) gsmLength+=2;
                if ( !gsm_legal_found && !gsm_advanced_found )
                    isGsmClear = false;
            }

            if ( isGsmClear ) {
                return gsmLength <= 160 ? 1: ( ( gsmLength - 1 )/153 ) + 1;
            } else {
                return sw.length() <= 70? 1: ( ( sw.length() - 1 )/67 ) + 1;
            }

        }

        time_t datetime2ts( const std::string& src_date, int ts, const std::string format ) {
            time_t now = time( NULL );
            struct tm result = *localtime( &now );
            char* pos= strptime( src_date.c_str(), format.c_str(), &result);
            if ( pos == NULL ) {
                return now;
            }
            result.tm_hour -= ts;
            result.tm_sec += result.tm_gmtoff;
            time_t tsl = (time_t)mktime( &result );

            return time_t( tsl );
        }

        std::string ts2datetime( time_t src_date, int ts, const std::string format ) {
            char buf[100];
            struct tm result = *localtime( &src_date );
            result.tm_hour += ts;
            result.tm_sec -= result.tm_gmtoff;
            mktime( &result );

            strftime( buf, 100, format.c_str(), &result );
            return buf;
        }

    }

}


