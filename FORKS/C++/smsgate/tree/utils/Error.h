/* 
 * File:   Error.h
 * Author: mohtep
 *
 * Created on 24 Февраль 2010 г., 16:04
 */

#ifndef _ERROR_H
#define _ERROR_H

#include <exception>
#include <boost/exception/all.hpp>
#include <boost/throw_exception.hpp>

#include <string>

namespace sms {

    typedef boost::error_info<struct tag_errno,int> throw_errno;
    typedef boost::error_info<struct tag_descr,const char*> throw_descr;
    
    class BasicError: public boost::exception, public std::exception{};

    class CriticalError: public BasicError{};
    class ParamError: public BasicError{};
    class FailureError: public BasicError{};
    class RetryError: public BasicError{};
    
}

#endif	/* _ERROR_H */

