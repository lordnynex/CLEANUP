/*
 * RequestHandler.h
 *
 *  Created on: 22.01.2010
 *      Author: mohtep
 */

#include <Wt/WObject>
#include <Wt/WResource>
#include <Wt/Http/Request>
#include <Wt/Http/Response>

#include "RequestTracker.h"
#include "SMSRequest.h"
#include "utils.h"


#ifndef REQUESTHANDLER_H_
#define REQUESTHANDLER_H_

class RequestHandler: public Wt::WResource {
public:
	RequestHandler(Wt::WObject *parent = 0);
	virtual ~RequestHandler();

protected:
	void handleRequest(const Wt::Http::Request& request,
			Wt::Http::Response& response);

private:
	
	static RequestTracker trck;
	static int counter;
};

class RequestHandlerSMST: public Wt::WResource {
public:
        RequestHandlerSMST(Wt::WObject *parent = 0);
        virtual ~RequestHandlerSMST();

protected:
        void handleRequest(const Wt::Http::Request& request,
                        Wt::Http::Response& response);

private:

        static RequestTracker trck;
        static int counter;
};

#endif /* REQUESTHANDLER_H_ */
