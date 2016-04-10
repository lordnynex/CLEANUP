/*
 * File:   PriorityQueue.h
 * Author: mohtep
 *
 * Created on 22 Февраль 2010 г., 18:22
 */

#ifndef _PRIORITYQUEUE_H
#define	_PRIORITYQUEUE_H

#include "Queue.h"

#include <list>
#include <map>
#include <deque>
#include <algorithm>
#include <boost/integer_traits.hpp>

namespace sms {

    template < class Type, class CompareSTD = std::less<long> >
    class SPriorityQueue {
    public:
        class Lock {
        public:
            Lock( SPriorityQueue& db ) {
                sclock = boost::recursive_mutex::scoped_lock( db.lock );
            }
        private:
            boost::recursive_mutex::scoped_lock sclock;
        };

        SPriorityQueue( int max_size = -1 );
        SPriorityQueue( const SPriorityQueue& orig );

        void push( const Type &val, unsigned int major_priority = 0, unsigned char minor_priority = 0 ) throw ( ContainerFull );
        void pop( ) throw ( ContainerEmpty );
        Type top( ) throw ( ContainerEmpty );
        int size( ) { return data.size(); };

        bool isEmpty();
        static bool Compare( const std::pair< long, Type >& l, const std::pair< long, Type >& r );

    private:
        bool isLimited;
        int max_size;

        boost::recursive_mutex lock;
        std::deque< std::pair< long, Type > > data;
    };

    template < class Type, class CompareSTD  >
    SPriorityQueue<Type, CompareSTD>::SPriorityQueue( int max_size ) {
        isLimited = true;
        this->max_size = max_size;

        if ( max_size == -1 ) {
            isLimited = false;
        }

    }

    template < class Type, class CompareSTD  >
    SPriorityQueue<Type, CompareSTD>::SPriorityQueue( const SPriorityQueue& orig ) {
        max_size = orig.max_size;
        isLimited = orig.isLimited;
    }


    template < class Type, class CompareSTD  >
    bool SPriorityQueue<Type, CompareSTD>::Compare( const std::pair< long, Type >& l, const std::pair< long, Type >& r ) {
        return CompareSTD()( l.first, r.first );
    }


    template < class Type, class CompareSTD  >
    void SPriorityQueue<Type, CompareSTD>::push( const Type &val, unsigned int major_priority, unsigned char minor_priority ) throw ( ContainerFull ) {
        boost::recursive_mutex::scoped_lock scoped_lock(lock);
        if ( isLimited && ( data.size() + 1 > max_size ) )
            BOOST_THROW_EXCEPTION(  ContainerFull()
                                    << throw_descr("SQueue<Type>::push Container is full"));

        long priority = major_priority << CHAR_BIT + minor_priority;
        data.push_back( std::make_pair( priority, val ) );
        std::push_heap( data.begin(), data.end(), Compare );
    }

    template < class Type, class CompareSTD  >
    void SPriorityQueue<Type, CompareSTD>::pop( ) throw ( ContainerEmpty ) {
        boost::recursive_mutex::scoped_lock scoped_lock(lock);
        if ( data.empty() )
            BOOST_THROW_EXCEPTION(  ContainerEmpty()
                                    << throw_descr("SQueue<Type>::pop Container is empty"));

        std::pop_heap( data.begin(), data.end(), Compare );
        data.pop_back();
    }

    template < class Type, class CompareSTD  >
    Type SPriorityQueue<Type, CompareSTD>::top( ) throw ( ContainerEmpty ) {
        boost::recursive_mutex::scoped_lock scoped_lock(lock);
        if ( data.empty() )
            BOOST_THROW_EXCEPTION(  ContainerEmpty()
                                    << throw_descr("SQueue<Type>::top Container is empty"));

        return data.front().second;
    }


    template < class Type, class CompareSTD  >
    bool SPriorityQueue<Type, CompareSTD>::isEmpty( ) {
        return data.empty();
    }
}

#endif	/* _PRIORITYQUEUE_H */

