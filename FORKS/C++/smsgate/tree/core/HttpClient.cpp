/* 
 * File:   HttpClient.cpp
 * Author: mohtep
 * 
 * Created on 25 Февраль 2010 г., 11:44
 */

#include "HttpClient.h"
#include "Logger.h"

#include <boost/thread.hpp>
#include <boost/thread/xtime.hpp>

namespace sms {

    HttpClient::HttpClient() {
        curl_global_init(CURL_GLOBAL_ALL);
    }

    HttpClient::~HttpClient() {
        curlMapT::iterator it;
        for ( it = curl_handle_map.begin(); it != curl_handle_map.end(); it++ ) {
            curl_easy_cleanup(it->second);
            curl_handle_map.erase(it);
        }

    }
    
    HttpClient::HttpClient(const HttpClient& orig) {
    }

    size_t HttpClient::write_data(void* ptr, size_t size, size_t nmemb, void* stream) {
        std::string& str = *static_cast< std::string* >( stream );
        std::ostringstream out;

        char* buf = new char[ nmemb+1 ];
        for ( size_t i = 0; i < size; i++) {
            strncpy ( buf, static_cast<const char*>( static_cast< char* >( ptr ) + i*nmemb ), nmemb );
            buf[ nmemb ] = 0;

            out << buf;
        }
        
        delete buf;
        str += out.str();

        return size*nmemb;
    }

    size_t HttpClient::write_header(void* ptr, size_t size, size_t nmemb, void* stream) {
        std::vector< std::string >& svec = *static_cast<  std::vector< std::string >* >( stream );
        std::ostringstream out;

        char* buf = new char[ nmemb+1 ];
        for ( size_t i = 0; i < size; i++) {
            strncpy ( buf, static_cast<const char*>( static_cast< char* >( ptr ) + i*nmemb ), nmemb );
            buf[ nmemb ] = 0;

            out << buf;
        }
        svec.push_back( out.str() );
        return size*nmemb;
    }

    HttpClient::Response HttpClient::get(std::string url, int timeout) throw ( HttpError ) {
        boost::thread::id id = boost::this_thread::get_id();
        boost::xtime now;
        boost::xtime_get( &now, boost::TIME_UTC_ );

        std::ostringstream out;
        out << "HttpClient::get URL=" << url << " ";

        char m202[] = "HTTP/1.1 202 Accepted";
        curl_slist* slist = curl_slist_append( NULL, m202);

        Response resp;
        CURL* curl_handle;
        {
            boost::recursive_mutex::scoped_lock lck(lock);

            if ( curl_handle_map.find( id ) == curl_handle_map.end() )
                curl_handle_map[ id ] = curl_easy_init();

            curl_handle = curl_handle_map[ id ];
        }

        curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl_handle, CURLOPT_HTTPGET, 1);
        curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(curl_handle, CURLOPT_HTTP200ALIASES, slist );
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl_handle, CURLOPT_HEADERFUNCTION, write_header);
        if ( timeout ) {
            curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT_MS, timeout);
            curl_easy_setopt(curl_handle, CURLOPT_CONNECTTIMEOUT_MS, timeout);
        }

        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &resp.body);
        curl_easy_setopt(curl_handle, CURLOPT_HEADERDATA, &resp.headers);

        int err_code = curl_easy_perform(curl_handle);

        boost::xtime now2;
        boost::xtime_get( &now2, boost::TIME_UTC_ );
        long diff = (now2.sec-now.sec)*1000 + ( now2.nsec - now.nsec )/1e6;
        if ( err_code != CURLE_OK ) {

            out << "Error " << err_code << " for " << diff << "ms";
            Logger::get_mutable_instance().httplogerr( out.str() );
                BOOST_THROW_EXCEPTION( HttpError()
                        << throw_errno( err_code )
                        << throw_uri( url.c_str() )
                        << throw_descr( "HTTP: get error" ) );
        }

        out << "done for " << diff << "ms";
        Logger::get_mutable_instance().httploginfo( out.str() );

        return resp;
    }

    HttpClient::Response HttpClient::post(std::string url, std::string data, int timeout) throw ( HttpError ) {
        boost::thread::id id = boost::this_thread::get_id();
        boost::xtime now;
        boost::xtime_get( &now, boost::TIME_UTC_ );

        std::ostringstream out;
        out << "HttpClient::post URL=" << url << " data=" << data << " ";

        char m202[] = "HTTP/1.1 202 Accepted";
        curl_slist* slist = curl_slist_append( NULL, m202);

        Response resp;
        CURL* curl_handle;
        {
            boost::recursive_mutex::scoped_lock lck(lock);

            if ( curl_handle_map.find( id ) == curl_handle_map.end() )
                curl_handle_map[ id ] = curl_easy_init();

            curl_handle = curl_handle_map[ id ];
        }

        struct curl_httppost* post = NULL;
        struct curl_httppost* last = NULL;

        curl_formadd(&post, &last, CURLFORM_COPYCONTENTS, data.c_str(), CURLFORM_END);

        curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl_handle, CURLOPT_HTTPPOST, post);
        curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, data.c_str() );
        curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(curl_handle, CURLOPT_HTTP200ALIASES, slist );
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl_handle, CURLOPT_HEADERFUNCTION, write_header);
        if ( timeout ) {
            curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT_MS, timeout);
            curl_easy_setopt(curl_handle, CURLOPT_CONNECTTIMEOUT_MS, timeout);
        }

        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &resp.body);
        curl_easy_setopt(curl_handle, CURLOPT_HEADERDATA, &resp.headers);

        int err_code = curl_easy_perform(curl_handle);
        boost::xtime now2;
        boost::xtime_get( &now2, boost::TIME_UTC_ );
        long diff = (now2.sec-now.sec)*1000 + ( now2.nsec - now.nsec )/1e6;
        if ( err_code != CURLE_OK ) {

            out << "Error " << err_code << " for " << diff << "ms response=" << resp.body;
            Logger::get_mutable_instance().httplogerr( out.str() );
                BOOST_THROW_EXCEPTION( HttpError()
                        << throw_errno( err_code )
                        << throw_uri( url.c_str() )
                        << throw_descr( "HTTP: post error" ) );
        }

        curl_formfree(post);

        out << "done for " << diff << "ms response=" << resp.body;
        Logger::get_mutable_instance().httploginfo( out.str() );


        return resp;
    }

}
