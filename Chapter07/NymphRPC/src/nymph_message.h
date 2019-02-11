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


enum {
	NYMPH_MESSAGE_REPLY = 0x01,		// Message is a reply.
	NYMPH_MESSAGE_EXCEPTION = 0x02,	// Message is an exception.
	NYMPH_MESSAGE_CALLBACK = 0x04	// Message is a callback.
};


struct NymphException {
	uint32_t id;
	std::string value;
};


class NymphMessage {
	std::vector<NymphType*> values;
	uint32_t command;
	uint32_t flags;
	uint32_t methodId;
	int state;
	uint64_t messageId;
	uint64_t responseId;
	NymphException exception;
	bool hasResult;
	bool responseOwned;
	std::string callbackName;
	NymphType* response;
	std::string loggerName;
	
public:
	NymphMessage();
	NymphMessage(uint32_t methodId);
	NymphMessage(std::string* binmsg);
	~NymphMessage();
	bool addValue(NymphType* value);
	bool finish(std::string &output);
	int getState() { return state; }
	void setInReplyTo(uint64_t msgId);
	bool isCallback() { return flags & NYMPH_MESSAGE_CALLBACK; }
	uint64_t getResponseId() { return responseId; }
	uint64_t getMessageId() { return messageId; }
	void setResultValue(NymphType* value);
	NymphType* getResponse() { return response; responseOwned = false; }
	std::vector<NymphType*> parameters() { return values; }
	uint32_t getMethodId() { return methodId; }
	NymphMessage* getReplyMessage();
	NymphException getException() { return exception; }
	std::string getCallbackName() { return callbackName; }
	bool isReply() { return flags & NYMPH_MESSAGE_REPLY; }
	bool isException() { return flags & NYMPH_MESSAGE_EXCEPTION; }
	bool setException(int exceptionId, std::string value);
	bool setCallback(std::string name);
};

#endif
