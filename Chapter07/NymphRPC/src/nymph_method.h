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

using namespace std;


class NymphRemoteClient;


typedef NymphMessage* (*NymphMethodCallback)(int session, NymphMessage* msg, void* data);


class NymphMethod {
	friend class NymphRemoteClient;
	friend class NymphRemoteServer;
	
	string name;
	uint32_t id;
	vector<NymphTypes> parameters;
	NymphMethodCallback callback;
	NymphTypes returnType;
	string loggerName;
	string serialized;
	bool isCallback;
	
	void setId(uint32_t id);
	
public:
	NymphMethod(string name, vector<NymphTypes> parameters, NymphTypes retType);
	void setCallback(NymphMethodCallback callback);
	NymphMessage* callCallback(int handle, NymphMessage* msg);
	bool call(Poco::Net::StreamSocket* socket, NymphRequest* &request, vector<NymphType*> &values, string &result);
	bool call(NymphSession* session, vector<NymphType*> &values, string &result);
	uint32_t getId() { return id; }
	string getSerialized() { return serialized; }
	bool enableCallback(bool state = true) { isCallback = state; return true; }
};

#endif
