/*
	club.cpp - Implementation of the Club class.
	
	Revision 0
	
	Notes:
			- 
			
	2018/02/28, Maya Posch
*/


#include "club.h"

#include <iostream>

using namespace std;


#include <Poco/NumberFormatter.h>

using namespace Poco;


#include "listener.h"

// PCA95x5 register map (PCA9555, PCA9535, PCAL9535A,...)
#define REG_INPUT_PORT0              0x00
#define REG_INPUT_PORT1              0x01
#define REG_OUTPUT_PORT0             0x02
#define REG_OUTPUT_PORT1             0x03
#define REG_POL_INV_PORT0            0x04
#define REG_POL_INV_PORT1            0x05
#define REG_CONF_PORT0               0x06
#define REG_CONG_PORT1               0x07
#define REG_OUT_DRV_STRENGTH_PORT0_L 0x40
#define REG_OUT_DRV_STRENGTH_PORT0_H 0x41
#define REG_OUT_DRV_STRENGTH_PORT1_L 0x42
#define REG_OUT_DRV_STRENGTH_PORT1_H 0x43
#define REG_INPUT_LATCH_PORT0        0x44
#define REG_INPUT_LATCH_PORT1        0x45
#define REG_PUD_EN_PORT0             0x46
#define REG_PUD_EN_PORT1             0x47
#define REG_PUD_SEL_PORT0            0x48
#define REG_PUD_SEL_PORT1            0x49
#define REG_INT_MASK_PORT0           0x4A
#define REG_INT_MASK_PORT1           0x4B
#define REG_INT_STATUS_PORT0         0x4C
#define REG_INT_STATUS_PORT1         0x4D
#define REG_OUTPUT_PORT_CONF         0x4F

// I2C IO expander bit / pin mapping
#define RELAY_POWER  0
#define RELAY_GREEN  1
#define RELAY_YELLOW 2
#define RELAY_RED    3

// Lock: 	header pin 11 - Pi Rev. 3 B / WiringPi: GPIO 0 - Pi Rev. 2: GPIO 17
// Status: 	header pin 07 - Pi Rev. 3 B / WiringPi: GPIO 7 - Pi Rev. 2: GPIO 4
#define GPIO_LOCK_FEEDBACK 0
#define GPIO_STATUS_SWITCH 7

// Static initialisations.
bool Club::currentStatusSwitchValue;
bool Club::currentLockSwitchValue;
bool Club::powerOn;
Thread Club::updateThread;
ClubUpdater Club::updater;
bool Club::relayPresent;
uint8_t Club::relayAddress;
string Club::mqttStatusTopic;
Listener* Club::mqtt = 0;

Condition Club::clubCond;
Mutex Club::clubCondMutex;
Mutex Club::logMutex;
bool Club::clubChanged = false;
bool Club::running = false;
bool Club::clubIsClosed = true;
bool Club::firstRun = true;
bool Club::lockChanged = false;
bool Club::statusChanged = false;
bool Club::previousLockSwitchValue = false;
bool Club::previousStatusSwitchValue = false;


// === CLUB UPDATER ===
// --- RUN ---
void ClubUpdater::run() {
	// Defaults.
	regDir0 = 0x00; // All pins as output.
	regOut0 = 0x00; // All pins low.
	Club::powerOn = false;
	// create a TimerCallback adapter instance (enables instance method callback)
	cb = new TimerCallback<ClubUpdater>(*this, &ClubUpdater::setPowerState);
	timer = new Timer(10 * 1000, 0); // 10 sec interval
	powerTimerActive = false;
	
	if (Club::relayPresent) {
		// TODO: Preemptively try to resolve bus contention by pulsing SCL a few times 
		// or sending 0xFF (release SDA). This can drive slave state machines to release SDA.
	
		// Start the i2c bus and check for presence of the relay board.
		Club::log(LOG_INFO, "ClubUpdater: Starting i2c relay device.");
		i2cRelayHandle = wiringPiI2CSetup(Club::relayAddress);
		if (i2cRelayHandle == -1) {
			Club::log(LOG_FATAL, string("ClubUpdater: error starting i2c relay device."));
			return;
		}
	
		// Configure the GPIO expander connected to relays.
		wiringPiI2CWriteReg8(i2cRelayHandle, REG_CONF_PORT0,   regDir0); 
		wiringPiI2CWriteReg8(i2cRelayHandle, REG_OUTPUT_PORT0, regOut0); 
		
		Club::log(LOG_INFO, "ClubUpdater: Finished setting defaults on the i2c relay board.");
	}
	
	// Perform initial update for club status.
	updateStatus();
	
	Club::log(LOG_INFO, "ClubUpdater: Initial status update complete.");
	Club::log(LOG_INFO, "ClubUpdater: Entering waiting condition.");
	
	// Start waiting using the condition variable until signalled by one of the interrupts.	
	while (Club::running) {
		Club::clubCondMutex.lock();
		if (!Club::clubCond.tryWait(Club::clubCondMutex, 60 * 1000)) { // Wait for a minute.
			Club::clubCondMutex.unlock();
			if (!Club::clubChanged) { continue; } // Timed out, still no change. Wait again.
		}
		else {
			Club::clubCondMutex.unlock();
		}
		
		updateStatus();
	}
}


