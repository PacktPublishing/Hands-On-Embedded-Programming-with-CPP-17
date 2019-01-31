/*
	nymph_method.cpp	- Implements the NymphRPC Method class.
	
	Revision 0
	
	Notes:
			- 
			
	History:
	2017/06/24, Maya Posch : Initial version.
	
	(c) Nyanko.ws
*/


#include "nymph_method.h"
#include "nymph_utilities.h"
#include "nymph_logger.h"
#include "nymph_listener.h"

#include <vector>
#include <sstream>
#include <algorithm>
#include <cstdlib>

using namespace std;

#include <Poco/NumberFormatter.h>

using namespace Poco;


// --- DUMMY CALLBACK ---
// Should never be called, but makes error handling/debugging a lot easier
// when a callback on a server application isn't set by the user.
NymphMessage* dummyCallback(int session, NymphMessage* msg, void* data) {
	string loggerName = "NymphMethod";
	NYMPH_LOG_ERROR("Dummy callback called for message. Message dropped.");
	return new NymphMessage();
}


// --- CONSTRUCTOR ---
// NymphMethod constructor.
NymphMethod::NymphMethod(string name, vector<NymphTypes> parameters, NymphTypes retType) {
	this->name = name;
	this->parameters = parameters;
	this->returnType = retType;
	this->callback = &dummyCallback;
	this->isCallback = false;
	loggerName = "NymphMethod";
}


// --- SET ID ---
// Sets the ID for this method. This ID is used as reference between clients and
// the server. 
// This method also generates a serialised version of the method, to be used 
// with the method sync functionality with clients.
void NymphMethod::setId(UInt32 id) {
	this->id = id;
	
	// Serialise this method.
	serialized = "METHOD";
	serialized += string(((char*) &id), 4);
	UInt8 length = (UInt8) this->name.length();
	serialized += string(((char*) &length), 1);
	serialized += this->name;
	length = (UInt8) this->parameters.size();
	serialized += string(((char*) &length), 1);
	for (int i = 0; i < length; ++i) {
		UInt8 type = (UInt8) parameters.at(i);
		serialized += string(((char*) &type), 1);
	}
	
	UInt8 type = (UInt8) returnType;
	serialized += string(((char*) &type), 1);
}


// --- SET CALLBACK ---
// Sets the callback method associated with this method. Used on the server side.
// The specified callback method should handle the provided message and deliver
// an appropriate response.
void NymphMethod::setCallback(NymphMethodCallback callback) {
	this->callback = callback;
}


// --- CALL CALLBACK ---
NymphMessage* NymphMethod::callCallback(int handle, NymphMessage* msg) {
	NYMPH_LOG_DEBUG("Calling callback for method: " + name);
	return callback(handle, msg, 0);
}


// --- CALL ---
// Call this method instance. Validates the input values, composes message,
// serialises message and sends it using the provided socket.
bool NymphMethod::call(Net::StreamSocket* socket, NymphRequest* &request, vector<NymphType*> &values, string &result) {
	// For each item in the values vector, match its type with the registered
	// signature type (NymphTypes enum).
	// If the types match, serialise the values NymphType instance and insert it
	// into a new NymphMessage.
	int vl = values.size();
	int pl = parameters.size();
	if (vl != pl) {
		result = "Provided value array length does not match method signature.";
		
		// Delete the values in the values vector since we own them.
		vector<NymphType*>::iterator it;
		for (it = values.begin(); it != values.end(); ++it) { delete (*it); }
		return false;
	}
	
	NymphMessage msg(id);
	if (isCallback) {
		msg.setCallback(name);
	}
	
	for (int i = 0; i < vl; ++i) {
		if (values[i]->type() != parameters[i] && parameters[i] != NYMPH_ANY) {
			stringstream ss;
			ss << "Type mismatch on parameter " << i << " for method " << name << ". "
				<< "Expected: " << parameters[i] << ", got: " << values[i]->type() << ".";
			result = ss.str();
			return false;
		}
		
		msg.addValue(values[i]);
	}
	
	// Obtain binary message.
	string binmsg;
	msg.finish(binmsg);
	
	// Finish the NymphRequest instance and add it to the listener.
	request->messageId = msg.getMessageId();
	NymphListener::addMessage(request);
	
	// Send the message.
	try {
		int ret = socket->sendBytes(((const void*) binmsg.c_str()), binmsg.length());
		if (ret != binmsg.length()) {
			// Handle error.
			result = "Failed to send message: ";		
			return false;
		}
		
		NYMPH_LOG_DEBUG("Sent " + NumberFormatter::format(ret) + " bytes.");
	}
	catch (Poco::Exception &e) {
		result = "Failed to send message: " + e.message();
		return false;
	}
	
	return true;
}


// Call the method instance, using an NymphSession instance.
bool NymphMethod::call(NymphSession* session, vector<NymphType*> &values, string &result) {
	// For each item in the values vector, match its type with the registered
	// signature type (NymphTypes enum).
	// If the types match, serialise the values NymphType instance and insert it
	// into a new NymphMessage.
	int vl = values.size();
	int pl = parameters.size();
	if (vl != pl) {
		result = "Provided value array length does not match method signature.";
		
		// Delete the values in the values vector since we own them.
		vector<NymphType*>::iterator it;
		for (it = values.begin(); it != values.end(); ++it) { delete (*it); }
		return false;
	}
	
	NymphMessage msg(id);
	if (isCallback) {
		msg.setCallback(name);
	}
	
	for (int i = 0; i < vl; ++i) {
		if (values[i]->type() != parameters[i] && parameters[i] != NYMPH_ANY) {
			stringstream ss;
			ss << "Type mismatch on parameter " << i << " for method " << name << ". "
				<< "Expected: " << parameters[i] << ", got: " << values[i]->type() << ".";
			result = ss.str();
			return false;
		}
		
		msg.addValue( values[i]);
	}
	
	// Obtain binary message.
	string binmsg;
	msg.finish(binmsg);
	
	// Send the message.
	if (!session->send(binmsg, result)) { return false; }
	
	return true;
}
