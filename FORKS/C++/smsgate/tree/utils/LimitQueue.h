#ifndef LIMITQUEUE_H
#define LIMITQUEUE_H

#include "Queue.h"
#include "PriorityQueue.h"
#include "Timer.h"

using namespace sms;

template < class Type >
class SLimitQueue {
public:
    class Lock {
    public:
        Lock( SLimitQueue& db ) {
            sclock = boost::recursive_mutex::scoped_lock( db.lock );
        }
    private:
        boost::recursive_mutex::scoped_lock sclock;
    };

    SLimitQueue( int limit = -1, int period = 60 );
    SLimitQueue( const SLimitQueue& orig );
    SLimitQueue operator =( const SLimitQueue& orig );
    ~SLimitQueue();

    void push( const Type& val, int weigth, unsigned int major_priority, unsigned char minor_priority ) throw ( ContainerFull );
    void pop( ) throw ( ContainerEmpty );
    Type top( ) throw ( ContainerEmpty );
    int size( ) { return already_sent; }
    int asize( ) { return awaiting; }
    int limit( ) { return capacity; }
    void setLimit( int limit = -1, int period = 60 );
    bool isOverLoaded( ) { return ( !isInfinite ) && ( awaiting >= ( 60.0 / period )*capacity ); }
    void onTimerTick();

    bool isEmpty();

private:
    bool isInfinite;
    int capacity;
    int already_sent;
    int total_sent;
    int awaiting;
    int period;
    int ticks;

    long timerid;

    boost::recursive_mutex lock;
    SPriorityQueue< std::pair< Type, int > > data_queue;
    SQueue< std::pair< Type, int > > data;
};


template < class Type >
void SLimitQueue<Type>::setLimit( int limit, int period ) {
    boost::recursive_mutex::scoped_lock scoped_lock(lock);
    isInfinite = false;
    capacity = limit;
    this->period = period;

    if ( limit == -1 ) {
        isInfinite = true;
    }

    if ( timerid ) {
        sms::Timer::Instance()->cancelEvent( timerid );
        timerid = 0;
    }

    if ( !isInfinite )
        timerid = sms::Timer::Instance()->addPeriodicEvent( boost::bind( &SLimitQueue<Type>::onTimerTick, this ), 1 );
}

template < class Type >
SLimitQueue<Type>::SLimitQueue( int limit, int period ): timerid( 0 ) {
    boost::recursive_mutex::scoped_lock scoped_lock(lock);
    already_sent = 0;
    total_sent = 0;
    awaiting = 0;
    ticks = 0;

    setLimit( limit, period );
}

template < class Type >
SLimitQueue<Type>::SLimitQueue( const SLimitQueue& orig ) {
    boost::recursive_mutex::scoped_lock scoped_lock( (const_cast< SLimitQueue& >(orig)).lock);
    boost::recursive_mutex::scoped_lock sc_lock(lock);
    isInfinite = orig.isInfinite;
    already_sent = 0;
    capacity = orig.capacity;
    period = orig.period;
    ticks = 0;
    data_queue = SPriorityQueue< Type >( orig.data_queue );
    data = SQueue< Type > ( orig.data );

    if ( !isInfinite )
        timerid = sms::Timer::Instance()->addPeriodicEvent( boost::bind( &SLimitQueue<Type>::onTimerTick, this ), 1 );
}

template < class Type >
SLimitQueue<Type>::~SLimitQueue() {
    boost::recursive_mutex::scoped_lock scoped_lock(lock);
    if ( !isInfinite )
        sms::Timer::Instance()->cancelEvent( timerid );
}

template < class Type >
void SLimitQueue<Type>::push( const Type &val, int weigth, unsigned int major_priority, unsigned char minor_priority ) throw ( ContainerFull ) {
    boost::recursive_mutex::scoped_lock scoped_lock(lock);

    if ( isInfinite ) {
        data.push( std::make_pair( val, weigth ) );
        already_sent += weigth;
    } else {
        data_queue.push( std::make_pair( val, weigth ), major_priority, minor_priority );
        awaiting += weigth;
    }

}

template < class Type >
void SLimitQueue<Type>::pop( ) throw ( ContainerEmpty ) {
    boost::recursive_mutex::scoped_lock scoped_lock(lock);
    if ( data.isEmpty() )
        BOOST_THROW_EXCEPTION(  ContainerEmpty()
                                << throw_descr("SLimitQueue<Type>::pop Container is empty"));

    already_sent -= data.top().second;
    data.pop();
}

template < class Type >
Type SLimitQueue<Type>::top( ) throw ( ContainerEmpty ) {
    boost::recursive_mutex::scoped_lock scoped_lock(lock);
    if ( data.isEmpty() )
        BOOST_THROW_EXCEPTION(  ContainerEmpty()
                                << throw_descr("SLimitQueue<Type>::top Container is empty"));

    return data.top().first;
}

template < class Type >
bool SLimitQueue<Type>::isEmpty( ) {
    return data.isEmpty();
}

template < class Type >
void SLimitQueue<Type>::onTimerTick() {
    boost::recursive_mutex::scoped_lock scoped_lock(lock);

    ticks++;
    if ( ticks >= period ) {
        ticks = 0;
        total_sent = already_sent;
    }

    while ( 1 ) {
        Type val;
        int blocks;
        {
            typename SPriorityQueue< std::pair< Type, int > >::Lock lck ( data_queue );
            if ( data_queue.isEmpty() ) break;
            if ( ( !isInfinite ) && ( total_sent >= capacity ) ) break;
            val = data_queue.top().first;
            blocks = data_queue.top().second;
            awaiting -= blocks;
            data_queue.pop();
        }
        data.push( std::make_pair( val, blocks ) );
        already_sent += blocks;
        total_sent += blocks;
    }

}

#endif // LIMITQUEUE_H
