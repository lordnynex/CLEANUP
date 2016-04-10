#include "Timer.h"

#include <boost/thread.hpp>
#include <boost/thread/xtime.hpp>
#include <algorithm>
#include <iostream>

namespace sms {

    Timer* Timer::pInstance_;

    Timer::Timer() {
        lastid = 0;
        boost::xtime xt;
        boost::xtime_get(&xt, boost::TIME_UTC_);
        boost::thread eventloop(boost::bind( &Timer::MainLoop, this ));
    }

    void Timer::MainLoop() {

        while ( 1 ) {
            boost::xtime xt;
            while ( true ) {
                long funcid;
                boost::recursive_mutex::scoped_lock( lock );
                //Queue block

                if ( timertable.empty() )
                    break;

                if ( timertable.front().first > xt.sec )
                    break;

                funcid = timertable.front().second;
                std::pop_heap( timertable.begin(), timertable.end(), Compare );
                timertable.pop_back();

                if ( functions.find( funcid ) == functions.end() ) {
                    std::ostringstream out;
                    out << "Timer: error in processing function ID=" << funcid;

                    Logger::get_mutable_instance().smslogerr( out.str() );
                    continue;
                }


                boost::thread eventloop( boost::bind( &Timer::funcWrapper, this, functions[ funcid ], funcid ) );

                boost::xtime_get(&xt, boost::TIME_UTC_);
                if ( delay_map[ funcid ] ) {
                    timertable.push_back( std::make_pair( xt.sec + delay_map[ funcid ], funcid ) );
                    std::push_heap( timertable.begin(), timertable.end(), Compare );
                }
            }

            boost::xtime_get(&xt, boost::TIME_UTC_);
            xt.sec += 1;
            boost::thread::sleep(xt);
        }
    }

    void Timer::funcWrapper( FuncPTR f, long fid ) {
        try {
            f();
        } catch ( ... ) {
            std::ostringstream out;
            out << "Timer: Timer bad function ID=" << fid;
            Logger::get_mutable_instance().smsloginfo( out.str() );
        }
    }

    long Timer::addSingleEvent( FuncPTR func, int delay ) {
        boost::recursive_mutex::scoped_lock( lock );
        boost::xtime xt;
        boost::xtime_get(&xt, boost::TIME_UTC_);

        lastid++;
        functions[ lastid ] = func;
        timertable.push_back( std::make_pair( xt.sec + delay, lastid ) );
        std::push_heap( timertable.begin(), timertable.end(), Compare );
        delay_map[ lastid ] = 0;
//        std::ostringstream out;
//        out << "Timer: addSingleEvent ID=" << lastid;
//        Logger::get_mutable_instance().loginfo( out.str() );
        return lastid;
    }

    long Timer::addPeriodicEvent( FuncPTR func, int delay ) {
        //delay *= 2;
        boost::recursive_mutex::scoped_lock( lock );
        boost::xtime xt;
        boost::xtime_get(&xt, boost::TIME_UTC_);

        lastid++;
        functions[ lastid ] = func;
        timertable.push_back( std::make_pair( xt.sec + delay, lastid )  );
        std::push_heap( timertable.begin(), timertable.end(), Compare );
        delay_map[ lastid ] = delay;
//        std::ostringstream out;
//        out << "Timer: addPeriodicEvent ID=" << lastid;
//        Logger::get_mutable_instance().loginfo( out.str() );
        return lastid;
   }

    void Timer::cancelEvent( int id ) {
        boost::recursive_mutex::scoped_lock( lock );

        functions.erase( id );
        delay_map.erase( id );

        std::deque< std::pair< long, long> >::iterator it;
        std::ostringstream out;
//        out << "Timer: cancelEvent ID=" << id;

//        Logger::get_mutable_instance().loginfo( out.str() );

    }

}
