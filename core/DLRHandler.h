#ifndef DLRHANDLER_H
#define DLRHANDLER_H

#include <Wt/WObject>
#include <Wt/WResource>
#include <Wt/Http/Request>
#include <Wt/Http/Response>
#include <Wt/WLogger>

#include "RequestTracker.h"

class DLRHandler: public Wt::WResource {
public:
        DLRHandler(Wt::WObject *parent = 0);
        virtual ~DLRHandler();

protected:
        void handleRequest(const Wt::Http::Request& request,
                        Wt::Http::Response& response);

private:
        Wt::WLogger logger;
        static RequestTracker trck;
        static int counter;
};


#endif // DLRHANDLER_H
