/*
	statushandler.h - Header file for the StatusHandler class.
	
	Revision 0
	
	Notes:
			- 
			
	2018/02/28, Maya Posch
*/


#ifndef STATUSHANDLER_H
#define STATUSHANDLER_H

#include <iostream>
#include <vector>

using namespace std;

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/URI.h>

using namespace Poco;
using namespace Poco::Net;

#include "club.h"


class StatusHandler: public HTTPRequestHandler { 
public: 
	void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) {
		// Process the request. Valid API calls:
		//
		// * GET /clubstatus
		// -> Returns the current club status.
		
		Club::log(LOG_INFO, "StatusHandler: Request from " + request.clientAddress().toString());
		
		URI uri(request.getURI());
		vector<string> parts;
		uri.getPathSegments(parts);
		
		response.setContentType("application/json"); // JSON mime-type
		response.setChunkedTransferEncoding(true); 
		
		// First path segment is 'clubstatus' (see RequestHandlerFactory::createRequestHandler())
		if (parts.size() == 1) {
			response.setStatus(HTTPResponse::HTTP_OK);
			ostream& ostr = response.send();
			ostr << "{ \"clubstatus\": " << !Club::currentStatusSwitchValue << ",";
			ostr << "\"lock\": " << Club::currentLockSwitchValue << ",";
			ostr << "\"power\": " << Club::powerOn << "}";
		}
		else {
			response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
			ostream& ostr = response.send();
			ostr << "{ \"error\": \"Invalid request.\" }";
		}
	}
};

#endif
