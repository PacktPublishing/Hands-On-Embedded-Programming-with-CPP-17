/*
	nymph_types.cpp	- Defines the NymphRPC data types.
	
	Revision 0
	
	Notes:
			- 
			
	History:
	2017/06/24, Maya Posch : Initial version.
	
	(c) Nyanko.ws
*/


#include "nymph_types.h"
#include "nymph_utilities.h"
#include "nymph_logger.h"

#include <sstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>

using namespace std;

#include <Poco/NumberFormatter.h>
#include <Poco/JSON/Stringifier.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Base64Decoder.h>
#include <Poco/Exception.h>

using namespace Poco;


// >>> NYMPH ARRAY <<<
// 

// --- DECONSTRUCTOR ---
NymphArray::~NymphArray() {
	for (int i = 0; i < values.size(); ++i) {
		delete values[i];
	}
}

// --- ADD VALUE ---
void NymphArray::addValue(NymphType* value) {
	values.push_back(value); 
	isEmpty = false;
	binSize += value->binarySize();
	//if (value->type() == NYMPH_ARRAY) { ++dimension; }
}

// --- TO STRING ---
string NymphArray::toString(bool quotes) {
	// TODO: implement
	return string();
}

// ---- SERIALIZE ---
string NymphArray::serialize() {
	string out;
	out.reserve(3 + binSize); // type & terminator + size.
	uint8_t typecode = NYMPH_TYPE_ARRAY;
	out.append(((const char*) &typecode), 1);
	
	uint64_t valueCount = (uint64_t) values.size();
	out.append(((const char*) &valueCount), 8);
	
	vector<NymphType*>::iterator it;
	for (it = values.begin(); it != values.end(); ++it) {
		out += (*it)->serialize();
	}
	
	typecode = NYMPH_TYPE_NONE;
	out.append(((const char*) &typecode), 1);
	
	return out;
}


// --- DESERIALIZE ---
bool NymphArray::deserialize(string* binary, int &index) {
	string loggerName = "NymphTypes";
	//NYMPH_LOG_DEBUG("NYMPH_TYPE_ARRAY");
	// An ARRAY type consists out of a length (uint64_t),
	// followed by typecode/value sequences. It ends with a NONE
	// typecode.
	
	// Size of the array, in number of elements.
	uint64_t numElements = getUInt64(binary, index);
	
	NYMPH_LOG_DEBUG("Array size: " + NumberFormatter::format(numElements) + " elements.");
	
	values.reserve(numElements);
	
	// Now parse the elements.
	uint8_t typecode = 0;
	for (uint64_t i = 0; i < numElements; ++i) {
		NYMPH_LOG_TRACE("Parsing array element " + NumberFormatter::format(i) + " of " + NumberFormatter::format(numElements) + " - Index: " + NumberFormatter::format(index) + ".");
		typecode = getUInt8(binary, index);
		NymphType* elVal = 0;
		NymphUtilities::parseValue(typecode, binary, index, elVal);
		if (elVal) {
			values.push_back(elVal);
			binSize += elVal->binarySize();
		}
	}
	
	typecode = getUInt8(binary, index);
	if (typecode != NYMPH_TYPE_NONE) {
		NYMPH_LOG_ERROR("Array terminator was not found where expected.");
	}
	
	return true;
}


// >>> NYMPH BOOLEAN <<<
// In the Nymph protocol, boolean is defined as two types:
// * Boolean false (0x02)
// * Boolean true (0x03)
// Thus by reading the type one also knows its value. For compatibility with
// the C/C++ boolean type, and the programmer's sanity, here both types are merged
// into a singular class.
/* NymphBoolean::NymphBoolean(bool value) {
	this->value = value;
	isEmpty = false;
}*/


NymphBoolean::NymphBoolean(string value) {
	int index = 0;
	deserialize(&value, index);
}


// --- TO STRING ---
string NymphBoolean::toString(bool quotes) {
	string out = "";
	if (quotes) { out += "\""; }
	if (value) { out += "true"; } else { out += "false"; }
	if (quotes) { out += "\""; }
	
	return out;
}

