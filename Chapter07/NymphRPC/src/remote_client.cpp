/*
	remote_client.h	- header file for the NymphRPC Remote Client class.
	
	Revision 0
	
	Notes:
			- This class declares the main class to be used by Nymph clients.
			
	2017/06/24, Maya Posch <posch@synyx.de>	: Initial version.
	(c) Nyanko.ws
*/

#include "remote_client.h"
#include "nymph_server.h"


#include <Poco/Net/NetException.h>
#include <Poco/NumberFormatter.h>

using namespace Poco;

#include "remote_server.h"
#include "nymph_utilities.h"

using namespace std;


// Static initialisations
Poco::Mutex NymphRemoteClient::methodsMutex;
Poco::Mutex NymphRemoteClient::callbacksMutex;
Poco::Mutex NymphRemoteClient::sessionsMutex;
long NymphRemoteClient::timeout = 3000;
UInt32 NymphRemoteClient::nextMethodId = 0;
bool NymphRemoteClient::synced = false;
string NymphRemoteClient::serializedMethods;
string NymphRemoteClient::loggerName = "NymphRemoteClient";
map<int, NymphSession*> NymphRemoteClient::sessions;


// -- CALLBACKS ---
map<string, NymphMethod>& NymphRemoteClient::callbacks() {
	static map<string, NymphMethod>* callbacksStatic = new map<string, NymphMethod>();
	return *callbacksStatic;
}


// -- METHODS ---
map<string, NymphMethod>& NymphRemoteClient::methods() {
	static map<string, NymphMethod>* methodsStatic = new map<string, NymphMethod>();
	return *methodsStatic;
}


// --- METHODS HASHES ---
map<UInt32, NymphMethod*>& NymphRemoteClient::methodsIds() {
	static map<UInt32, NymphMethod*>* methodsIdsStatic = new map<UInt32, NymphMethod*>();
	return *methodsIdsStatic;
}


// --- SYNC METHODS ---
// Callback for the built-in sync method. Returns a Nymph message containing
// the list of custom methods.
NymphMessage* NymphRemoteClient::syncMethods(int session, NymphMessage* msg, void* data) {
	NYMPH_LOG_DEBUG("Sync method called by client...");
	if (!synced) {
		// Create updated serialized methods table.
		static map<UInt32, NymphMethod*> &methodsIdsStatic = NymphRemoteClient::methodsIds();
		methodsMutex.lock();
		map<UInt32, NymphMethod*>::iterator it;
		serializedMethods = "METHODS";
		UInt32 size = (UInt32) methodsIdsStatic.size();
		serializedMethods += string(((char*) &size), 4);
		for (it = methodsIdsStatic.begin(); it != methodsIdsStatic.end(); ++it) {
			serializedMethods += it->second->getSerialized();
		}
		
		methodsMutex.unlock();
	}
	
	// Prepare return message.
	NymphMessage* returnMsg = msg->getReplyMessage();
	NymphString* methodsStr = new NymphString(serializedMethods);
	returnMsg->setResultValue(methodsStr);
	return returnMsg;
}


// --- INIT ---
// Initialise the runtime.
bool NymphRemoteClient::init(logFnc logger, int level, long timeout) {
	NymphRemoteClient::timeout = timeout;
	setLogger(logger, level);
	
	
	// Register built-in synchronisation method ('nymphsync').
	vector<NymphTypes> parameters;
	NymphMethod syncFunction("nymphsync", parameters, NYMPH_STRING);
	syncFunction.setCallback(syncMethods);
	NymphRemoteClient::registerMethod("nymphsync", syncFunction);
	
	return true;
}


// --- SET LOGGER ---
// Sets the logger function to be used by the Nymph Logger class, along with the
// desired maximum log level:
// 0: Fatal
// 1: Critical
// 2: Errors
// 3: Warnings
// 4: Notice
// 5: Information
// 6: Debug
// 7: Trace
void NymphRemoteClient::setLogger(logFnc logger, int level) {
	NymphLogger::setLoggerFunction(logger);
	Poco::Message::Priority prio;
	switch (level) {
		case 0:
			prio = Poco::Message::PRIO_FATAL;
			break;
		case 1:
			prio = Poco::Message::PRIO_CRITICAL;
			break;
		case 2:
			prio = Poco::Message::PRIO_ERROR;
			break;
		case 3:
			prio = Poco::Message::PRIO_WARNING;
			break;
		case 4:
			prio = Poco::Message::PRIO_NOTICE;
			break;
		case 5:
			prio = Poco::Message::PRIO_INFORMATION;
			break;
		case 6:
			prio = Poco::Message::PRIO_DEBUG;
			break;
		case 7:
			prio = Poco::Message::PRIO_TRACE;
			break;
		default:
			prio = Poco::Message::PRIO_TRACE;
			break;
	}
	
	NymphLogger::setLogLevel(prio);
}


// --- START ---
bool NymphRemoteClient::start(int port) {
	NymphServer::start(port);
	return true;
}


// --- SHUTDOWN ---
// Shutdown the runtime. Close any open connections and clean up resources.
bool NymphRemoteClient::shutdown() {
	NymphServer::stop();
	return true;
}


