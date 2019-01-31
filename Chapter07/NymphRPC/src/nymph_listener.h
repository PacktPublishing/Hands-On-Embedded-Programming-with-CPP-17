/*
	nymph_listener.h	- Declares the NymphRPC Listener class.
	
	Revision 0
	
	Notes:
			- 
			
	History:
	2017/06/24, Maya Posch	: Initial version.
	
	(c) Nyanko.ws
*/


#pragma once
#ifndef NYMPH_LISTENER_H
#define NYMPH_LISTENER_H


#include <vector>
#include <string>

using namespace std;

#include <Poco/Mutex.h>

#include "nymph_socket_listener.h"


// TYPES

typedef void (*NymphCallbackMethod)(NymphMessage* msg, void* data);


struct NymphCallback {
	string name;				// Callback method name.
	NymphCallbackMethod method;	// The callback.
	void* data;					// Custom user data.
};


// ---


class NymphListener {
	static map<int, NymphSocketListener*> listeners;
	static Poco::Mutex listenersMutex;
	static string loggerName;
	
	static map<string, NymphCallback>& callbacks();
	static Poco::Mutex& callbacksMutex();
	
public:
	static void stop();
	
	static bool addConnection(int handle, NymphSocket socket);
	static bool removeConnection(int handle);
	static bool addMessage(NymphRequest* &request);
	static bool removeMessage(int handle, int64_t messageId);
	static bool addCallback(NymphCallback callback);
	static bool callCallback(NymphMessage* msg, void* data);
	static bool removeCallback(string name);
};

#endif
