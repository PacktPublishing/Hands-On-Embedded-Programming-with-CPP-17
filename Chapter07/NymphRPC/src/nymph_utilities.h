/*
	nymph_utilities.h	- Declares the NymphRPC Utilities class.
	
	Revision 0
	
	Notes:
			- 
			
	History:
	2017/06/24, Maya Posch : Initial version.
	
	(c) Nyanko.ws
*/


#pragma once
#ifndef NYMPH_UTILITIES_H

#include "nymph_types.h"

#include <Poco/Poco.h>
#include <Poco/Mutex.h>
#include <Poco/Dynamic/Var.h>

#include <string>

class NymphUtilities {
	static int64_t messageId;
	static Poco::Mutex idMutex;	
	static std::string loggerName;
	
public:
	//static UInt32 createHash(std::string name);
	static int64_t getMessageId();
	static NymphType* dynamicVarToNymphType(Poco::Dynamic::Var &object);
	static bool parseValue(uint8_t typecode, std::string* binmsg, int &index, NymphType* &value);
};

#endif
