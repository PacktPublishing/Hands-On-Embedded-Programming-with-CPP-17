/*
	listener.h - Header file for the Listener MQTT class.
	
	Revision 0
	
	Notes:
			- 
			
	2018/02/28, Maya Posch
*/


#pragma once
#ifndef LISTENER_H
#define LISTENER_H

#include <mosquittopp.h>

#include <string>
#include <map>

using namespace std;

#include <Poco/Mutex.h>

using namespace Poco;


class Listener : public mosqpp::mosquittopp {
	//
	
public:
	Listener(string clientId, string host, int port, string user, string pass);
	~Listener();
	
	void on_connect(int rc);
	void on_message(const struct mosquitto_message* message);
	void on_subscribe(int mid, int qos_count, const int* granted_qos);
	
	void sendMessage(string topic, string& message);
	void sendMessage(string& topic, char* message, int msgLength);
};

#endif
