/*
	nymph_logger.cpp	- implementation for the NymphRPC Logger class.
	
	Revision 0
	
	Features:
			- 
	
	Notes:
			- This class declares the main class to be used for logging.
			
	2017/06/24, Maya Posch : Initial version.	
	(c) Nyanko.ws
*/

#include "nymph_logger.h"

#include <Poco/AutoPtr.h>
#include <Poco/NumberFormatter.h>
//#include <Poco/DateTimeFormatter.h>

using namespace Poco;


// >>> NYMPH LOGGER CHANNEL <<<
// --- CONSTRUCTOR ---
NymphLoggerChannel::NymphLoggerChannel(logFnc function) {
	loggerFunction = function;
}


// --- DECONSTRUCTOR ---
NymphLoggerChannel::~NymphLoggerChannel() {
	//
}


// --- CLOSE ---
// Close the channel before discarding it.
void NymphLoggerChannel::close() {
	// Nothing to do.
}


// --- LOG ---
void NymphLoggerChannel::log(const Message &msg) {
	// Convert message to log string.
	int level;
	string logLevel;
	switch (msg.getPriority()) {
		case Message::PRIO_FATAL:
			level = 0;
			logLevel = "FATAL";
			break;
		case Message::PRIO_CRITICAL:
			level = 1;
			logLevel = "CRITICAL";
			break;
		case Message::PRIO_ERROR:
			level = 2;
			logLevel = "ERROR";
			break;
		case Message::PRIO_WARNING:
			level = 3;
			logLevel = "WARNING";
			break;
		case Message::PRIO_NOTICE:
			level = 4;
			logLevel = "NOTICE";
			break;
		case Message::PRIO_INFORMATION:
			level = 5;
			logLevel = "INFO";
			break;
		case Message::PRIO_DEBUG:
			level = 6;
			logLevel = "DEBUG";
			break;
		case Message::PRIO_TRACE:
			level = 7;
			logLevel = "TRACE";
			break;
		default:
			level = 7;
			logLevel = "UNKNOWN";
			break;
	}
	
	string msgStr;
	//const string timeFormat = "%H:%M:%s"; // 24-hour, minutes, seconds & usecs.
	//msgStr = DateTimeFormatter::format(msg.getTimeStamp(), timeFormat);
	//msgStr = logLevel + "\t";
	msgStr = NumberFormatter::format(msg.getPid());
	msgStr += "." + NumberFormatter::format(msg.getTid());
	msgStr += "\t" + msg.getSource() + "\t";
	msgStr += NumberFormatter::format(msg.getSourceLine()) + "\t";
	msgStr += msg.getText() + "\t\t- ";
	msgStr += msg.getSourceFile();
	
	(*loggerFunction)(level, msgStr);
}


// --- OPEN ---
void NymphLoggerChannel::open() {
	// Nothing to do.
}


// >>> NYMPH LOGGER <<<
// Static initialisations
Message::Priority NymphLogger::priority;
//Poco::Logger* NymphLogger::loggerRef;


// --- SET LOGGER FUNCTION ---
// Initialises the logger and associated logging channel.
void NymphLogger::setLoggerFunction(logFnc function) {
	AutoPtr<NymphLoggerChannel> nymphChannel(new NymphLoggerChannel(function));
	Logger::root().setChannel(nymphChannel);
	//loggerRef = &Logger::get("NymphLogger");
}


// --- SET LOG LEVEL ---
void NymphLogger::setLogLevel(Poco::Message::Priority priority) {
	NymphLogger::priority = priority;
	Logger::root().setLevel(priority);
	
}


// --- LOGGER ---
// Returns a reference to the logger instance.
Logger& NymphLogger::logger() {
	//return *loggerRef;
	return Logger::get("NymphLogger");
}


// Returns a reference to the logger instance using the provided name.
Logger& NymphLogger::logger(string &name) {
	// TODO: cache the returned logger for future calls.
	return Logger::get(name);
}
