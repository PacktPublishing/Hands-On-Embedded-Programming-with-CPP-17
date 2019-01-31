/*
	remote_client.h	- header file for the NymphRPC Remote Client class.
	
	Revision 0
	
	Notes:
			- This class declares the main class to be used by Nymph servers.
			
	2017/06/24, Maya Posch	: Initial version.
	(c) Nyanko.ws
*/


#pragma once
#ifndef NYMPH_REMOTE_CLIENT_H
#define NYMPH_REMOTE_CLIENT_H


#include <vector>
#include <string>
#include <map>

#include <Poco/Mutex.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/StreamSocket.h>

#include "nymph_method.h"
#include "nymph_listener.h"
#include "nymph_logger.h"
#include "nymph_session.h"

using namespace std;


class NymphRemoteClient {
	static Poco::Mutex methodsMutex;
	static Poco::Mutex callbacksMutex;
	static Poco::Mutex sessionsMutex;
	static map<int, NymphSession*> sessions;
	static long timeout;
	static string loggerName;
	static bool synced;
	static string serializedMethods;
	static uint32_t nextMethodId;
	
	static map<string, NymphMethod>& callbacks();
	static map<string, NymphMethod>& methods();
	static map<uint32_t, NymphMethod*>& methodsIds();
	
	static NymphMessage* syncMethods(int session, NymphMessage* msg, void* data);
	
public:
	static bool init(logFnc logger, int level = NYMPH_LOG_LEVEL_TRACE, long timeout = 3000);
	static void setLogger(logFnc logger, int level);
	static bool start(int port = 4004);
	static bool shutdown();
	static bool registerMethod(string name, NymphMethod method);
	static bool callMethodCallback(int handle, uint32_t methodId, NymphMessage* msg, NymphMessage* &response);
	static bool removeMethod(string name);
	
	static bool registerCallback(string name, NymphMethod method);
	static bool callCallback(int handle, string name, 
								vector<NymphType*> &values, string &result);
	static bool removeCallback(string name);
	
	static bool addSession(int handle, NymphSession* session);
	static bool removeSession(int handle);
};

#endif
