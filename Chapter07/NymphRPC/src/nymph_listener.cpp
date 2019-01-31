/*
	nymph_listener.cpp	- Implements the NymphRPC Listener class.
	
	Revision 0
	
	Notes:
			- 
			
	History:
	2017/06/24, Maya Posch : Initial version.
	
	(c) Nyanko.ws
*/


#include "nymph_listener.h"
#include "nymph_logger.h"

#include <iostream>
#include <cstdlib>

using namespace std;

#include <Poco/Thread.h>
#include <Poco/NumberFormatter.h>
#include <Poco/Condition.h>

using namespace Poco;


// Static initialisations.
map<int, NymphSocketListener*> NymphListener::listeners;
Mutex NymphListener::listenersMutex;
string NymphListener::loggerName = "NymphListener";


// --- CALLBACKS ---
map<string, NymphCallback>& NymphListener::callbacks() {
	static map<string, NymphCallback>* callbacksStatic = new map<string, NymphCallback>();
	return *callbacksStatic;
}


// --- CALLBACKS MUTEX ---
Mutex& NymphListener::callbacksMutex() {
	static Mutex* callbacksMutexStatic = new Mutex;
	return *callbacksMutexStatic;
}


// --- STOP ---
void NymphListener::stop() {
	// Shut down all listening threads.
	listenersMutex.lock();
	for (int i = 0; i < listeners.size(); ++i) {
		listeners[i]->stop();
	}
	
	listenersMutex.unlock();
}


// --- ADD CONNECTION ---
bool NymphListener::addConnection(int handle, NymphSocket socket) {
	NYMPH_LOG_INFORMATION("Adding connection. Handle: " + NumberFormatter::format(handle) + ".");
	
	// Create new thread for NymphSocketListener instance which handles
	// the new socket. Save reference to this listener.
	Poco::Condition* cnd = new Poco::Condition;
	Poco::Mutex* mtx = new Poco::Mutex;
	long timeout = 1000; // 1 second
	mtx->lock();
	NymphSocketListener* esl = new NymphSocketListener(socket, cnd, mtx);
	Poco::Thread* thread = new Poco::Thread;
	thread->start(*esl);
	if (!cnd->tryWait(*mtx, timeout)) {
		// Handle listener timeout.
		NYMPH_LOG_ERROR("Creating of new listener thread timed out.");
		mtx->unlock();
		return false;
	}
	
	mtx->unlock();
	
	listenersMutex.lock();
	listeners.insert(std::pair<int, NymphSocketListener*>(handle, esl));
	listenersMutex.unlock();
	
	NYMPH_LOG_INFORMATION("Listening socket has been added.");
	
	// TODO: Request the method signatures from the server if configured to do so.
	
	
	return true;
}


// --- REMOVE CONNECTION ---
// Removes a connection using the Nymph connection handle.
bool NymphListener::removeConnection(int handle) {
	map<int, NymphSocketListener*>::iterator it;
	listenersMutex.lock();
	
	NYMPH_LOG_INFORMATION("Removing connection for handle: " + NumberFormatter::format(handle) + ".");
	
	it = listeners.find(handle);
	if (it == listeners.end()) { listenersMutex.unlock(); return true; }
	
	it->second->stop(); // Tell the listening thread to terminate.
	listeners.erase(it);
	
	NYMPH_LOG_INFORMATION("Listening socket has been removed.");
	
	listenersMutex.unlock();
	return true;
}


// --- ADD MESSAGE ---
bool NymphListener::addMessage(NymphRequest* &request) {
	NYMPH_LOG_INFORMATION("Adding request for message ID: " + NumberFormatter::formatHex(request->messageId) + ".");
	
	listenersMutex.lock();
	map<int, NymphSocketListener*>::iterator it;
	it = listeners.find(request->handle);
	if (it == listeners.end()) {
		NYMPH_LOG_ERROR("Handle " + NumberFormatter::format(request->handle) + " not found. Dropping message ID " + NumberFormatter::formatHex(request->messageId));
		listenersMutex.unlock();
		return false;
	}
	
	it->second->addMessage(request);
	
	listenersMutex.unlock();
	return true;
}


// --- REMOVE MESSAGE ---
bool NymphListener::removeMessage(int handle, Int64 messageId) {
	NYMPH_LOG_INFORMATION("Removing request for message ID: " + NumberFormatter::formatHex(messageId) + ".");
	
	listenersMutex.lock();
	map<int, NymphSocketListener*>::iterator it;
	it = listeners.find(handle);
	if (it == listeners.end()) {
		NYMPH_LOG_ERROR("Handle " + NumberFormatter::format(handle) + " not found. Dropping message ID " + NumberFormatter::formatHex(messageId));
		listenersMutex.unlock();
		return false;
	}
	
	it->second->removeMessage(messageId);
	
	listenersMutex.unlock();
	return true;
}


// --- ADD CALLBACK ---
bool NymphListener::addCallback(NymphCallback callback) {
	static map<string, NymphCallback> &callbacksStatic = NymphListener::callbacks();
	static Mutex& callbacksMutexStatic = NymphListener::callbacksMutex();
	callbacksMutexStatic.lock();
	
	// FIXME: ensure Poco logging is initialised before calling logging.
	//NYMPH_LOG_INFORMATION("Adding callback for method: " + callback.name + ".");
	
	callbacksStatic.insert(pair<string, NymphCallback>(callback.name, callback));
	callbacksMutexStatic.unlock();
	
	return true;
}


// --- CALL CALLBACK ---
bool NymphListener::callCallback(NymphMessage* msg, void* data) {
	static map<string, NymphCallback> &callbacksStatic = NymphListener::callbacks();
	static Mutex& callbacksMutexStatic = NymphListener::callbacksMutex();
	map<string, NymphCallback>::iterator cit;
	callbacksMutexStatic.lock();
	cit = callbacksStatic.find(msg->getCallbackName());
	if (cit == callbacksStatic.end()) {
		NYMPH_LOG_WARNING("Callback not found for method: "  
				+ msg->getCallbackName() + ".");
		
		callbacksMutexStatic.unlock();
		return false;				
	}
	
	NYMPH_LOG_INFORMATION("Found callback for method " + msg->getCallbackName() + ".");
	
	// We found a matching callback. Call it.
	// Use the provided data, else the default data.
	if (data) { (cit->second.method)(msg, data); }
	else { (cit->second.method)(msg, cit->second.data); }
	callbacksMutexStatic.unlock();
	
	return true;
}


// --- REMOVE CALLBACK ---
bool NymphListener::removeCallback(string name) {
	static map<string, NymphCallback> &callbacksStatic = NymphListener::callbacks();
	static Mutex& callbacksMutexStatic = NymphListener::callbacksMutex();
	map<string, NymphCallback>::iterator it;
	callbacksMutexStatic.lock();
	
	NYMPH_LOG_INFORMATION("Removing callback for method: " + name + ".");
	
	it = callbacksStatic.find(name);
	if (it != callbacksStatic.end()) { callbacksStatic.erase(it); }
	callbacksMutexStatic.unlock();
	return true;
}
