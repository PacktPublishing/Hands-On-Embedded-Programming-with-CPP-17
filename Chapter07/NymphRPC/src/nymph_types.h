/*
	nymph_types.h	- Defines the NymphRPC data types.
	
	Revision 0
	
	Notes:
			- 
			
	History:
	2017/06/24, Maya Posch : Initial version.
	
	(c) Nyanko.ws
*/


#pragma once
#ifndef NYMPH_TYPES_H
#define NYMPH_TYPES_H

#include <Poco/Poco.h>
#include <Poco/JSON/Object.h>

#include <string>
#include <map>
#include <vector>
#include <cstdint>


enum NymphInternalTypes {
	NYMPH_TYPE_NULL          	= 0x00,
    NYMPH_TYPE_NONE          	= 0x01,
    NYMPH_TYPE_BOOLEAN_FALSE	= 0x02,
    NYMPH_TYPE_BOOLEAN_TRUE  	= 0x03,
    NYMPH_TYPE_UINT8         	= 0x04,
	NYMPH_TYPE_SINT8			= 0x05,
    NYMPH_TYPE_UINT16        	= 0x06,
	NYMPH_TYPE_SINT16			= 0x07,
    NYMPH_TYPE_UINT32       	= 0x08,
	NYMPH_TYPE_SINT32			= 0x09,
    NYMPH_TYPE_UINT64        	= 0x0a,
	NYMPH_TYPE_SINT64			= 0x0b,
    NYMPH_TYPE_FLOAT         	= 0x0c,
    NYMPH_TYPE_DOUBLE        	= 0x0d,
    NYMPH_TYPE_ARRAY         	= 0x0e,
    NYMPH_TYPE_EMPTY_STRING  	= 0x0f,
    NYMPH_TYPE_STRING        	= 0x10,
    NYMPH_TYPE_STRUCT        	= 0x11,
    NYMPH_TYPE_VOID           	= 0x12
};


// External types.
// I.e. those seen by an application using the library.
enum NymphTypes {
	NYMPH_NULL	= 0,
	NYMPH_ARRAY,
	NYMPH_BOOL,
	NYMPH_UINT8,
	NYMPH_SINT8,
	NYMPH_UINT16,
	NYMPH_SINT16,
	NYMPH_UINT32,
	NYMPH_SINT32,
	NYMPH_UINT64,
	NYMPH_SINT64,
	NYMPH_FLOAT,
	NYMPH_DOUBLE,
	NYMPH_INT,
	NYMPH_LONG,
	NYMPH_SHORT,
	NYMPH_STRING,
	NYMPH_STRUCT,
	NYMPH_ANY
};


// >>> UTILITY METHODS <<<
//
inline uint8_t getUInt8(std::string* binary, int &index) {
	return (uint8_t) (*binary)[index++];
}


inline uint16_t getUInt16(std::string* binary, int &index) {
	uint16_t val = *((uint16_t*) &((*binary)[index]));
	index += 2;
	return val;
}


inline uint32_t getUInt32(std::string* binary, int &index) {
	uint32_t val = *((uint32_t*) &((*binary)[index]));
	index += 4;
	return val;
}


inline uint64_t getUInt64(std::string* binary, int &index) {
	uint64_t val = *((uint64_t*) &((*binary)[index]));
	index += 8;
	return val;
}


inline int8_t getSInt8(std::string* binary, int &index) {
	return (int8_t) (*binary)[index++];
}


inline int16_t getSInt16(std::string* binary, int &index) {
	int16_t val = *((int16_t*) &((*binary)[index]));
	index += 2;
	return val;
}


inline int32_t getSInt32(std::string* binary, int &index) {
	int32_t val = *((int32_t*) &((*binary)[index]));
	index += 4;
	return val;
}


inline int64_t getSInt64(std::string* binary, int &index) {
	int64_t val = *((int64_t*) &((*binary)[index]));
	index += 8;
	return val;
}


class NymphType {
	//
	
public:
	virtual ~NymphType() {}
	virtual NymphTypes type() = 0;
	virtual std::string toString(bool quotes = false) = 0;
	virtual std::string serialize() = 0;
	virtual bool deserialize(std::string* binary, int &index) = 0;
	virtual bool empty() = 0;
	virtual uint32_t binarySize() = 0;
};


class NymphString;


struct NymphPair {
	NymphType* key;
	NymphType* value;
};


class NymphNull : public NymphType {
public:
	NymphTypes type() { return NYMPH_NULL; }
	std::string toString(bool quotes = false) { return std::string(); }
	std::string serialize() { return std::string(); }
	bool deserialize(std::string* binary, int &index) { return true; }
	bool empty() { return true; }
	uint32_t binarySize() { return 0; }
};