void NymphBoolean::setValue(bool value) { this->value = value; }
bool NymphBoolean::getValue() { return value; }


// --- SERIALIZE ---
string NymphBoolean::serialize() {
	string out;
	out.reserve(1);
	int8_t typecode = 0;
	if (value) { typecode = NYMPH_TYPE_BOOLEAN_TRUE; }
	else { typecode = NYMPH_TYPE_BOOLEAN_FALSE; }
	
	out.append(((const char*) &typecode), 1);
	return out;
}


// --- DESERIALIZE ---
// Input is a single-character string. Read it and convert it to the proper
// boolean value.
bool NymphBoolean::deserialize(string* binary, int &index) {
	isEmpty = false;
	if (binary->length() < 1) { return false; }
	unsigned char ch = (*binary)[index];
	if (ch == NYMPH_TYPE_BOOLEAN_FALSE) { value = false; }
	else if (ch == NYMPH_TYPE_BOOLEAN_TRUE) { value = true; }
	else { return false; }
	
	++index;
	return true;
}


// >>> NYMPH STRING <<<
// The Nymph string can have two forms:
// * Empty string (0x92)
// * String (0x93)
// Further format:
// - length: integer type. uint8_t is either length (within range), or defines
// the integer type (short, int, long) followed by the length in that type.
// Finally the length number of bytes follow.
// For the sanity of developers, both types of strings are merged into 
// this singular class.
NymphString::NymphString(string value) {
	this->value = value;
	isEmpty = false;
	emptyString = false;
	binSize = 0;
} 

// --- TO STRING ---
string NymphString::toString(bool quotes) {
	string out = "";
	if (quotes) { out += "\""; }
	out += value;
	if (quotes) { out += "\""; }
	
	return out;
}


// --- SET VALUE ---
void NymphString::setValue(string value) { 
	this->value = value; 
	isEmpty = false; 
	
	uint64_t length = value.length();
	if (length <= 0xFF) { binSize = 3 + length; }
	else if (length <= 0xFFFF) { binSize = 4 + length; }
	else if (length <= 0xFFFFFFFF) { binSize = 6 + length; }
	else { binSize = 10 + length; }
}


// ---- SERIALIZE ---
string NymphString::serialize() {
	string out;
	uint8_t strType;
	if (emptyString) {
		strType = NYMPH_TYPE_EMPTY_STRING;
		out.append(((const char*) &strType), 1);
		return out;
	}
	else {
		strType = NYMPH_TYPE_STRING;		
		uint64_t length = value.length();
		uint8_t typecode = 0;
		if (length <= 0xFF) {
			out.reserve(3 + value.length());
			out.append(((const char*) &strType), 1);
			typecode = NYMPH_TYPE_UINT8;			
			out.append(((const char*) &typecode), 1);
			uint8_t l = length;
			out.append(((const char*) &l), 1);
		}
		else if (length <= 0xFFFF) {
			out.reserve(4 + value.length());
			out.append(((const char*) &strType), 1);
			typecode = NYMPH_TYPE_UINT16;			
			out.append(((const char*) &typecode), 1);
			uint16_t l = length;
			out.append(((const char*) &l), 2);
		}
		else if (length <= 0xFFFFFFFF) {
			out.reserve(6 + value.length());
			out.append(((const char*) &strType), 1);
			typecode = NYMPH_TYPE_UINT32;			
			out.append(((const char*) &typecode), 1);
			uint8_t l = length;
			out.append(((const char*) &l), 4);
		}
		else {
			out.reserve(10 + value.length());
			out.append(((const char*) &strType), 1);
			typecode = NYMPH_TYPE_UINT64;			
			out.append(((const char*) &typecode), 1);
			uint8_t l = length;
			out.append(((const char*) &l), 8);
		}
		
		out += value;
	}
	
	return out;
}


