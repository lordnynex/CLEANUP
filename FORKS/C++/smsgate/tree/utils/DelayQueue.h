/* 
 * File:   DelayQueue.h
 * Author: mohtep
 *
 * Created on 28 Февраль 2010 г., 13:04
 */

#include "Cache.h"

#include <set>

#ifndef _DELAYQUEUE_H
#define	_DELAYQUEUE_H

namespace sms {

    template < class Type >
    class SDelayQueue {
    public:
        class Lock {
        public:
            Lock( SDelayQueue& db ):sclock( db.lock ) { }
        private:
            boost::recursive_mutex::scoped_lock sclock;
        };

        void onExpired( Type val, int id )  {
            Lock lck(*this);
            data_ready.push( val );
        }

        SDelayQueue( int __max_size = -1 ): data_ready( __max_size ), data_queue( __max_size ) {
            lastid = 0;
            max_size = __max_size;
            data_queue.setOnExpiredFunc( boost::bind( &SDelayQueue<Type>::onExpired, this, _1, _2 ) );
        }

        SDelayQueue( const SDelayQueue<Type>& orig ): data_ready( orig.max_size ), data_queue( orig.max_size ) {
            max_size = orig.max_size;
            data_queue.setOnExpiredFunc( boost::bind( &SDelayQueue<Type>::onExpired, this, _1, _2 ) );
        }

        void push( const Type& val, int delay ) throw ( ContainerFull ) {
            if ( delay < 0 )
                delay = 0;
            data_queue.push( val, lastid, delay );
            lastid++;
        }

        void pop( ) throw ( ContainerEmpty ) { Lock lck(*this); data_ready.pop(); }
        Type top( ) throw ( ContainerEmpty ) { Lock lck(*this); return data_ready.top(); }
        int size( ) { return data_ready.size(); }
        int asize( ) { return data_queue.size(); }

        bool isEmpty() { return data_ready.isEmpty(); }

        boost::recursive_mutex lock;
    private:
        SQueue< Type > data_ready;
        SCache< Type, int > data_queue;
        int max_size;

        int lastid;
    };

}
#endif	/* _DELAYQUEUE_H */

