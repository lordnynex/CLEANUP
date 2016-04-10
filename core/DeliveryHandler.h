/*
 * DeliveryHandler.h
 *
 *  Created on: 15.02.2010
 *      Author: mohtep
 */

#include <Wt/WObject>
#include <Wt/WResource>
#include <Wt/Http/Request>
#include <Wt/Http/Response>
#include <Wt/WLogger>

#include "RequestTracker.h"

#ifndef DELIVERYHANDLER_H_
#define DELIVERYHANDLER_H_

class DeliveryHandler: public Wt::WResource {
public:
	DeliveryHandler(Wt::WObject *parent = 0);
	virtual ~DeliveryHandler();

protected:
	void handleRequest(const Wt::Http::Request& request,
			Wt::Http::Response& response);

private:
	Wt::WLogger logger;
	static RequestTracker trck;
	static int counter;
};


#endif /* DELIVERYHANDLER_H_ */