// --- DESERIALIZE ---
bool NymphString::deserialize(string* binary, int &index) {
	string loggerName = "NymphTypes";
	uint8_t typecode = 0;
	typecode = getUInt8(binary, index);
	uint64_t l = 0;
	switch (typecode) {
		 case NYMPH_TYPE_UINT8: {
			l = getUInt8(binary, index);
		 }
            break;
		case NYMPH_TYPE_UINT16: {
			l = getUInt16(binary, index);
		}
			break;
		case NYMPH_TYPE_UINT32: {
			l = getUInt32(binary, index);
		}
			break;
		case NYMPH_TYPE_UINT64: {
			l = getUInt64(binary, index);
		}
			break;
		default:
			NYMPH_LOG_ERROR("Not a valid integer type for string length.");
			return false;
	}
	
	value = binary->substr(index, l);
	index += l;
	
	NYMPH_LOG_DEBUG("String value: " + value + ".");
	
	isEmpty = false;
	emptyString = false;
	
	// Set size of the serialised message.
	uint64_t length = value.length();
	if (length <= 0xFF) { binSize = 3 + length; }
	else if (length <= 0xFFFF) { binSize = 4 + length; }
	else if (length <= 0xFFFFFFFF) { binSize = 6 + length; }
	else { binSize = 10 + length; }
	
	return true;
}


// >>> NYMPH DOUBLE <<<
// 64-bit floating point type.

// --- TO STRING ---
string NymphDouble::toString(bool quotes) {
	string out = "";
	if (quotes) { out += "\""; }
	out += Poco::NumberFormatter::format(value);
	if (quotes) { out += "\""; }
	
	return out;
}

// ---- SERIALIZE ---
string NymphDouble::serialize() {
	string out;
	out.reserve(9);
	uint8_t typecode = NYMPH_TYPE_DOUBLE;
	out.append(((const char*) &typecode), 1);
	
	string valStr(((const char*) &value), 8);
	reverse(valStr.begin(), valStr.end());
	out += valStr;
	return out;
}


// --- DESERIALIZE ---
bool NymphDouble::deserialize(string* binary, int &index) {
	value = *((double*) &((*binary)[index]));
	index += 8;
	return true;
}


// >>> NYMPH FLOAT <<<
// 32-bit floating point type.

// --- TO STRING ---
string NymphFloat::toString(bool quotes) {
	string out = "";
	if (quotes) { out += "\""; }
	out += Poco::NumberFormatter::format(value);
	if (quotes) { out += "\""; }
	
	return out;
}

// ---- SERIALIZE ---
string NymphFloat::serialize() {
	string out;
	out.reserve(5);
	uint8_t typecode = NYMPH_TYPE_FLOAT;
	out.append(((const char*) &typecode), 1);
	
	string valStr(((const char*) &value), 4);
	reverse(valStr.begin(), valStr.end());
	out += valStr;
	return out;
}


// --- DESERIALIZE ---
bool NymphFloat::deserialize(string* binary, int &index) {
	value = *((float*) &((*binary)[index]));
	index += 4;
	return true;
}


// >>> NYMPH INTEGERS <<<
// Signed and unsigned 8 through 32-bit integer types.

// >> UINT 8 <<
// --- TO STRING ---
string NymphUint8::toString(bool quotes) {
	string out = "";
	if (quotes) { out += "\""; }
	out += Poco::NumberFormatter::format(value);
	if (quotes) { out += "\""; }
	
	return out;
}

// ---- SERIALIZE ---
string NymphUint8::serialize() {
	string out;
	out.reserve(2);
	uint8_t typecode = NYMPH_TYPE_UINT8;
	out.append(((const char*) &typecode), 1);
	
	out.append(((const char*) &value), 1);
	return out;
}


// --- DESERIALIZE ---
bool NymphUint8::deserialize(string* binary, int &index) {
	value = getUInt8(binary, index);
	return true;
}


// >> SINT 8 <<
// --- TO STRING ---
string NymphSint8::toString(bool quotes) {
	string out = "";
	if (quotes) { out += "\""; }
	out += Poco::NumberFormatter::format(value);
	if (quotes) { out += "\""; }
	
	return out;
}

// ---- SERIALIZE ---
string NymphSint8::serialize() {
	string out;
	out.reserve(2);
	uint8_t typecode = NYMPH_TYPE_SINT8;
	out.append(((const char*) &typecode), 1);
	out.append(((const char*) &value), 1);
	return out;
}


