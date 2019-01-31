/*
	wiringPiI2C.h - Header for the mock wiringPi I2C implementation.
	
	Revision 0
	
	Features:
			- 
			
	Notes:
			-
			
	2018/06/04, Maya Posch
*/


#ifndef WIRINGPII2C_H
#define WIRINGPII2C_H


int wiringPiI2CSetup(const int devId);
int wiringPiI2CWriteReg8(int fd, int reg, int data);

#endif
