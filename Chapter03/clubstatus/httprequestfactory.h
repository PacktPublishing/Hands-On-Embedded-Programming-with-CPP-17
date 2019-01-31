/*
	httprequestfactory.h - Header for the ClubStatus' request handler factory.
	
	Revision 0
	
	Notes:
			- 
	
	2018/02/28, Maya Posch
*/


#pragma once
#ifndef HTTPREQUESTFACTORY_H
#define HTTPREQUESTFACTORY_H

#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>

using namespace Poco::Net;

#include "statushandler.h"
#include "datahandler.h"


class RequestHandlerFactory: public HTTPRequestHandlerFactory { 
public:
	RequestHandlerFactory() {}
	HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request) {
		if (request.getURI().compare(0, 12, "/clubstatus/") == 0) { 
			return new StatusHandler(); } 
		else { 
			return new DataHandler(); 
		}
	}
};

#endif
