/*
 * File:   MonitoringHandler.cpp
 * Author: mohtep
 *
 * Created on 17 Февраль 2010 г., 18:32
 */

#include "MonitoringHandler.h"
#include "StatManager.h"
#include "SMPPGateManager.h"
#include "SMSMessage.h"

#include <cstdio>

using std::string;

MonitoringHandler::MonitoringHandler(Wt::WObject *parent) : Wt::WResource(parent) {
	logger.addField("message", true);
	logger.setStream( std::cout );
}

MonitoringHandler::~MonitoringHandler() {
	beingDeleted();
}

void MonitoringHandler::handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response) {
    response.setMimeType("text/html");

    try {
        RequestTracker *trck = RequestTracker::Instance();
        response.out().setf(std::ios::fixed ,std::ios::floatfield);
        response.out().precision(1);
        response.out() << "<meta http-equiv=\"refresh\" content=\"1\">";


        response.out() << "<pre>";
        response.out() << "Uptime: " << StatManager::Instance()->getUptime() << "<br>";
        response.out() << "Operations queue length: " << trck->op_queue.size() << "<br>";
        response.out() << "Delayed queue length: " << trck->del_queue.size() << "<br>";
        response.out() << "Delayed queue total length: " << trck->del_queue.asize() << "<br>";
        response.out() << "Outbox queue length: " << trck->out_queue->size() << "<br>";
        RequestTracker::TPartnersLimitMap::iterator it;
        response.out() << "Outbox partner queue length: ";
        {
            for ( it = trck->pl_map.begin(); it != trck->pl_map.end(); it++ )
                if ( it->second->asize() > 0 ) {
                if ( it->second->asize() > it->second->limit() )
                    response.out() << "[" << it->first << ";" << it->second->size() << "] ";
            }
        }
        response.out() << "<br>";

        response.out() << "Outbox partner total length: ";
        {
            for ( it = trck->pl_map.begin(); it != trck->pl_map.end(); it++ )
                if ( it->second->asize() > 0 ) {
                if ( it->second->asize() > it->second->limit() )
                    response.out() << "[" << it->first << ";" << it->second->asize() << "] ";
            }
        }
        response.out() << "<br>";

        response.out() << "Requests cache length: " << trck->req_cache.size() << "<br>";
        response.out() << "Message cache length: " << SMSMessageManager::get_mutable_instance().count() << "<br>";
        response.out() << "Not synced messages cache length: " << SMSMessageManager::get_mutable_instance().count_dirty() << "<br>";

        StatManager::gNamePropMap s1sec = StatManager::Instance()->get1SecondStatsSMPPGate();
        StatManager::gNamePropMap s1min = StatManager::Instance()->get1MinuteStatsSMPPGate();
        StatManager::gNamePropMap s5min = StatManager::Instance()->get5MinuteStatsSMPPGate();
        StatManager::gNamePropMap s1hour = StatManager::Instance()->get1HourStatsSMPPGate();
        StatManager::gNamePropMap s1day = StatManager::Instance()->get1DayStatsSMPPGate();

        response.out() << "<table>";
        response.out() << "<tr>";
        response.out() << "<td>";
        response.out() << "Queue info: ";
        response.out() << "</td>";
        for ( StatManager::gNamePropMap::iterator it = s1sec.begin(); it != s1sec.end(); it++ ) {
            response.out() << "<td>";
            if ( SMPPGateManager::Instance()->getGates()[it->first].enabled() ) {
                if ( SMPPGateManager::Instance()->getGates()[it->first].isBusy() )
                    response.out() << "<font color=orange>";
                else if ( SMPPGateManager::Instance()->getGates()[it->first].suspended() )
                    response.out() << "<font color=red>";
                else
                    response.out() << "<font color=green>";
            }
            response.out() << "[" << it->first<< "]<br>";
                response.out() << "</font>";
            response.out() << "</td>";
        }
        response.out() << "</tr>";

        response.out() << "<tr>";
        response.out() << "<td>";
        response.out() << "1 Minute stats: ";
        response.out() << "</td>";
        for ( StatManager::gNamePropMap::iterator it = s1min.begin(); it != s1min.end(); it++ ) {
            response.out() << "<td>";
            response.out() << "[" << it->first<< "]<br>";
            response.out() << "RQT:   " << it->second.requests << "<br>";
            response.out() << "ACK:        " << it->second.acks << "<br>";
            response.out() << "Rsp:  " << it->second.responses << "<br>";
            response.out() << "Dlr: " << it->second.deliveres << "<br>";
            //        response.out() << "ADT: " << it->second.deliverytime << "<br>";
            response.out() << "DLV (" << it->second.deliveres*100 / ( it->second.requests == 0 ? 1: it->second.requests ) << "%)" << "<br>";
            response.out() << "</td>";
        }
        response.out() << "</tr>";

        response.out() << "<tr>";
        response.out() << "<td>";
        response.out() << "5 Minute stats: ";
        response.out() << "</td>";
        for ( StatManager::gNamePropMap::iterator it = s5min.begin(); it != s5min.end(); it++ ) {
            response.out() << "<td>";
            response.out() << "[" << it->first<< "]<br>";
            response.out() << "RQT:   " << it->second.requests << "<br>";
            response.out() << "ACK:        " << it->second.acks << "<br>";
            response.out() << "Rsp:  " << it->second.responses << "<br>";
            response.out() << "Dlr: " << it->second.deliveres << "<br>";
            //        response.out() << "ADT: " << it->second.deliverytime << "<br>";
            response.out() << "DLV (" << it->second.deliveres*100 / ( it->second.requests == 0 ? 1: it->second.requests ) << "%)" << "<br>";
            response.out() << "</td>";
        }
        response.out() << "</tr>";

        response.out() << "<tr>";
        response.out() << "<td>";
        response.out() << "1 Hour stats: ";
        response.out() << "</td>";
        for ( StatManager::gNamePropMap::iterator it = s1hour.begin(); it != s1hour.end(); it++ ) {
            response.out() << "<td>";
            response.out() << "[" << it->first<< "]<br>";
            response.out() << "RQT:   " << it->second.requests << "<br>";
            response.out() << "ACK:        " << it->second.acks << "<br>";
            response.out() << "Rsp:  " << it->second.responses << "<br>";
            response.out() << "Dlr: " << it->second.deliveres << "<br>";
            //        response.out() << "ADT: " << it->second.deliverytime << "<br>";
            response.out() << "DLV (" << it->second.deliveres*100 / ( it->second.requests == 0 ? 1: it->second.requests ) << "%)" << "<br>";
            response.out() << "</td>";
        }
        response.out() << "</tr>";

        response.out() << "<tr>";
        response.out() << "<td>";
        response.out() << "1 Day stats: ";
        response.out() << "</td>";
        for ( StatManager::gNamePropMap::iterator it = s1day.begin(); it != s1day.end(); it++ ) {
            response.out() << "<td>";
            response.out() << "[" << it->first<< "]<br>";
            response.out() << "RQT:   " << it->second.requests << "<br>";
            response.out() << "ACK:        " << it->second.acks << "<br>";
            response.out() << "Rsp:  " << it->second.responses << "<br>";
            response.out() << "Dlr: " << it->second.deliveres << "<br>";
            //        response.out() << "ADT: " << it->second.deliverytime << "<br>";
            response.out() << "DLV (" << it->second.deliveres*100 / ( it->second.requests == 0 ? 1: it->second.requests ) << "%)" << "<br>";
            response.out() << "</td>";
        }
        response.out() << "</tr>";

        response.out() << "</table>";
        response.out() << "</pre>";

    } catch (...) {}

}