class NymphArray : public NymphType {
	std::vector<NymphType*> values;
	bool isEmpty;
	uint32_t binSize; // For pre-allocating.
	
public:
	NymphArray() { isEmpty = true; binSize = 0; }
	~NymphArray();
	NymphTypes type() { return NYMPH_ARRAY; }
	std::string toString(bool quotes = false);
	std::vector<NymphType*> getValues() { return values; }
	void addValue(NymphType* value);
	std::string serialize();
	bool deserialize(std::string* binary, int &index);
	bool empty() { return isEmpty; }
	uint32_t binarySize() { return (10 + binSize); }
};


class NymphBoolean : public NymphType {
	bool value;
	bool isEmpty;
	
public:
	NymphBoolean(bool value) { this->value = value; }
	NymphBoolean(std::string* value, int &index) { deserialize(value, index); }
	NymphBoolean(std::string value);
	NymphTypes type() { return NYMPH_BOOL; }
	std::string toString(bool quotes = false);
	void setValue(bool value);
	bool getValue();
	std::string serialize();
	bool deserialize(std::string* binary, int &index);
	bool empty() { return isEmpty; }
	uint32_t binarySize() { return 1; }
};


class NymphUint8 : public NymphType {
	uint8_t value;
	bool isEmpty;
	
public:
	NymphUint8(uint8_t value) { this->value = value; }
	NymphUint8(std::string* value, int &index) { deserialize(value, index); }
	NymphTypes type() { return NYMPH_UINT8; }
	std::string toString(bool quotes = false);
	void setValue(uint8_t value) { this->value = value; isEmpty = false; }
	uint8_t getValue() { return value; }
	std::string serialize();
	bool deserialize(std::string* binary, int &index);
	bool empty() { return isEmpty; }
	uint32_t binarySize() { return 2; }
};


class NymphSint8 : public NymphType {
	int8_t value;
	bool isEmpty;
	
public:
	NymphSint8(int8_t value) { this->value = value; }
	NymphSint8(std::string* value, int &index) { deserialize(value, index); }
	NymphTypes type() { return NYMPH_SINT8; }
	std::string toString(bool quotes = false);
	void setValue(int8_t value);
	int8_t getValue() { return value; }
	std::string serialize();
	bool deserialize(std::string* binary, int &index);
	bool empty() { return isEmpty; }
	uint32_t binarySize() { return 2; }
};


class NymphUint16 : public NymphType {
	uint16_t value;
	bool isEmpty;
	
public:
	NymphUint16(uint16_t value) { this->value = value; }
	NymphUint16(std::string* value, int &index) { deserialize(value, index); }
	NymphTypes type() { return NYMPH_UINT16; }
	std::string toString(bool quotes = false);
	void setValue(uint16_t value) { this->value = value; isEmpty = false; }
	uint16_t getValue() { return value; }
	std::string serialize();
	bool deserialize(std::string* binary, int &index);
	bool empty() { return isEmpty; }
	uint32_t binarySize() { return 3; }
};


class NymphSint16 : public NymphType {
	int16_t value;
	bool isEmpty;
	
public:
	NymphSint16(int16_t value) { this->value = value; }
	NymphSint16(std::string* value, int &index) { deserialize(value, index); }
	NymphTypes type() { return NYMPH_SINT16; }
	std::string toString(bool quotes = false);
	void setValue(int16_t value) { this->value = value; isEmpty = false; }
	int16_t getValue() { return value; }
	std::string serialize();
	bool deserialize(std::string* binary, int &index);
	bool empty() { return isEmpty; }
	uint32_t binarySize() { return 3; }
};


class NymphUint32 : public NymphType {
	uint32_t value;
	bool isEmpty;
	
public:
	NymphUint32(uint32_t value) { this->value = value; }
	NymphUint32(std::string* value, int &index) { deserialize(value, index); }
	NymphTypes type() { return NYMPH_UINT32; }
	std::string toString(bool quotes = false);
	void setValue(uint32_t value) { this->value = value; isEmpty = false; }
	uint32_t getValue() { return value; }
	std::string serialize();
	bool deserialize(std::string* binary, int &index);
	bool empty() { return isEmpty; }
	uint32_t binarySize() { return 5; }
};


class NymphSint32 : public NymphType {
	int32_t value;
	bool isEmpty;
	
public:
	NymphSint32(int32_t value) { this->value = value; }
	NymphSint32(std::string* value, int &index) { deserialize(value, index); }
	NymphTypes type() { return NYMPH_SINT32; }
	std::string toString(bool quotes = false);
	void setValue(int32_t value) { this->value = value; isEmpty = false; }
	int32_t getValue() { return value; }
	std::string serialize();
	bool deserialize(std::string* binary, int &index);
	bool empty() { return isEmpty; }
	uint32_t binarySize() { return 5; }
};