// --- UPDATE STATUS ---
void ClubUpdater::updateStatus() {
	
	Club::clubChanged = false; // changes will be handled below; reset clubChanged flag.
	
	// Adjust the club status using the flags that got updated by the interrupt handler(s).
	// leave updateStatus() if no pending changes are found.
	if (Club::lockChanged) {
		string state = (Club::currentLockSwitchValue) ? "locked" : "unlocked";
		Club::log(LOG_INFO, string("ClubUpdater: lock status changed to ") + state);
		Club::lockChanged = false;
		
		if (Club::currentLockSwitchValue == Club::previousLockSwitchValue) {
			Club::log(LOG_WARNING, string("ClubUpdater: lock interrupt triggered, but value hasn't changed. Aborting."));
			return;
		}
		
		Club::previousLockSwitchValue = Club::currentLockSwitchValue;
	}
	else if (Club::statusChanged) {		
		string state = (Club::currentStatusSwitchValue) ? "off" : "on";
		Club::log(LOG_INFO, string("ClubUpdater: status switch status changed to ") + state);
		Club::statusChanged = false;
		
		if (Club::currentStatusSwitchValue == Club::previousStatusSwitchValue) {
			Club::log(LOG_WARNING, string("ClubUpdater: status interrupt triggered, but value hasn't changed. Aborting."));
			return;
		}
		
		Club::previousStatusSwitchValue = Club::currentStatusSwitchValue;
	}
	else if (Club::firstRun) {
		Club::log(LOG_INFO, string("ClubUpdater: starting initial update run."));
		timer->start(*cb);
		timer->stop();
		Club::firstRun = false;
	}
	else {
		Club::log(LOG_ERROR, string("ClubUpdater: update triggered, but no change detected. Aborting."));
		return;
	}
	
	// Changes were found above - continue handling their implications.
	
	// Check whether we are opening the club.
	if (Club::clubIsClosed && !Club::currentStatusSwitchValue) {
		Club::clubIsClosed = false;
		
		Club::log(LOG_INFO, string("ClubUpdater: Opening club."));
		
		// Power has to be on. Write to relay with a 10 second delay.
		Club::powerOn = true;
		
		try {
			timer->stop();
			timer->start(*cb);
			powerTimerActive = true;
			Club::log(LOG_INFO, "ClubUpdater: Started power timer...");
		}
		catch (Poco::IllegalStateException &e) {
			Club::log(LOG_ERROR, "ClubUpdater: IllegalStateException on timer start: " + e.message());
			return;
		}
		catch (...) {
			Club::log(LOG_ERROR, "ClubUpdater: Unknown exception on timer start.");
			return;
		}
		
		// Send MQTT notification. Payload is '1' (true) as ASCII.
		char msg = { '1' };
		Club::mqtt->sendMessage(Club::mqttStatusTopic, &msg, 1);
		
		Club::log(LOG_DEBUG, "ClubUpdater: Sent MQTT message.");
	}
	else if (!Club::clubIsClosed && Club::currentStatusSwitchValue) { // check if we're closing the club.
		Club::clubIsClosed = true;
		
		Club::log(LOG_INFO, string("ClubUpdater: Closing club."));
		
		// Power has to be off. Write to relay with a 10 second delay.
		Club::powerOn = false;
		
		try {
			timer->stop();
			timer->start(*cb);
			powerTimerActive = true;
			Club::log(LOG_INFO, "ClubUpdater: Started power timer...");
		}
		catch (Poco::IllegalStateException &e) {
			Club::log(LOG_ERROR, "ClubUpdater: IllegalStateException on timer start: " + e.message());
			return;
		}
		catch (...) {
			Club::log(LOG_ERROR, "ClubUpdater: Unknown exception on timer start.");
			return;
		}
		
		// Send MQTT notification. Payload is '0' (false) as ASCII.
		char msg = { '0' };
		Club::mqtt->sendMessage(Club::mqttStatusTopic, &msg, 1);
		
		Club::log(LOG_DEBUG, "ClubUpdater: Sent MQTT message.");
	}
	// else { // no change 
		
	regOut0 = 0; // reset all relay outputs, activate conditionally below.
	
	// Update the traffic light and power relays.
	if (Club::currentStatusSwitchValue) { 
		Club::log(LOG_INFO, string("ClubUpdater: New lights, clubstatus off."));
		mutex.lock();
	
		string state = (Club::powerOn) ? "on" : "off";
		if (powerTimerActive) {
			Club::log(LOG_DEBUG, string("ClubUpdater: Power timer active, inverting power state from: ") + state);
			if (!Club::powerOn) {
				regOut0 |= (1UL << RELAY_POWER);
			}
		}
		else {
			Club::log(LOG_DEBUG, string("ClubUpdater: Power timer not active, using current power state: ") + state);
			if (Club::powerOn) {
				regOut0 |= (1UL << RELAY_POWER); 
			}
		}
		
		if (Club::currentLockSwitchValue) {
			Club::log(LOG_INFO, string("ClubUpdater: Red on."));
			regOut0 |= (1UL << RELAY_RED); 
		} 
		else {
			Club::log(LOG_INFO, string("ClubUpdater: Yellow on."));
			regOut0 |= (1UL << RELAY_YELLOW);
		} 
		
		Club::log(LOG_DEBUG, "ClubUpdater: Changing output register to: 0x" + NumberFormatter::formatHex(regOut0, 2));
		
		writeRelayOutputs();
		mutex.unlock();
	}
	else { // Club on.
		Club::log(LOG_INFO, string("ClubUpdater: New lights, clubstatus on."));
		
		mutex.lock();
		string state = (Club::powerOn) ? "on" : "off";
		if (powerTimerActive) {
			Club::log(LOG_DEBUG, string("ClubUpdater: Power timer active, inverting power state from: ") + state);
			regOut0 = !Club::powerOn; // Take the inverse of what the timer callback will set.
		}
		else {
			Club::log(LOG_DEBUG, string("ClubUpdater: Power timer not active, using current power state: ") + state);
			regOut0 = Club::powerOn; // Use the current power state value.
		}
		
		if (Club::currentLockSwitchValue) {
			Club::log(LOG_INFO, string("ClubUpdater: Yellow & Red on."));
			regOut0 |= (1UL << RELAY_YELLOW) | (1UL << RELAY_RED);
		}
		else {
			Club::log(LOG_INFO, string("ClubUpdater: Green on."));
			regOut0 |= (1UL << RELAY_GREEN);
		}
		
		Club::log(LOG_DEBUG, "ClubUpdater: Changing output register to: 0x" + 	NumberFormatter::formatHex(regOut0, 2));
		
		writeRelayOutputs();
		mutex.unlock();
	}
}


