/*
	nymph_message.cpp	- Implements the NymphRPC Message class.
	
	Revision 0
	
	Notes:
			- 
			
	History:
	2017/06/24, Maya Posch : Initial version.
	
	(c) Nyanko.ws
*/

#include "nymph_message.h"
#include "nymph_utilities.h"
#include "nymph_logger.h"

#include <sstream>
#include <algorithm>
#include <cstdlib>

// debug
#include <iostream>

using namespace std;

#include <Poco/NumberFormatter.h>

using namespace Poco;


// --- CONSTRUCTOR ---
NymphMessage::NymphMessage() {
	flags = 0;
	state = 0;
	response = 0;
	responseId = 0;
	hasResult = false;
	responseOwned = false;
	loggerName = "NymphMessage";
}


NymphMessage::NymphMessage(uint32_t methodId) {
	flags = 0;
	state = 0; // no error
	response = 0;
	responseId = 0;
	hasResult = false;
	responseOwned = false;
	this->methodId = methodId;
	loggerName = "NymphMessage";
}


// Deserialises a binary Nymph message.
NymphMessage::NymphMessage(string* binmsg) {
	flags = 0;
	state = 0; // no error
	response = 0;
	responseId = 0;
	hasResult = false;
	responseOwned = false;
	loggerName = "NymphMessage";
	uint64_t binLength = binmsg->length();
	
	// The string we receive here is stripped of the Nymph header (0x4452474e, 'DRGN')
	// as well as the length of the message.
	// First we get the Nymph protocol version (0x00), then the command index number 
	// (<uint32>).
	uint8_t version = 0;
	methodId = 0;
	
	int index = 0;
	version = getUInt8(binmsg, index);
	methodId = getUInt32(binmsg, index);
	
	NYMPH_LOG_DEBUG("Method ID: " + NumberFormatter::formatHex(methodId) + ".");
	
	
	if (version != 0x00) {
		// Handle wrong version.
		NYMPH_LOG_ERROR("Wrong Nymph version: " + NumberFormatter::format(version) + ".");
		
		state = -1;
		return;
	}
	
	// Read the message flags.
	flags = getUInt32(binmsg, index);
	
	NYMPH_LOG_DEBUG("Message flags: 0x" + NumberFormatter::formatHex(flags));
	
	// Read the message ID & optionally the request message ID (if response).
	messageId = getUInt64(binmsg, index);
	
	uint8_t typecode;
	if (flags & NYMPH_MESSAGE_REPLY) {
		responseId = getUInt64(binmsg, index);
		
		// Read in the response
		typecode = getUInt8(binmsg, index);
		NymphUtilities::parseValue(typecode, binmsg, index, response);
		
		if (index >= binLength) {
			// Out of bounds, abort.
			NYMPH_LOG_ERROR("Message parsing index out of bounds. Abort.");
			return;
		}
		
		if ((*binmsg)[index] != NYMPH_TYPE_NONE) {
			// We didn't reach the message end. 
			// FIXME: handle this case, maybe do some pre-flight checks.
		}
	}
	else if (flags & NYMPH_MESSAGE_EXCEPTION) {
		responseId = getUInt64(binmsg, index);
		
		// Read in the exception (integer, string).
		typecode = getUInt8(binmsg, index);
		NymphType* value;
		NymphUtilities::parseValue(typecode, binmsg, index, value);
		if (value->type() == NYMPH_UINT32) {
			exception.id = ((NymphUint32*) value)->getValue();
		}
		
		typecode = getUInt8(binmsg, index);
		NymphUtilities::parseValue(typecode, binmsg, index, value);
		if (value->type() == NYMPH_STRING) {
			exception.value = ((NymphString*) value)->getValue();
		}
	}
	else if (flags & NYMPH_MESSAGE_CALLBACK) {
		// Read in the name of the callback method.
		typecode = getUInt8(binmsg, index);
		NymphType* value = 0;
		NymphUtilities::parseValue(typecode, binmsg, index, value);
		if (value && value->type() == NYMPH_STRING) {
			callbackName = ((NymphString*) value)->getValue();
		}
	}
	else {		
		// Read in the parameter values.
		NymphType* value;
		while ((*binmsg)[index] != NYMPH_TYPE_NONE) {
			if (index >= binLength) {
				NYMPH_LOG_ERROR("Reached end of message without terminator found.");
				NYMPH_LOG_ERROR("Message is likely corrupt.");
				
				// TODO: set a 'corrupt' flag or such for the message.
				
				break;
			}
			
			typecode = getUInt8(binmsg, index);
			NymphUtilities::parseValue(typecode, binmsg, index, value);
			values.push_back(value);
		}
	}
}


// --- DECONSTRUCTOR ---
// Delete all values stored in this message since we have taken ownership.
NymphMessage::~NymphMessage() {
	uint64_t l = values.size();
	for (uint64_t i = 0; i < l; ++i) {
		if (values[i]) {
			delete values[i];
		}
	}
	
	if (responseOwned && response) {
		delete response;
	}
	
	values.clear();
}


