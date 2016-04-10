/*
 * File:   Queue.h
 * Author: mohtep
 *
 * Created on 22 Февраль 2010 г., 15:04
 */

#ifndef _QUEUE_H
#define	_QUEUE_H

#include <map>
#include <deque>
#include <queue>
#include <stdexcept>

#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/mutex.hpp>

#include "Error.h"

namespace sms {


    class ContainerError: public FailureError{};
    class ContainerFull: public ContainerError{};
    class ContainerEmpty: public ContainerError{};

    template < class Type >
    class SQueue {
    public:
        class Lock {
        public:
            Lock( SQueue& db ) {
                sclock = boost::recursive_mutex::scoped_lock( db.lock );
            }
        private:
            boost::recursive_mutex::scoped_lock sclock;
        };

        SQueue( int max_size = -1 );
        SQueue( const SQueue& orig );

        void push( const Type& val ) throw ( ContainerFull );
        void pop( ) throw ( ContainerEmpty );
        Type top( ) throw ( ContainerEmpty );
        int size( ) { return data.size(); };

        bool isEmpty();

    private:
        bool isLimited;
        int max_size;

        boost::recursive_mutex lock;
        std::deque< Type > data;
    };

    template < class Type >
    SQueue<Type>::SQueue( int max_size ) {
        isLimited = true;
        this->max_size = max_size;

        if ( max_size == -1 ) {
            isLimited = false;
        }

    }

    template < class Type >
    SQueue<Type>::SQueue( const SQueue& orig ) {
        max_size = orig.maxsize;
        isLimited = orig.isLimited;
    }

    template < class Type >
    void SQueue<Type>::push( const Type &val ) throw ( ContainerFull ) {
        boost::recursive_mutex::scoped_lock scoped_lock(lock);
        if ( isLimited && ( data.size() + 1 > max_size ) )
            BOOST_THROW_EXCEPTION(  ContainerFull()
                                    << throw_descr("SQueue<Type>::push Container is full"));

        data.push_back( val );
    }

    template < class Type >
    void SQueue<Type>::pop( ) throw ( ContainerEmpty ) {
        boost::recursive_mutex::scoped_lock scoped_lock(lock);
        if ( data.empty() )
            BOOST_THROW_EXCEPTION(  ContainerEmpty()
                                    << throw_descr("SQueue<Type>::pop Container is empty"));

        data.pop_front();
    }

    template < class Type >
    Type SQueue<Type>::top( ) throw ( ContainerEmpty ) {
        boost::recursive_mutex::scoped_lock scoped_lock(lock);
        if ( data.empty() )
            BOOST_THROW_EXCEPTION(  ContainerEmpty()
                                    << throw_descr("SQueue<Type>::top Container is empty"));

        return data.front();
    }

    template < class Type >
    bool SQueue<Type>::isEmpty( ) {
        return data.empty();
    }

}

#endif	/* _QUEUE_H */