// --- DESERIALIZE ---
bool NymphSint8::deserialize(string* binary, int &index) {
	value = getSInt8(binary, index);
	return true;
}


// >> UINT 16 <<
// --- TO STRING ---
string NymphUint16::toString(bool quotes) {
	string out = "";
	if (quotes) { out += "\""; }
	out += Poco::NumberFormatter::format(value);
	if (quotes) { out += "\""; }
	
	return out;
}

// ---- SERIALIZE ---
string NymphUint16::serialize() {
	string out;
	out.reserve(3);
	uint8_t typecode = NYMPH_TYPE_UINT16;
	out.append(((const char*) &typecode), 1);
	out.append(((const char*) &value), 2);
	return out;
}


// --- DESERIALIZE ---
bool NymphUint16::deserialize(string* binary, int &index) {
	value = getUInt16(binary, index);
	return true;
}


// >> SINT 16 <<
// --- TO STRING ---
string NymphSint16::toString(bool quotes) {
	string out = "";
	if (quotes) { out += "\""; }
	out += Poco::NumberFormatter::format(value);
	if (quotes) { out += "\""; }
	
	return out;
}

// ---- SERIALIZE ---
string NymphSint16::serialize() {
	string out;
	out.reserve(3);
	uint8_t typecode = NYMPH_TYPE_SINT16;
	out.append(((const char*) &typecode), 1);
	out.append(((const char*) &value), 2);
	return out;
}


// --- DESERIALIZE ---
bool NymphSint16::deserialize(string* binary, int &index) {
	value = getSInt16(binary, index);
	return true;
}


// >> UINT 32 <<
// --- TO STRING ---
string NymphUint32::toString(bool quotes) {
	string out = "";
	if (quotes) { out += "\""; }
	out += Poco::NumberFormatter::format(value);
	if (quotes) { out += "\""; }
	
	return out;
}

// ---- SERIALIZE ---
string NymphUint32::serialize() {
	string out;
	out.reserve(5);
	uint8_t typecode = NYMPH_TYPE_UINT32;
	out.append(((const char*) &typecode), 1);
	out.append(((const char*) &value), 4);
	return out;
}


// --- DESERIALIZE ---
bool NymphUint32::deserialize(string* binary, int &index) {
	value = getUInt32(binary, index);
	return true;
}


// >> SINT 32 <<
// --- TO STRING ---
string NymphSint32::toString(bool quotes) {
	string out = "";
	if (quotes) { out += "\""; }
	out += Poco::NumberFormatter::format(value);
	if (quotes) { out += "\""; }
	
	return out;
}

// ---- SERIALIZE ---
string NymphSint32::serialize() {
	string out;
	out.reserve(5);
	uint8_t typecode = NYMPH_TYPE_SINT32;
	out.append(((const char*) &typecode), 1);
	out.append(((const char*) &value), 4);
	return out;
}


// --- DESERIALIZE ---
bool NymphSint32::deserialize(string* binary, int &index) {
	value = getSInt32(binary, index);
	return true;
}


// >> UINT 64 <<
// --- TO STRING ---
string NymphUint64::toString(bool quotes) {
	string out = "";
	if (quotes) { out += "\""; }
	out += Poco::NumberFormatter::format(value);
	if (quotes) { out += "\""; }
	
	return out;
}

// ---- SERIALIZE ---
string NymphUint64::serialize() {
	string out;
	out.reserve(9);
	uint8_t typecode = NYMPH_TYPE_UINT64;
	out.append(((const char*) &typecode), 1);
	out.append(((const char*) &value), 8);
	return out;
}


// --- DESERIALIZE ---
bool NymphUint64::deserialize(string* binary, int &index) {
	value = getUInt64(binary, index);
	return true;
}


// >> SINT 64 <<
// --- TO STRING ---
string NymphSint64::toString(bool quotes) {
	string out = "";
	if (quotes) { out += "\""; }
	out += Poco::NumberFormatter::format(value);
	if (quotes) { out += "\""; }
	
	return out;
}

