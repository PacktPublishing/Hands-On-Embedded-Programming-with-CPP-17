/*
	nymph_socket_listener.h	- Declares the NymphRPC Socket Listener class.
	
	Revision 0
	
	Notes:
			- 
			
	History:
	2017/06/24, Maya Posch : Initial version.
	
	(c) Nyanko.ws
*/


#pragma once
#ifndef NYMPH_SOCKET_LISTENER_H
#define NYMPH_SOCKET_LISTENER_H

#include "nymph_message.h"

#include <Poco/Runnable.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Semaphore.h>
#include <Poco/Condition.h>

#include <map>
#include <string>

// TYPES

struct NymphSocket {
	Poco::Net::StreamSocket* socket;	// Pointer to the socket instance.
	Poco::Semaphore* semaphore;		// Signals when it's safe to delete the socket.
	void* data;					// User data.
	int handle;					// The Nymph internal socket handle.
};


struct NymphRequest {
	int handle;
	uint64_t messageId;
	Poco::Mutex mutex;
	Poco::Condition condition;
	NymphType* response;
	bool exception;
	NymphException exceptionData;
};

// ---


class NymphSocketListener : public Poco::Runnable {
	std::string loggerName;
	bool listen;
	NymphSocket nymphSocket;
	Poco::Net::StreamSocket* socket;
	std::map<uint64_t, NymphRequest*> messages;
	Poco::Mutex messagesMutex;
	bool init;
	Poco::Condition* readyCond;
	Poco::Mutex* readyMutex;
	
public:
	NymphSocketListener(NymphSocket socket, Poco::Condition* cond, Poco::Mutex* mtx);
	~NymphSocketListener();
	void run();
	void stop();
	bool addMessage(NymphRequest* &request);
	bool removeMessage(uint64_t messageId);
};

#endif