// --- REGISTER METHOD ---
bool NymphRemoteClient::registerMethod(string name, NymphMethod method) {
	static map<string, NymphMethod> &methodsStatic = NymphRemoteClient::methods();
	static map<UInt32, NymphMethod*> &methodsIdsStatic = NymphRemoteClient::methodsIds();
	methodsMutex.lock();
	method.setId(nextMethodId++);
	pair<map<string, NymphMethod>::iterator, bool> newPair;
	newPair = methodsStatic.insert(pair<string, NymphMethod>(name, method));
	
	// TODO: check whether a new method was inserted or just overwritten.
	// Create a reference to the method instance in the methods map here.
	methodsIdsStatic.insert(pair<UInt32, NymphMethod*>(method.getId(), &(newPair.first->second)));
	synced = false;
	methodsMutex.unlock();
	
	return true;
}


// --- CALL METHOD CALLBACK ---
bool NymphRemoteClient::callMethodCallback(int handle, UInt32 methodId, NymphMessage* msg, NymphMessage* &response) {
	static map<UInt32, NymphMethod*> &methodsIdsStatic = NymphRemoteClient::methodsIds();
	methodsMutex.lock();
	map<UInt32, NymphMethod*>::iterator it;
	it = methodsIdsStatic.find(methodId);
	if (it == methodsIdsStatic.end()) {
		NYMPH_LOG_ERROR("Specified method ID " + NumberFormatter::format(methodId) + " was not found.");
		methodsMutex.unlock();
		return false;
	}
	
	// Call the callback method.
	response = it->second->callCallback(handle, msg);
	methodsMutex.unlock();
	return true;
}


// --- REMOVE METHOD ---
bool NymphRemoteClient::removeMethod(string name) {
	static map<string, NymphMethod> &methodsStatic = NymphRemoteClient::methods();
	static map<UInt32, NymphMethod*> &methodsIdsStatic = NymphRemoteClient::methodsIds();
	methodsMutex.lock();
	map<string, NymphMethod>::iterator it;
	it = methodsStatic.find(name);
	UInt32 id = it->second.getId();
	if (it != methodsStatic.end()) {
		methodsStatic.erase(it);
	}
	
	map<UInt32, NymphMethod*>::iterator mit;
	mit = methodsIdsStatic.find(id);
	if (mit != methodsIdsStatic.end()) {
		methodsIdsStatic.erase(mit);
	}
	
	methodsMutex.unlock();
	
	return true;
}


// --- REGISTER CALLBACK ---
bool NymphRemoteClient::registerCallback(string name, NymphMethod method) {
	static map<string, NymphMethod> &callbacksStatic = NymphRemoteClient::callbacks();
	callbacksMutex.lock();
	method.enableCallback();
	pair<map<string, NymphMethod>::iterator, bool> newPair;
	newPair = callbacksStatic.insert(pair<string, NymphMethod>(name, method));
	callbacksMutex.unlock();
	
	return true;
}


// --- CALL CALLBACK ---
bool NymphRemoteClient::callCallback(int handle, string name, 
									vector<NymphType*> &values, string &result) {
	map<int, NymphSession*>::iterator it;
	sessionsMutex.lock();
	it = sessions.find(handle);
	if (it == sessions.end()) { 
		result = "Provided handle was not found.";
		sessionsMutex.unlock();
		return false; 
	}
	
	NYMPH_LOG_DEBUG("Calling callback method: " + name);
	
	// Get the callback.
	static map<string, NymphMethod> &callbacksStatic = NymphRemoteClient::callbacks();
	callbacksMutex.lock();
	map<string, NymphMethod>::iterator mit;
	mit = callbacksStatic.find(name);
	if (mit == callbacksStatic.end()) {
		result = "Specified method name was not found.";
		callbacksMutex.unlock();
		sessionsMutex.unlock();
		return false;
	}
	
	bool ret = mit->second.call(it->second, values, result);
	callbacksMutex.unlock();
	sessionsMutex.unlock();
	
	if (!ret) {
		NYMPH_LOG_ERROR("Calling callback method failed: " + result);
		return false; 
	}
	
	return true;
}


// --- REMOVE CALLBACK ---
bool NymphRemoteClient::removeCallback(string name) {
	static map<string, NymphMethod> &callbacksStatic = NymphRemoteClient::callbacks();
	callbacksMutex.lock();
	map<string, NymphMethod>::iterator it;
	it = callbacksStatic.find(name);
	if (it != callbacksStatic.end()) {
		callbacksStatic.erase(it);
	}
	
	callbacksMutex.unlock();
	
	return true;
}


// --- ADD SESSION ---
bool NymphRemoteClient::addSession(int handle, NymphSession* session) {
	sessionsMutex.lock();
	sessions.insert(pair<int, NymphSession*>(handle, session));
	sessionsMutex.unlock();
	
	return true;
}


// --- REMOVE SESSION ---
bool NymphRemoteClient::removeSession(int handle) {
	map<int, NymphSession*>::iterator it;
	sessionsMutex.lock();
	it = sessions.find(handle);
	if (it == sessions.end()) {
		sessionsMutex.unlock();
		return false;
	}
	
	sessions.erase(it);
	sessionsMutex.unlock();
	
	return true;
}
