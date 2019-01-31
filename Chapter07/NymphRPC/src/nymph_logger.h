/*
	nymph_logger.h	- header file for the NymphRPC Logger class.
	
	Revision 0
	
	Notes:
			- The central logging client for the Nymph library.
			
	2017/06/24, Maya Posch	: Initial version.
	(c) Nyanko.ws
*/

#pragma once
#ifndef NYMPH_LOGGER_H
#define NYMPH_LOGGER_H

#include <Poco/Logger.h>
#include <Poco/LogStream.h>
#include <Poco/Channel.h>

#include <string>

using namespace std;


enum NymphLogLevels {
	NYMPH_LOG_LEVEL_FATAL = 1,
	NYMPH_LOG_LEVEL_CRITICAL,
	NYMPH_LOG_LEVEL_ERROR,
	NYMPH_LOG_LEVEL_WARNING,
	NYMPH_LOG_LEVEL_NOTICE,
	NYMPH_LOG_LEVEL_INFO,
	NYMPH_LOG_LEVEL_DEBUG,
	NYMPH_LOG_LEVEL_TRACE
};


#define NYMPH_LOG_FATAL(msg) \
	if (NymphLogger::priority >= Poco::Message::PRIO_FATAL) { \
		NymphLogger::logger(loggerName).fatal(msg, __FILE__, __LINE__);\
	} 
#define NYMPH_LOG_CRITICAL(msg) \
	if (NymphLogger::priority >= Poco::Message::PRIO_CRITICAL) { \
		NymphLogger::logger(loggerName).critical(msg, __FILE__, __LINE__);\
	}
#define NYMPH_LOG_ERROR(msg) \
	if (NymphLogger::priority >= Poco::Message::PRIO_ERROR) { \
		NymphLogger::logger(loggerName).error(msg, __FILE__, __LINE__);\
	}
#define NYMPH_LOG_WARNING(msg) \
	if (NymphLogger::priority >= Poco::Message::PRIO_WARNING) { \
		NymphLogger::logger(loggerName).warning(msg, __FILE__, __LINE__);\
	}
#define NYMPH_LOG_NOTICE(msg) \
	if (NymphLogger::priority >= Poco::Message::PRIO_NOTICE) { \
		NymphLogger::logger(loggerName).notice(msg, __FILE__, __LINE__);\
	}
#define NYMPH_LOG_INFORMATION(msg) \
	if (NymphLogger::priority >= Poco::Message::PRIO_INFORMATION) { \
		NymphLogger::logger(loggerName).information(msg, __FILE__, __LINE__);\
	}
#define NYMPH_LOG_DEBUG(msg) \
	if (NymphLogger::priority >= Poco::Message::PRIO_DEBUG) { \
		NymphLogger::logger(loggerName).debug(msg, __FILE__, __LINE__);\
	}
#define NYMPH_LOG_TRACE(msg) \
	if (NymphLogger::priority >= Poco::Message::PRIO_TRACE) { \
		NymphLogger::logger(loggerName).trace(msg, __FILE__, __LINE__);\
	}


// Function pointer typedef for the function-based logger.
typedef void (*logFnc)(int, string);


class NymphLoggerChannel : public Poco::Channel {
	logFnc loggerFunction;
	
public:
	NymphLoggerChannel(logFnc function);
	~NymphLoggerChannel();
	
	void close();
	//string getProperty(const string &name) { return string(); }
	void log(const Poco::Message &msg);
	void open();
	//void setProperty(const string &name, const string &value) { }
};


class NymphLogger {
	//static Poco::Logger* loggerRef;
public:
	static Poco::Message::Priority priority;
	
	static void setLoggerFunction(logFnc function);
	static void setLogLevel(Poco::Message::Priority priority);
	static Poco::Logger& logger();
	static Poco::Logger& logger(string &name);
	//static void log(string message);
};

#endif