// --- WRITE RELAY OUTPUTS ---
void ClubUpdater::writeRelayOutputs() {
	if(Club::relayPresent) {
		// Write the current state of the locally saved output 0 register contents to the device.
		wiringPiI2CWriteReg8(i2cRelayHandle, REG_OUTPUT_PORT0, regOut0);
		
		Club::log(LOG_DEBUG, "ClubUpdater: Finished writing relay outputs with: 0x" 
				+ NumberFormatter::formatHex(regOut0, 2));
	}
}	


// --- SET POWER STATE ---
void ClubUpdater::setPowerState(Timer &t) {
	Club::log(LOG_INFO, string("ClubUpdater: setPowerState called."));
	
	// Update register with current power state, then update remote device.
	mutex.lock();
	if (Club::powerOn) { 
		regOut0 |= (1UL << RELAY_POWER); 
	} else { 
		regOut0 &= ~(1UL << RELAY_POWER); 
	}
	
	Club::log(LOG_DEBUG, "ClubUpdater: Writing relay with: 0x" + NumberFormatter::formatHex(regOut0, 2));
	
	writeRelayOutputs();
	
	Club::log(LOG_DEBUG, "ClubUpdater: Written relay outputs.");
	
	//delete timer;
	powerTimerActive = false;
	//t.restart(0);
	mutex.unlock();
	
	Club::log(LOG_DEBUG, "ClubUpdater: Finished setPowerState.");
}