class NymphUint64 : public NymphType {
	uint64_t value;
	bool isEmpty;
	
public:
	NymphUint64(uint64_t value) { this->value = value; }
	NymphUint64(std::string* value, int &index) { deserialize(value, index); }
	NymphTypes type() { return NYMPH_UINT64; }
	std::string toString(bool quotes = false);
	void setValue(int64_t value) { this->value = value; isEmpty = false; }
	uint64_t getValue() { return value; }
	std::string serialize();
	bool deserialize(std::string* binary, int &index);
	bool empty() { return isEmpty; }
	uint32_t binarySize() { return 9; }
};


class NymphSint64 : public NymphType {
	int64_t value;
	bool isEmpty;
	
public:
	NymphSint64(int64_t value) { this->value = value; }
	NymphSint64(std::string* value, int &index) { deserialize(value, index); }
	NymphTypes type() { return NYMPH_SINT64; }
	std::string toString(bool quotes = false);
	void setValue(int64_t value) { this->value = value; isEmpty = false; }
	int64_t getValue() { return value; }
	std::string serialize();
	bool deserialize(std::string* binary, int &index);
	bool empty() { return isEmpty; }
	uint32_t binarySize() { return 9; }
};


class NymphDouble : public NymphType {
	double value;
	bool isEmpty;
	
public:
	NymphDouble(double value) { this->value = value; }
	NymphDouble(std::string* value, int &index) { deserialize(value, index); }
	NymphTypes type() { return NYMPH_DOUBLE; }
	std::string toString(bool quotes = false);
	std::string serialize();
	bool deserialize(std::string* binary, int &index);
	bool empty() { return isEmpty; }
	void setValue(double value) { this->value = value; }
	double getValue() { return value; }
	uint32_t binarySize() { return 9; }
};


class NymphFloat : public NymphType {
	float value;
	bool isEmpty;
	
public:
	NymphFloat(float value) { this->value = value; }
	NymphFloat(std::string* value, int &index) { deserialize(value, index); }
	NymphTypes type() { return NYMPH_FLOAT; }
	std::string toString(bool quotes = false);
	std::string serialize();
	bool deserialize(std::string* binary, int &index);
	bool empty() { return isEmpty; }
	void setValue(float value) { this->value = value; }
	float getValue() { return value; }
	uint32_t binarySize() { return 5; }
};



class NymphString : public NymphType {
	std::string value;
	bool emptyString;
	bool isEmpty;
	uint32_t binSize;
	
public:
	NymphString() { isEmpty = true; emptyString = true; binSize = 0; }
	NymphString(std::string value);
	NymphString(std::string* value, int &index) { deserialize(value, index); }
	NymphTypes type() { return NYMPH_STRING; }
	std::string toString(bool quotes = false);
	std::string serialize();
	bool deserialize(std::string* binary, int &index);
	bool empty() { return isEmpty; }
	void setValue(std::string value);
	std::string getValue() { return value; }
	void setEmptyString(bool val = true) { isEmpty = false; emptyString = val; }
	uint32_t binarySize() { return binSize; }
};


class NymphStruct : public NymphType {
	std::vector<NymphPair> pairs;
	bool isEmpty;
	uint32_t binSize; // For pre-allocating.
	
public:
	NymphStruct() { isEmpty = true; binSize = 0; }
	NymphTypes type() { return NYMPH_STRUCT; }
	std::string toString(bool quotes = false);
	std::string serialize();
	bool deserialize(std::string* binary, int &index);
	bool empty() { return isEmpty; }
	uint32_t binarySize() { return binSize; }
};


/* class NymphTable : public NymphType {
	vector<NymphType*> values;
	bool isEmpty;
	
public:
	NymphTable() { isEmpty = true; }
	~NymphTable();
	NymphTypes type() { return NYMPH_TABLE; }
	std::string toString(bool quotes = false);
	std::string serialize();
	bool deserialize(std::string binary);
	bool empty() { return isEmpty; }
	void addValue(NymphType* value, int &index); { values.push_back(value); isEmpty = false; }
	vector<NymphType*> getValues() { return values; }
	void setValues(vector<NymphType*> values) { this->values = values; isEmpty = false; }
	bool getJson(std::string &json, std::string &result);
	std::string getJson();
	bool setJson(std::string &json, std::string &result);
	bool setJsonObject(JSON::Object::Ptr &json, std::string &result);
	JSON::Object::Ptr getJsonObject(std::string &result);
}; */

#endif
