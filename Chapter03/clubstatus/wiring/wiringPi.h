/*
	wiringPi.h - Mock implementation of the wiringPi header for integration testing.
	
	Revision 0
	
	Features:
			- 
			
	Notes:
			-
			
	2018/06/04, Maya Posch	
*/


#pragma once
#ifndef WIRINGPI_H
#define WIRINGPI_H

#include <Poco/Timer.h>


// Pin modes
#define	INPUT			 0
#define	OUTPUT			 1
#define	PWM_OUTPUT		 2
#define	GPIO_CLOCK		 3
#define	SOFT_PWM_OUTPUT		 4
#define	SOFT_TONE_OUTPUT	 5
#define	PWM_TONE_OUTPUT		 6

#define	LOW			 0
#define	HIGH			 1

// Pull up/down/none
#define	PUD_OFF			 0
#define	PUD_DOWN		 1
#define	PUD_UP			 2

// Interrupt levels
#define	INT_EDGE_SETUP		0
#define	INT_EDGE_FALLING	1
#define	INT_EDGE_RISING		2
#define	INT_EDGE_BOTH		3


// Types
typedef void (*ISRCB)(void);
		

class WiringTimer {
	Poco::Timer* wiringTimer;
	Poco::TimerCallback<WiringTimer>* cb;
	uint8_t triggerCnt;
	
public:
	ISRCB isrcb_0;
	ISRCB isrcb_7;
	bool isr_0_set;
	bool isr_7_set;
	
	WiringTimer();
	~WiringTimer();
	void start();
	void trigger(Poco::Timer &t);
};




int wiringPiSetup(); 
void pinMode(int pin, int mode); 
void pullUpDnControl(int pin, int pud); 
int digitalRead(int pin);
int wiringPiISR(int pin, int mode, void (*function)(void));

#endif