// === CLUB ===
// --- START ---
bool Club::start(bool relaypresent, uint8_t relayaddress, string topic) {
	Club::log(LOG_INFO, "Club: starting up...");
	// Defaults.
	relayPresent = relaypresent;
	relayAddress = relayaddress;
	mqttStatusTopic = topic;
	
	// Start the WiringPi framework.
	// We assume that this service is running inside the 'gpio' group.
	// Since we use this setup method we are expected to use WiringPi pin numbers.
	wiringPiSetup();
	
	Club::log(LOG_INFO,  "Club: Finished wiringPi setup.");

	// Configure and read current GPIO inputs.
	pinMode(GPIO_LOCK_FEEDBACK, INPUT);
	pinMode(GPIO_STATUS_SWITCH, INPUT);
	pullUpDnControl(GPIO_LOCK_FEEDBACK, PUD_DOWN);
	pullUpDnControl(GPIO_STATUS_SWITCH, PUD_DOWN);
	
	// The switch inputs are double-buffered to detect changes.
	currentLockSwitchValue   =  digitalRead(GPIO_LOCK_FEEDBACK);
	currentStatusSwitchValue = !digitalRead(GPIO_STATUS_SWITCH);
	previousLockSwitchValue   = currentLockSwitchValue;
	previousStatusSwitchValue = currentStatusSwitchValue;
	
	Club::log(LOG_INFO, "Club: Finished configuring pins.");
	
	// Register GPIO interrupts for the lock and club status switches.
	wiringPiISR(GPIO_LOCK_FEEDBACK, INT_EDGE_BOTH, &lockISRCallback);
	wiringPiISR(GPIO_STATUS_SWITCH, INT_EDGE_BOTH, &statusISRCallback);
	
	Club::log(LOG_INFO, "Club: Configured interrupts.");
	
	// Start update thread.
	running = true;
	updateThread.start(updater);
	
	Club::log(LOG_INFO, "Club: Started update thread.");
	
	return true;
}


// --- STOP ---
void Club::stop() {
	running = false;
	
	// unregister the GPIO interrupts.
	// TODO: research whether clean-up is needed with a restart instead of an application shutdown.
	
	// Close the relay board i2c file descriptor.
	// TODO: Check back if wiringPiI2CClose() implemented in later revision, 
	// see https://github.com/WiringPi/WiringPi-Node/pull/50 for details.
}


// --- LOCK ISR CALLBACK ---
void Club::lockISRCallback() {
	// the door lock opened / closed, triggering an interrupt. 
	// Update the internal state clubLocked to reflect that.
	currentLockSwitchValue = digitalRead(GPIO_LOCK_FEEDBACK);
	lockChanged = true;
	
	// set clubChanged flag
	clubChanged = true;
	clubCond.signal();
}


// --- STATUS ISR CALLBACK ---
void Club::statusISRCallback() {
	// Update the current status GPIO value.
	currentStatusSwitchValue = !digitalRead(GPIO_STATUS_SWITCH);
	statusChanged = true;
	
	// set clubChanged flag
	clubChanged = true;
	clubCond.signal();
}


// --- LOG ---
void Club::log(Log_level level, string msg) {
	string msg_keyword, log_topic;
	
	switch (level) {
		case LOG_FATAL: {
			msg_keyword = "FATAL:";
			log_topic   = "/log/fatal";
			break; }
		case LOG_ERROR: {
			msg_keyword = "ERROR:";
			log_topic   = "/log/error";
			break; }
		case LOG_WARNING: {
			msg_keyword = "WARNING:";
			log_topic   = "/log/warning";
			break; }
		case LOG_INFO: {
			msg_keyword = "INFO:";
			log_topic   = "/log/info";
			break; }
		case LOG_DEBUG: {
			msg_keyword = "DEBUG:";
			log_topic   = "/log/debug";
			break; }
		default:{ // Invalid logging type.
			return; }
	}
	
	logMutex.lock();
	ostream& os = (level == LOG_INFO || level == LOG_DEBUG) ? cout : cerr;
	os << msg_keyword << "\t" << msg << endl;
	if (mqtt) {
		string message = msg_keyword + " " + msg;
		mqtt->sendMessage(log_topic, message);
	}	
	logMutex.unlock();
}
