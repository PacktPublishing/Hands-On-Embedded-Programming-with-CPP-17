/*
	nymph_method.h	- header file for the NympRPC hMethod class.
	
	Revision 0
	
	Notes:
			- 
			
	History:
	2017/06/24, Maya Posch : Initial version.
	
	(c) Nyanko.ws
*/


#pragma once
#ifndef NYMPH_METHOD_H
#define NYMPH_METHOD_H

#include "nymph_types.h"
#include "nymph_listener.h"
#include "nymph_message.h"
#include "nymph_session.h"

#include <Poco/Poco.h>
#include <Poco/Net/StreamSocket.h>

#include <vector>
#include <string>


class NymphRemoteClient;


typedef NymphMessage* (*NymphMethodCallback)(int session, NymphMessage* msg, void* data);


class NymphMethod {
	friend class NymphRemoteClient;
	friend class NymphRemoteServer;
	
	std::string name;
	uint32_t id;
	std::vector<NymphTypes> parameters;
	NymphMethodCallback callback;
	NymphTypes returnType;
	std::string loggerName;
	std::string serialized;
	bool isCallback;
	
	void setId(uint32_t id);
	
public:
	NymphMethod(std::string name, std::vector<NymphTypes> parameters, NymphTypes retType);
	void setCallback(NymphMethodCallback callback);
	NymphMessage* callCallback(int handle, NymphMessage* msg);
	bool call(Poco::Net::StreamSocket* socket, NymphRequest* &request, std::vector<NymphType*> &values, std::string &result);
	bool call(NymphSession* session, std::vector<NymphType*> &values, std::string &result);
	uint32_t getId() { return id; }
	std::string getSerialized() { return serialized; }
	bool enableCallback(bool state = true) { isCallback = state; return true; }
};

#endif
