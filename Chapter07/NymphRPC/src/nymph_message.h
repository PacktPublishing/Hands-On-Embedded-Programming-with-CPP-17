/*
	nymph_message.h	- Declares the NymphRPC Message class.
	
	Revision 0
	
	Notes:
			- 
			
	2017/06/24, Maya Posch : Initial version.
	(c) Nyanko.ws
*/


#pragma once
#ifndef NYMPH_MESSAGE_H
#define NYMPH_MESSAGE_H

#include "nymph_types.h"

#include <Poco/Poco.h>

#include <vector>

using namespace std;


enum {
	NYMPH_MESSAGE_REPLY = 0x01,		// Message is a reply.
	NYMPH_MESSAGE_EXCEPTION = 0x02,	// Message is an exception.
	NYMPH_MESSAGE_CALLBACK = 0x04	// Message is a callback.
};


struct NymphException {
	uint32_t id;
	string value;
};


class NymphMessage {
	vector<NymphType*> values;
	uint32_t command;
	uint32_t flags;
	uint32_t methodId;
	int state;
	uint64_t messageId;
	uint64_t responseId;
	NymphException exception;
	bool hasResult;
	bool responseOwned;
	string callbackName;
	NymphType* response;
	string loggerName;
	
public:
	NymphMessage();
	NymphMessage(uint32_t methodId);
	NymphMessage(string* binmsg);
	~NymphMessage();
	bool addValue(NymphType* value);
	bool finish(string &output);
	int getState() { return state; }
	void setInReplyTo(uint64_t msgId);
	bool isCallback() { return flags & NYMPH_MESSAGE_CALLBACK; }
	uint64_t getResponseId() { return responseId; }
	uint64_t getMessageId() { return messageId; }
	void setResultValue(NymphType* value);
	NymphType* getResponse() { return response; responseOwned = false; }
	vector<NymphType*> parameters() { return values; }
	uint32_t getMethodId() { return methodId; }
	NymphMessage* getReplyMessage();
	NymphException getException() { return exception; }
	string getCallbackName() { return callbackName; }
	bool isReply() { return flags & NYMPH_MESSAGE_REPLY; }
	bool isException() { return flags & NYMPH_MESSAGE_EXCEPTION; }
	bool setException(int exceptionId, string value);
	bool setCallback(string name);
};

#endif
