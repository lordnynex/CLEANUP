/*
 * File:   MonitoringHandler.h
 * Author: mohtep
 *
 * Created on 17 Февраль 2010 г., 18:32
 */

#ifndef _MONITORINGHANDLER_H
#define	_MONITORINGHANDLER_H

#include <Wt/WObject>
#include <Wt/WResource>
#include <Wt/Http/Request>
#include <Wt/Http/Response>
#include <Wt/WLogger>

#include "RequestTracker.h"
#include "SMPPGate.h"

class MonitoringHandler: public Wt::WResource {
public:
	MonitoringHandler(Wt::WObject *parent = 0);
	virtual ~MonitoringHandler();

protected:
	void handleRequest(const Wt::Http::Request& request,
			Wt::Http::Response& response);

private:
	Wt::WLogger logger;
	static RequestTracker trck;
	static int counter;
};

#endif	/* _MONITORINGHANDLER_H */

