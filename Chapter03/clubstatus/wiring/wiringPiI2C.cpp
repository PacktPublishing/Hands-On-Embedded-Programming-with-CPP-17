/*
	wiringPiI2C.h - Implementation for the mock wiringPi I2C implementation.
	
	Revision 0
	
	Features:
			- 
			
	Notes:
			-
			
	2018/06/04, Maya Posch
*/


#include "wiringPiI2C.h"

#include "../club.h"

#include <Poco/NumberFormatter.h>

using namespace Poco;


int wiringPiI2CSetup(const int devId) {
	// Device ID is ignored as the test hard-codes the device.
	Club::log(LOG_INFO, "wiringPiI2CSetup: setting up device ID: 0x" 
							+ NumberFormatter::formatHex(devId, 2));
	
	// Return the device handle.
	return 0;
}


int wiringPiI2CWriteReg8(int fd, int reg, int data) {
	// Write the provided data to the specified register of the simulated device.
	
	// Here we could simulate one or more devices, or just output what we received here.
	Club::log(LOG_INFO, "wiringPiI2CWriteReg8: Device handle 0x0" + NumberFormatter::formatHex(fd) 
							+ ", Register 0x" + NumberFormatter::formatHex(reg, 2)
							+ " set to: 0x" + NumberFormatter::formatHex(data, 2));
	
	return 0;
}
