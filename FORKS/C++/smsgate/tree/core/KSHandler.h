#ifndef KSHANDLER_H
#define KSHANDLER_H

#include <Wt/WObject>
#include <Wt/WResource>
#include <Wt/Http/Request>
#include <Wt/Http/Response>
#include <Wt/WLogger>

#include "RequestTracker.h"

class KSHandler: public Wt::WResource {
public:
        KSHandler(Wt::WObject *parent = 0);
        virtual ~KSHandler();

protected:
        void handleRequest(const Wt::Http::Request& request,
                        Wt::Http::Response& response);

private:
        Wt::WLogger logger;
        static RequestTracker trck;
        static int counter;
};


#endif // KSHANDLER_H
