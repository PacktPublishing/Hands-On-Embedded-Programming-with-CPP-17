/*
	club.h - Header file for the Club class.
	
	Revision 0
	
	Notes:
			- 
			
	2018/02/28, Maya Posch
*/

#ifndef CLUB_H
#define CLUB_H


#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Timestamp.h>
#include <Poco/Timer.h>
#include <Poco/Mutex.h>
#include <Poco/Condition.h>
#include <Poco/Thread.h>

using namespace Poco;
using namespace Poco::Net;

#include <string>
#include <vector>
#include <map>
#include <queue>

using namespace std;

// Raspberry Pi GPIO, i2c, etc. functionality.
#include <wiringPi.h>
#include <wiringPiI2C.h>


enum Log_level {
	LOG_FATAL   = 1,
	LOG_ERROR   = 2,
	LOG_WARNING = 3,
	LOG_INFO    = 4,
	LOG_DEBUG   = 5
};


class Listener;


class ClubUpdater : public Runnable {
	TimerCallback<ClubUpdater>* cb;
	uint8_t regDir0;
	uint8_t regOut0;
	int i2cRelayHandle;
	Timer* timer;
	Mutex mutex;
	Mutex timerMutex;
	bool powerTimerActive;
	
public:
	void run();
	void updateStatus();
	void writeRelayOutputs();
	void setPowerState(Timer &t);
};


class Club {
	static Thread updateThread;
	static ClubUpdater updater;
	
	static void lockISRCallback();
	static void statusISRCallback();
	
public:
	static bool currentStatusSwitchValue;
	static bool currentLockSwitchValue;
	static bool powerOn;
	static Listener* mqtt;
	static bool relayPresent;
	static uint8_t relayAddress;
	static string mqttStatusTopic;	// Topic we publish status updates on.
	
	static Condition clubCond;
	static Mutex clubCondMutex;
	static Mutex logMutex;
	static bool clubChanged;
	static bool running;
	static bool clubIsClosed;
	static bool firstRun;
	static bool lockChanged;
	static bool statusChanged;
	static bool previousLockSwitchValue;
	static bool previousStatusSwitchValue;
	
	static bool start(bool relaypresent, uint8_t relayaddress, string topic);
	static void stop();
	static void setRelay();
	static void log(Log_level level, string msg);
};

#endif
