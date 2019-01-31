/*
	remote_server.h	- header file for the Remote Server class.
	
	Revision 0
	
	Notes:
			- This class declares the main class to be used by NymphRPC clients.
			
	2017/06/24, Maya Posch	: Initial version.	
	(c) Nyanko.ws
*/


#ifndef REMOTE_SERVER_H
#define REMOTE_SERVER_H


#include <vector>
#include <string>
#include <map>

#include <Poco/Semaphore.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/StreamSocket.h>

#include "nymph_method.h"
#include "nymph_listener.h"
#include "nymph_logger.h"

using namespace std;


class NymphRemoteServer {
	static map<int, Poco::Net::StreamSocket*> sockets;
	static map<int, Poco::Semaphore*> socketSemaphores;
	static Poco::Mutex socketsMutex;
	static int lastHandle;
	static long timeout;
	static string loggerName;
	static uint32_t nextMethodId;
	
	static map<string, NymphMethod>& methods();
	static map<uint32_t, NymphMethod*>& methodsIds();
	static Poco::Mutex& methodsMutex();
	
	static bool registerMethod(string name, NymphMethod method);
	
public:
	static bool init(logFnc logger, int level = NYMPH_LOG_LEVEL_TRACE, long timeout = 3000);
	static bool sync(int handle, string &result);
	static void setLogger(logFnc logger, int level);
	static bool shutdown();
	static bool connect(string host, int port, int &handle, void* data, string &result);
	static bool connect(string url, int &handle, void* data, string &result);
	static bool connect(Poco::Net::SocketAddress sa, int &handle, void* data, string &result);
	static bool disconnect(int handle, string &result);
	static bool callMethod(int handle, string name, vector<NymphType*> &values, 
										NymphType* &returnvalue, string &result);
	static bool callMethodId(int handle, uint32_t id, vector<NymphType*> &values, NymphType* &returnvalue, string &result);
	static bool removeMethod(string name);
	
	static bool registerCallback(string name, NymphCallbackMethod method, void* data);
	static bool removeCallback(string name);
};

#endif
