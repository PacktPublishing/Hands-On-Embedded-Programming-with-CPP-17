/*
	nymph_utilities.cpp	- Implements the NymphRPC Utilities class.
	
	Revision 0
	
	Notes:
			- 
			
	History:
	2017/06/24, Maya Posch : Initial version.
	
	(c) Nyanko.ws
*/


#include "nymph_utilities.h"
#include "nymph_logger.h"

#include <cstring>
#include <cstdlib>
#include <iostream>

using namespace std;

#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Object.h>
#include <Poco/NumberFormatter.h>

using namespace Poco;


// Static initialisations.
Int64 NymphUtilities::messageId = 1;
Mutex NymphUtilities::idMutex;
string NymphUtilities::loggerName = "NymphUtilities";


// --- GET MESSAGE ID ---
Int64 NymphUtilities::getMessageId() {
	Int64 temp;
	idMutex.lock();
	temp = messageId++;
	idMutex.unlock();
	return temp;
}


// --- JSON OBJECT TO NYMPH TYPE ---
NymphType* NymphUtilities::dynamicVarToNymphType(Dynamic::Var &object) {
	if (object.isString()) {
		NYMPH_LOG_INFORMATION("Converting JSON string to Nymph string.");
		string out;
		try { object.convert(out); }
		catch (...) {
			NYMPH_LOG_ERROR("Failed to convert dynamic variable to string.");
			return 0;
		}
		
		return (NymphType*) new NymphString(out);
	} 
	else if (object.isBoolean()) {
		NYMPH_LOG_INFORMATION("Converting JSON boolean to Nymph boolean.");
		bool out;
		try { object.convert(out); }
		catch (...) {
			NYMPH_LOG_ERROR("Failed to convert dynamic variable to boolean.");
			return 0;
		}
		
		return (NymphType*) new NymphBoolean(out);
	} 
	else if (object.isInteger()) {
		NYMPH_LOG_INFORMATION("Converting JSON integer to Nymph integer.");
		Int32 out;
		try { object.convert(out); }
		catch (...) {
			NYMPH_LOG_ERROR("Failed to convert dynamic variable to integer.");
			return 0;
		}
		
		return (NymphType*) new NymphUint32(out);
	}  
	else if (object.isNumeric()) {
		NYMPH_LOG_INFORMATION("Converting JSON double to Nymph double.");
		double out;
		try { object.convert(out); }
		catch (...) {
			NYMPH_LOG_ERROR("Failed to convert dynamic variable to double.");
			return 0;
		}
		
		return (NymphType*) new NymphDouble(out);
	}
	
	return 0;
}


// --- PARSE VALUE ---
// Parses the value section of a message's key/value pair. Determines the value
// type and uses the appropriate NymphType to parse the value.
bool NymphUtilities::parseValue(UInt8 typecode, string* binmsg, int &index, NymphType* &value) {
	//NYMPH_LOG_DEBUG("parseValue called with typecode: " + NumberFormatter::formatHex(typecode) + ".");
	
	switch (typecode) {
        case NYMPH_TYPE_NULL:
			NYMPH_LOG_DEBUG("NYMPH_TYPE_NONE");
			value = 0;
			break;
        case NYMPH_TYPE_BOOLEAN_FALSE:
			NYMPH_LOG_DEBUG("NYMPH_TYPE_BOOLEAN_FALSE");
			value = new NymphBoolean(false);
			break;
        case NYMPH_TYPE_BOOLEAN_TRUE:
			NYMPH_LOG_DEBUG("NYMPH_TYPE_BOOLEAN_TRUE");
			value = new NymphBoolean(true);
			break;
		case NYMPH_TYPE_FLOAT: {
			NYMPH_LOG_DEBUG("NYMPH_TYPE_FLOAT");
			/* float v = 0;
			for (int i = 0; i < 4; ++i) {
				v = (UInt32) v | (((UInt8) binmsg[index++]) << ((3 - i) * 8));
			} */
			
			value = new NymphFloat(binmsg, index);
			break;
		}
		case NYMPH_TYPE_DOUBLE:
			NYMPH_LOG_DEBUG("NYMPH_TYPE_DOUBLE");
			value = new NymphDouble(binmsg, index);
			break;
        case NYMPH_TYPE_UINT8:
			NYMPH_LOG_DEBUG("NYMPH_TYPE_UINT8");
            value = new NymphUint8(binmsg, index);
            break;
		case NYMPH_TYPE_SINT8:
			NYMPH_LOG_DEBUG("NYMPH_TYPE_SINT8");
			value = new NymphSint8(binmsg, index);
			break;
		case NYMPH_TYPE_UINT16:
			NYMPH_LOG_DEBUG("NYMPH_TYPE_UINT16");
			value = new NymphUint16(binmsg, index);
			break;
		case NYMPH_TYPE_SINT16:
			NYMPH_LOG_DEBUG("NYMPH_TYPE_SINT16");
			value = new NymphSint16(binmsg, index);
			break;
		case NYMPH_TYPE_UINT32:
			NYMPH_LOG_DEBUG("NYMPH_TYPE_UINT32");
			value = new NymphUint32(binmsg, index);
			break;
		case NYMPH_TYPE_SINT32:
			NYMPH_LOG_DEBUG("NYMPH_TYPE_SINT32");
			value = new NymphSint32(binmsg, index);
			break;
		case NYMPH_TYPE_UINT64:
			NYMPH_LOG_DEBUG("NYMPH_TYPE_UINT64");
			value = new NymphUint64(binmsg, index);
			break;
		case NYMPH_TYPE_SINT64:
			NYMPH_LOG_DEBUG("NYMPH_TYPE_SINT64");
			value = new NymphSint64(binmsg, index);
			break;
        case NYMPH_TYPE_EMPTY_STRING:
			NYMPH_LOG_DEBUG("NYMPH_TYPE_EMPTY_STRING");
			value = new NymphString(string());
			break;
        case NYMPH_TYPE_STRING:
			NYMPH_LOG_DEBUG("NYMPH_TYPE_STRING");
			value = new NymphString(binmsg, index);
            break;
        case NYMPH_TYPE_ARRAY: {
			NYMPH_LOG_DEBUG("NYMPH_TYPE_ARRAY");
			value = new NymphArray;
			value->deserialize(binmsg, index);
            break;
		}
		case NYMPH_TYPE_STRUCT:
			NYMPH_LOG_DEBUG("NYMPH_TYPE_STRUCT");
			value = new NymphStruct();
			value->deserialize(binmsg, index);
			break;
        default:
			NYMPH_LOG_DEBUG("Default case. And nothing happened.");
		
            // Assume it's a single byte value.
			/* UInt8 v = 0;
			if (typecode >= NYMPH_TYPE_MIN_TINY_INT || 
					typecode <= NYMPH_TYPE_MAX_TINY_INT) {
                // fits within range: interpret as value
                v = (UInt8) typecode;
            }
			
			NYMPH_LOG_DEBUG("Value: " + NumberFormatter::formatHex(v) + ".");
			
			value = new NymphInt(v); */
    }
	
	return true;
}