// --- ADD VALUE ---
// Adds a new value to this message. Message takes ownership of value.
bool NymphMessage::addValue(NymphType* value) {
	values.push_back(value);
	
	return true;
}


// --- FINISH ---
// Finish the header contents, merge with contents. Make result available.
bool NymphMessage::finish(string &output) {
	uint8_t nymphNone = NYMPH_TYPE_NONE;
	
	NYMPH_LOG_DEBUG("Serialising message with flags: 0x" + NumberFormatter::formatHex(flags));
	
	// Header section.
	uint32_t signature = 0x4452474e; // 'DRGN'
	uint32_t length = 0; // Filled in later.
	uint8_t version = 0x00;
	
	// Content
	string content;
	if (flags & NYMPH_MESSAGE_REPLY) {
		content = response->serialize();
	}
	else {
		unsigned int valueLen = values.size();
		for (unsigned int i = 0; i < valueLen; ++i) {
			content += values[i]->serialize();
		}
	}
	
	// If the first message in a series, add a new messageId.
	if (!(flags & NYMPH_MESSAGE_REPLY)) { messageId = NymphUtilities::getMessageId(); }
	
	// If the message is a callback, prepare it for serialisation.
	string cbnStr;
	if (flags & NYMPH_MESSAGE_CALLBACK) {
		NymphString cbn(callbackName);
		cbnStr = cbn.serialize();
	}
	
	// Message length is always:
	// * 1 byte from the header (version).
	// * 4 bytes method ID.
	// * 4 bytes message flags.
	// * 8 bytes message ID.
	// * 1 byte from the terminating typecode (Nymph None).
	// ===
	// 18 bytes + other values size.
	// 
	// For a response message, add another 8 bytes to the length. (incl. exceptions).
	// For a callback message, add 1 byte + callback name length.
	length = (uint32_t) (content.length() + 18);
	if (flags & NYMPH_MESSAGE_REPLY) { length += 8; }
	if (flags & NYMPH_MESSAGE_EXCEPTION) { length += 8; }
	else if (flags & NYMPH_MESSAGE_CALLBACK) {
		length += cbnStr.length();
	}
	
	// Write the header contents.
	// FIXME: On a big-endian system all integers will be in the wrong byte order.
	// TODO: add endianness-check. Currently assume LE.
	string signatureStr, lengthStr, methodIdStr, msgIdStr;
	string replyStr, flagsStr;
	signatureStr = string(((const char*) &signature), 4);
	lengthStr = string(((const char*) &length), 4);	
	methodIdStr = string(((const char*) &methodId), 4);
	flagsStr = string(((const char*) &flags), 4);
	msgIdStr = string(((const char*) &messageId), 8);
	if (flags & NYMPH_MESSAGE_REPLY || flags & NYMPH_MESSAGE_EXCEPTION) {
		replyStr = string(((const char*) &responseId), 8);
	}
	
	output = signatureStr;
	output += lengthStr;
	output += string(((const char*) &version), 1);
	output += methodIdStr;
	output += flagsStr;
	output += msgIdStr;
	if (flags & NYMPH_MESSAGE_REPLY || flags & NYMPH_MESSAGE_EXCEPTION) {
		output += replyStr;
	}
	else if (flags & NYMPH_MESSAGE_CALLBACK) {
		output += cbnStr;
	}
	
	output += content;	
	output += string(((const char*) &nymphNone), 1);
	
	return true;
}


// --- SET IN REPLY TO ---
// Set the message ID we're responding to with this message. Set the 'reply' bitflag, too.
void NymphMessage::setInReplyTo(uint64_t msgId) {
	
	//NYMPH_LOG_DEBUG("Current message flags: 0x" + NumberFormatter::formatHex(flags));
	
	responseId = msgId;
	messageId = msgId + 1;
	
	
	NYMPH_LOG_DEBUG("New message flags: 0x" + NumberFormatter::formatHex(flags));
}


// --- SET RESULT VALUE ---
// Sets the result value for a response message. Message takes ownership of value.
void NymphMessage::setResultValue(NymphType* value) {
	flags |= NYMPH_MESSAGE_REPLY;
	response = value;
	responseOwned = true;
}


// --- GET REPLY MESSAGE ---
// Returns a new Nymph message instance, prefilled with just the 'responseId' and
// 'messageId' values.
NymphMessage* NymphMessage::getReplyMessage() {
	NymphMessage* msg = new NymphMessage(methodId);
	msg->setInReplyTo(messageId);
	return msg;
}


// --- SET EXCEPTION ---
// Set the value for an exception to be sent with the message.
// This is generally used by the receiving side to indicate an issue with the
// message itself, or a service being accessed.
// The message instance takes ownership of the value.
bool NymphMessage::setException(int exceptionId, string value) {
	flags |= NYMPH_MESSAGE_EXCEPTION;
	values.push_back(new NymphUint32(exceptionId));
	values.push_back(new NymphString(value));
	
	return true;
}


// --- SET CALLBACK ---
// Enable the status to that of a callback message (server->client).
bool NymphMessage::setCallback(string name) {
	flags |= NYMPH_MESSAGE_CALLBACK;
	callbackName = name;
	return true;
}
