/* 
 * File:   HttpClient.h
 * Author: mohtep
 *
 * Created on 25 Февраль 2010 г., 11:44
 */

#ifndef _HTTPCLIENT_H
#define	_HTTPCLIENT_H

#include <string>
#include <sstream>
#include <cstring>
#include <vector>

#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread.hpp>

#include <curl/curl.h>
#include <curl/easy.h>

#include "Error.h"

namespace sms {

    class HttpError: public RetryError {};
    typedef boost::error_info<struct tag_uri,const char*> throw_uri;

    class HttpClient {
    public:

        struct Response {
            std::string body;
            std::vector< std::string > headers;
        };

        HttpClient();
        HttpClient(const HttpClient& orig);
        virtual ~HttpClient();

        Response get( std::string url, int timeout = 0 ) throw (HttpError);
        Response post( std::string url, std::string data, int timeout = 0 ) throw (HttpError);
    private:
        typedef std::map< boost::thread::id, CURL * > curlMapT;
        typedef std::map< boost::thread::id, long > curlReqMapT;
        curlMapT curl_handle_map;
        curlReqMapT curl_req_map;
        boost::recursive_mutex lock;

        static size_t write_data( void *ptr, size_t size, size_t nmemb, void *stream );
        static size_t write_header( void *ptr, size_t size, size_t nmemb, void *stream );
    };
}

#endif	/* _HTTPCLIENT_H */