// ---- SERIALIZE ---
string NymphSint64::serialize() {
	string out;
	out.reserve(9);
	uint8_t typecode = NYMPH_TYPE_SINT64;
	out.append(((const char*) &typecode), 1);
	out.append(((const char*) &value), 8);
	return out;
}


// --- DESERIALIZE ---
bool NymphSint64::deserialize(string* binary, int &index) {
	value = getSInt64(binary, index);
	return true;
}


// >>> NYMPH STRUCT <<<
// A struct type is a collection of key/value pairs.

// --- TO STRING ---
string NymphStruct::toString(bool quotes) {
	string out = "";
	if (quotes) { out += "\""; }
	out += "unimplemented";
	if (quotes) { out += "\""; }
	
	return out;
}

// ---- SERIALIZE ---
string NymphStruct::serialize() {
	string out;
	out.reserve(2 + binSize); // type & terminator + size.
	uint8_t typecode = NYMPH_TYPE_STRUCT;
	out.append(((const char*) &typecode), 1);
	
	uint32_t length = pairs.size();
	for (uint32_t i = 0; i < length; ++i) {
		out += pairs[i].key->serialize();
		out += pairs[i].value->serialize();
	}
	
	typecode = NYMPH_TYPE_NONE;
	out += string(((const char*) &typecode), 1);
	
	return out;
}


// --- DESERIALIZE ---
bool NymphStruct::deserialize(string* binary, int &index) {
	// Read pairs until NONE type has been found.
	// FIXME: check that we're not running out of bytes to read.
	while ((*binary)[index] != NYMPH_TYPE_NONE) {
		if ((*binary)[index] != NYMPH_TYPE_STRING) { return false; }
		uint8_t typecode = getUInt8(binary, index);
		NymphPair p;
		if (!NymphUtilities::parseValue(typecode, binary, index, p.key)) { return false; }
		typecode = getUInt8(binary, index);
		if (!NymphUtilities::parseValue(typecode, binary, index, p.value)) { return false; }
	}
	
	return true;
}


// >>> NYMPH TABLE <<<
// This type is basically an array of values, with the possibility of adding
// sub-levels with an additional Nymph Array level.

