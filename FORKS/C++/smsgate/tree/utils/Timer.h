#ifndef TIMER_H
#define TIMER_H

#include <deque>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <map>

#include "Logger.h"

namespace sms {

    class Timer {
    public:
        Timer();

        typedef boost::function<void()> FuncPTR;

        static Timer* Instance() {
            boost::recursive_mutex::scoped_lock( lock );
            if (!pInstance_)
                pInstance_ = new Timer;
            return pInstance_;
        }

        void MainLoop();
        long addSingleEvent( FuncPTR func, int delay );
        long addPeriodicEvent( FuncPTR func, int delay );
        void cancelEvent( int id );
        static bool Compare( const std::pair< long, long >& l, const std::pair< long, long >& r ) {
            return ( l.first > r.first );
        }
        void funcWrapper( FuncPTR f, long fid );

    private:
        static Timer* pInstance_;
        boost::recursive_mutex lock;
        long lastid;
        std::map<long, FuncPTR> functions;
        std::map<long, long> delay_map;
        std::deque< std::pair< long, long> > timertable;
    };

}

#endif // TIMER_H
