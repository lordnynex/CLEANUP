#ifndef MTHANDLER_H
#define MTHANDLER_H

#include <Wt/WObject>
#include <Wt/WResource>
#include <Wt/Http/Request>
#include <Wt/Http/Response>

#include "RequestTracker.h"
#include "SMSRequest.h"
#include "utils.h"

class MTHandler: public Wt::WResource {
public:
        MTHandler(Wt::WObject *parent = 0);
        virtual ~MTHandler();

protected:
        void handleRequest(const Wt::Http::Request& request,
                        Wt::Http::Response& response);

private:

        static RequestTracker trck;
        static int counter;
};

#endif // MTHANDLER_H