// --- DECONSTRUCTOR ---
/* NymphTable::~NymphTable() {
	for (int i = 0; i < values.size(); ++i) {
		delete values[i];
	}
}

// --- TO STRING ---
string NymphTable::toString(bool quotes) {
	string out = "";
	if (quotes) { out += "\""; }
	out += getJson();
	if (quotes) { out += "\""; }
	
	return out;
}

// ---- SERIALIZE ---
string NymphTable::serialize() {
	string out;
	uint8_t typecode = 0;
	typecode = NYMPH_TYPE_CUSTOM;
	out = string(((const char*) &typecode), 1);
	typecode = NYMPH_TYPE_INT;
	out += string(((const char*) &typecode), 1);
	uint32_t nameHash = 0x7feae04b; // Nymph Hashmap name hash
	string h = string(((const char*) &nameHash), 4);
	reverse(h.begin(), h.end());
	out += h;
	
	uint8_t length = 1;
	out += string(((const char*) &length), 1);
	out += string(((const char*) &typecode), 1);
	uint32_t keyHash = 0xee9ff760; // 'keysAndValues' hash.
	h = string(((const char*) &keyHash), 4);
	reverse(h.begin(), h.end());
	out += h;
	
	// Start array key/value pair.
	
	typecode = NYMPH_TYPE_ARRAY;
	out += string(((const char*) &typecode), 1);
	typecode = NYMPH_TYPE_ANY;
	out += string(((const char*) &typecode), 1);
	uint8_t dim = 1;
	out += string(((const char*) &dim), 1);
	typecode = NYMPH_TYPE_SHORT;
	out += string(((const char*) &typecode), 1);
	int16_t pairCount = values.size();
	h = string(((const char*) &pairCount), 2);
	reverse(h.begin(), h.end());
	out += h;
		
	vector<NymphType*>::iterator it;
	for (it = values.begin(); it != values.end(); ++it) {
		out += (*it)->serialize();
	}
	
	typecode = NYMPH_TYPE_NONE; 	// end array
	out += string (((const char*) &typecode), 1);
	typecode = NYMPH_TYPE_NONE;	// end custom.
	out += string (((const char*) &typecode), 1);
	
	return out;
}


// --- DESERIALIZE ---
bool NymphTable::deserialize(string binary) {
	// TODO: 
	
	return true;
}


// --- GET JSON OBJECT ---
// Returns a JSON Object representation of the table.
JSON::Object::Ptr NymphTable::getJsonObject(string &result) {
	JSON::Object::Ptr root = new JSON::Object();
	vector<NymphType*>::iterator it;
	for (it = values.begin(); it != values.end(); ++it) {
    	string key = (*it)->toString(false);
    	if (!key.empty()) {
			//IPCE_DEBUG("key: %s (%d)", key.c_str(), key.length());
			NymphType* value = *(++it);

			switch(value->type()) {
				case NYMPH_STRING:
					root->set(key, ((NymphString*) value)->getValue());
					break;
				case NYMPH_BOOL:
					root->set(key, ((NymphBoolean*) value)->getValue());
					break;
				case NYMPH_BYTE:
					root->set(key, ((NymphByte*) value)->getValue());
					break;
				case NYMPH_SHORT:
					root->set(key, ((NymphShort*) value)->getValue());
					break;
				case NYMPH_INT:
					root->set(key, ((NymphInt*) value)->getValue());
					break;
				case NYMPH_TABLE:
					root->set(key, ((NymphTable*) value)->getJsonObject(result));
					break;
				case NYMPH_ARRAY:
					// TODO
					break;
				default:
					result = "Unknown Nymph type.";
					continue; // skip this pair.
			}
    	}
		else {
			result = "Key was an empty string.";
		}
	}
	
	return root;
}


// --- GET JSON ---
// Returns a JSON representation of the stored table.
//
// The table is stored as an interleaved key/value format. The output JSON
// has each key as a string, with the value in its original format.
bool NymphTable::getJson(string &json, string &result) {
	JSON::Object::Ptr root = getJsonObject(result);
	ostringstream oss;
	JSON::Stringifier::stringify(root, oss);
	json = oss.str();
	
	return true;
}


string NymphTable::getJson() {
	string result;
	JSON::Object::Ptr root = getJsonObject(result);
	ostringstream oss;
	JSON::Stringifier::stringify(root, oss);
	return oss.str();
}


// --- SET JSON ---
// Parses the provided JSON string into the internal table representation.
bool NymphTable::setJson(string &json, string &result) {
	string loggerName = "NymphTypes";
	isEmpty = false;
	JSON::Parser parser;
	Dynamic::Var res;
	try {
		res = parser.parse(json);
	}
	catch (SyntaxException& e) {
		result = "JSON decoding failed.";
		NYMPH_LOG_ERROR("JSON decoding error: " + e.message());
		return false;
	}
	catch (JSON::JSONException& e) {
		result = "JSON decoding failed.";
		NYMPH_LOG_ERROR("JSON decoding error: " + e.message());
		return false;
	}
	catch (...) {
		result = "JSON decoding failed.";
		NYMPH_LOG_ERROR("JSON decoding error.");
		return false;
	}
	
	JSON::Object::Ptr object;
	try {
		object = res.extract<JSON::Object::Ptr>();
	}
	catch (BadCastException& e) {
		result = "Failed to extract JSON object.";
		NYMPH_LOG_ERROR(result + ". " + e.message());
		return false;
	}
	
	return setJsonObject(object, result);
}


// --- SET JSON OBJECT ---
bool NymphTable::setJsonObject(JSON::Object::Ptr &json, string &result) {
	string loggerName = "NymphTypes";
	isEmpty = false;
	
	NYMPH_LOG_INFORMATION("Creating new hash table.");
	JSON::Object::ConstIterator it;
	for (it = json->begin(); it != json->end(); ++it) {
		Dynamic::Var key = it->first; // Get the object key.
		NymphInt* k = new NymphInt(atoi(key.toString().c_str()));
		values.push_back(k);
		
		Dynamic::Var value = it->second; // Get the object value.
		if (value.isString()) {
			NYMPH_LOG_DEBUG("Converting string value...");
			string v;
			try { it->second.convert<string>(v); }
			catch (...) {
				result = "Exception converting to string.";
				NYMPH_LOG_ERROR("Exception converting to string.");
				return false; 
			}
			
			NymphString* es = new NymphString(v);			
			if (v.length() < 500) {
				NYMPH_LOG_DEBUG("Key: " + NumberFormatter::format(k->getValue()) + ", value: " + v);
			}
			
			values.push_back(es);
		} 
		else if (value.isBoolean()) {
			NYMPH_LOG_DEBUG("Converting boolean value...");
			bool v;
			try { it->second.convert<bool>(v); }
			catch (...) {
				result = "Exception converting to boolean.";
				NYMPH_LOG_ERROR("Exception converting to boolean.");
				return false;
			}
			
			NymphBoolean* eb = new NymphBoolean(v);
			NYMPH_LOG_DEBUG("Key: " + NumberFormatter::format(k->getValue()) + ", value: " + NumberFormatter::format(v));
			values.push_back(eb);
		} 
		else if (value.isInteger()) {			
			NYMPH_LOG_DEBUG("Converting integer value...");
			int32_t v;
			try { it->second.convert<int32_t>(v); }
			catch (...) {
				result = "Exception converting to integer.";
				NYMPH_LOG_ERROR("Exception converting to integer.");
				return false;
			}
			
			NymphInt* el = new NymphInt(v);
			NYMPH_LOG_DEBUG("Key: " + NumberFormatter::format(k->getValue()) + ", value: " + NumberFormatter::format(v));
			values.push_back(el);
		}  
		else if (value.isNumeric()) { // floating point value
			NYMPH_LOG_DEBUG("Converting double value...");
			double v;
			try { it->second.convert<double>(v); }
			catch (...) {
				result = "Exception converting to double.";
				NYMPH_LOG_ERROR("Exception converting to double.");
				return false;
			}
			
			NymphDouble* ed = new NymphDouble(v);
			NYMPH_LOG_DEBUG("Key: " + NumberFormatter::format(k->getValue()) + ", value: " + NumberFormatter::format(v));
			values.push_back(ed);
		} 
		else {	// Array or Object
			// This is a bit of an edge-case, specific to the BMW usage:
			// an embedded object as value, which contains a Base64-encoded
			// binary string, with as key 'binary'.
			// Since a hash table is flat (no sub-levels), this means that we
			// can just collapse any objects we find.
			//
			// FIXME: make this generic instead of searching for the 'binary'
			// key.
			
			NYMPH_LOG_DEBUG("Converting object value...");
			
			JSON::Object::Ptr child = value.extract<JSON::Object::Ptr>();
			Dynamic::Var binary = child->get("binary");
			string v;
			if (!binary.isEmpty() && binary.isString()) {
				string str;
				try {
					binary.convert<string>(str);
					istringstream istr(str);
					ostringstream ostr;
					Base64Decoder b64d(istr);
					if (!b64d.good()) {
						result = "Decoding Base64 data failed. Bad stream.";
						NYMPH_LOG_ERROR("Decoding Base64 data failed. Bad stream.");
						return false;
					}
					
					copy(istreambuf_iterator<char>(b64d), istreambuf_iterator<char>(),
							ostreambuf_iterator<char>(ostr));
					v = ostr.str();
					if (v.empty()) {
						result = "Base64 decoding failed.";
						NYMPH_LOG_ERROR("Base64 decoding failed.");
						return false;
					}
				}
				catch (Poco::DataFormatException& e) {
					result = "Base64 decoding failed.";
					NYMPH_LOG_ERROR("Base64 decoding error: " + e.message());
					return false;
				}
				
				NymphBytes* eb = new NymphBytes(v);
				NYMPH_LOG_DEBUG("Key: " + NumberFormatter::format(k->getValue()) + ", value size: " + NumberFormatter::format(v.length()));
				values.push_back(eb);
			}
		}*/
		/* else {
			// FIXME: handle this case.
			cout << "Unknown JSON type.\n";
		} */
	/*}

	return true;
}
 */
