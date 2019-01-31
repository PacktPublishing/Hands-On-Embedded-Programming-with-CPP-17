/*
	nymph_test_server.cpp - Test server application using the NymphRPC library.
	
	Revision 0
	
	Features:
				- 
				
	Notes:
				-
				
	2017/06/24, Maya Posch	: Initial version.
	(c) Nyanko.ws
*/


#include "../../src/nymph.h"


#include <iostream>
#include <vector>
#include <csignal>

using namespace std;


#include <Poco/Condition.h>
#include <Poco/Thread.h>

using namespace Poco;


Condition gCon;
Mutex gMutex;
//volatile sig_atomic_t gSignalTrigger = 0;


void signal_handler(int signal) {
	//gSignalTrigger = 1;
	gCon.signal();
}


// --- LOG FUNCTION ---
void logFunction(int level, string logStr) {
	cout << level << " - " << logStr << endl;
}


// Callback for helloFunction.
NymphMessage* hello(int session, NymphMessage* msg, void* data) {
	cout << "Received message for session: " << session << ", msg ID: " << msg->getMessageId() << "\n";
	
	string echoStr = ((NymphString*) msg->parameters()[0])->getValue();
	cout << "Message string: " << echoStr << "\n";
	
	NymphMessage* returnMsg = msg->getReplyMessage();
	NymphString* world = new NymphString(echoStr);
	returnMsg->setResultValue(world);
	return returnMsg;
}


// Callback for helloCallback. Called to register a client-side callback which 
// we will call right afterwards.
NymphMessage* helloCallback(int session, NymphMessage* msg, void* data) {
	cout << "Received message for session: " << session << ", msg ID: " << msg->getMessageId() << "\n";
	
	string echoStr = ((NymphString*) msg->parameters()[0])->getValue();
	cout << "Client callback method name: " << echoStr << "\n";
	
	// Register and call the callback method ('callbackFunction') on the client.
	vector<NymphTypes> parameters;
	parameters.push_back(NYMPH_STRING);
	NymphMethod remoteMethod(echoStr, parameters, NYMPH_NULL);
	remoteMethod.enableCallback();
	NymphRemoteClient::registerCallback(echoStr, remoteMethod);
	
	vector<NymphType*> values;
	values.push_back(new NymphString(echoStr));
	string result;
	NymphMessage* returnMsg = msg->getReplyMessage();
	NymphBoolean* world = 0;
	if (!NymphRemoteClient::callCallback(session, echoStr, values, result)) {
		cerr << "Calling callback failed: " << result << endl;
		world = new NymphBoolean(false);
	}
	else { world = new NymphBoolean(true); }
	
	returnMsg->setResultValue(world);
	return returnMsg;
}


int main() {
	// Initialise the server instance.
	cout << "Initialising server...\n";
	long timeout = 5000; // 5 seconds.
	NymphRemoteClient::init(logFunction, NYMPH_LOG_LEVEL_TRACE, timeout);
	
	// Register methods to expose to the clients.
	cout << "Registering methods...\n";
	vector<NymphTypes> parameters;
	parameters.push_back(NYMPH_STRING);
	NymphMethod helloFunction("helloFunction", parameters, NYMPH_STRING);
	helloFunction.setCallback(hello);
	NymphRemoteClient::registerMethod("helloFunction", helloFunction);
	
	NymphMethod helloCallbackFunction("helloCallbackFunction", parameters, NYMPH_BOOL);
	helloCallbackFunction.setCallback(helloCallback);
	NymphRemoteClient::registerMethod("helloCallbackFunction", helloCallbackFunction);
	
	// Install signal handler to terminate the server.
	signal(SIGINT, signal_handler);
	
	// Start server on port 4004.
	NymphRemoteClient::start(4004);
	
	// Loop until the SIGINT signal has been received.
	//while (gSignalTrigger == 0) { }
	gMutex.lock();
	gCon.wait(gMutex);
	
	// Clean-up
	NymphRemoteClient::shutdown();
	
	// Wait before exiting, giving threads time to exit.
	Thread::sleep(2000); // 2 seconds.
	
	return 0;
}
