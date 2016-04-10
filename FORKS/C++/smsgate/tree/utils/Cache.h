/*
 * File:   Cache.h
 * Author: mohtep
 *
 * Created on 22 Февраль 2010 г., 20:55
 */

#ifndef _CACHE_H
#define	_CACHE_H

#include <algorithm>
#include <set>

#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/utility.hpp>
#include <boost/tuple/tuple.hpp>
#include "Timer.h"
#include "Queue.h"

namespace sms {

    template < class Type, class TypeID >
    class SCache {
    public:
        typedef boost::function< void(Type, TypeID) > FuncPTR;

        SCache( int max_size = -1, int default_ttl = 10*60 );
        SCache( const SCache& orig );
        ~SCache();

        void push( const Type val, const TypeID id, int ttl=-1 );
        bool exists( const TypeID val );
        Type get( const TypeID id );
        int size() {
            return occur.size();
        }
        bool isEmpty();
        void setOnExpiredFunc( FuncPTR func );

        static bool Compare( const std::pair< int, TypeID >& l, const std::pair< int, TypeID >& r );
        void onTimerTick();
        void onExpired( Type, TypeID )  { }

    private:
        bool isLimited;
        int max_size;
        int default_ttl;
        long timerid;
        FuncPTR expFunc;

        boost::recursive_mutex lock;
        std::deque< std::pair< int, TypeID > > data;
        std::map< TypeID, boost::tuples::tuple< Type, int, int > > occur;
    };

    template < class Type, class TypeID >
    SCache<Type, TypeID>::SCache( int max_size, int default_ttl ): expFunc( boost::bind( &SCache<Type, TypeID>::onExpired, this, _1, _2 ) ) {

        isLimited = true;
        if ( max_size == -1 ) {
            isLimited = false;
        }

        this->max_size = max_size;
        this->default_ttl = default_ttl;
        timerid = Timer::Instance()->addPeriodicEvent( boost::bind( &SCache<Type, TypeID>::onTimerTick, this ), 1 );
    }

    template < class Type, class TypeID >
    SCache<Type, TypeID>::SCache( const SCache& orig ) {
        max_size = orig.max_size;
        isLimited = orig.isLimited;
        default_ttl = orig.default_ttl;
        expFunc = orig.expFunc;
        timerid = Timer::Instance()->addPeriodicEvent( boost::bind( &SCache<Type, TypeID>::onTimerTick, this ), 1 );
        onTimerTick();
    }

    template < class Type, class TypeID >
    SCache<Type, TypeID>::~SCache( ) {
        Timer::Instance()->cancelEvent( timerid );
    }

    template < class Type, class TypeID >
    void SCache<Type, TypeID>::onTimerTick() {
        boost::recursive_mutex::scoped_lock scoped_lock(lock);
        boost::xtime now;
        boost::xtime_get( &now, boost::TIME_UTC_ );

        while ( ( !data.empty() ) && ( data.front().first <= now.sec ) ) {
            TypeID id = data.front().second;
            std::pop_heap( data.begin(), data.end(), Compare );
            data.pop_back();

            boost::tuples::get<2>( occur[ id ] )--;
            if ( boost::tuples::get<2>( occur[ id ] ) == 0 ) {
                expFunc( boost::tuples::get<0>( occur[ id ] ), id );
                occur.erase( id );
            }
        }
    }

    template < class Type, class TypeID >
    bool SCache<Type, TypeID>::Compare( const std::pair< int, TypeID >& l, const std::pair< int, TypeID >& r ) {
        return ( l.first > r.first );
    }

    template < class Type, class TypeID >
    void SCache<Type, TypeID>::setOnExpiredFunc( FuncPTR func ) {
        boost::recursive_mutex::scoped_lock scoped_lock(lock);
        expFunc = func;
    }

    template < class Type, class TypeID >
    void SCache<Type, TypeID>::push( const Type val, const TypeID id, int ttl ) {
        boost::recursive_mutex::scoped_lock scoped_lock(lock);
        boost::xtime now;
        boost::xtime_get( &now, boost::TIME_UTC_);

        if ( ttl == -1 )
            ttl = default_ttl;

        if ( isLimited && ( occur.size() + 1 > max_size ) ) {
            return;
        }

        if ( occur.find( id ) != occur.end() ) {
            boost::tuples::get<2>( occur[ id ] )++;
        } else {
            boost::tuples::get<0>( occur[ id ] ) = val;
            boost::tuples::get<1>( occur[ id ] ) = ttl;
            boost::tuples::get<2>( occur[ id ] ) = 1;
        }
        data.push_back( std::make_pair( now.sec + ttl, id ) );
        long size = data.size();
        std::push_heap( data.begin(), data.end(), Compare );
    }

    template < class Type, class TypeID >
    bool SCache<Type, TypeID>::exists( const TypeID id ) {
        boost::recursive_mutex::scoped_lock scoped_lock(lock);
        return ( occur.find( id ) != occur.end() );
    }

    template < class Type, class TypeID >
    Type SCache<Type, TypeID>::get( const TypeID id ) {
        boost::recursive_mutex::scoped_lock scoped_lock(lock);
        if ( exists( id ) )
            return boost::tuples::get<0>( occur[ id ] );
        else
            BOOST_THROW_EXCEPTION(  ContainerEmpty()
                                    << throw_descr("SCache<Type, TypeID>::get item not found"));
    }

    template < class Type, class TypeID >
    bool SCache<Type, TypeID>::isEmpty( ) {
        boost::recursive_mutex::scoped_lock scoped_lock(lock);
        return occur.empty();
    }

}

#endif	/* _CACHE_H */

