/*
	listener.cpp - Handle MQTT events.
	
	Revision 0
	
	Features:
				- 
				
	Notes:
				-
				
	2018/02/28, Maya Posch
*/

#include "listener.h"

#include <iostream>

using namespace std;


// --- CONSTRUCTOR ---
Listener::Listener(string clientId, string host, int port, string user, string pass) : mosquittopp(clientId.c_str()) {
	int keepalive = 60;
	username_pw_set(user.c_str(), pass.c_str());
	connect(host.c_str(), port, keepalive);
}


// --- DECONSTRUCTOR ---
Listener::~Listener() {
	//
}


// --- ON CONNECT ---
void Listener::on_connect(int rc) {
	cout << "Connected. Subscribing to topics..." << endl;
	
	// Check code.
	if (rc == 0) {
		// Subscribe to desired topics.
		string topic = "/club/status";
		subscribe(0, topic.c_str(), 1);
	}
	else {
		// handle.
		cerr << "Connection failed. Aborting subscribing." << endl;
	}
}


// --- ON MESSAGE ---
void Listener::on_message(const struct mosquitto_message* message) {
	string topic = message->topic;
	string payload = string((const char*) message->payload, message->payloadlen);
	
	if (topic == "/club/status") {
		// No payload. Just publish the current club status.
		string topic = "/club/status/response";
		char payload[] = { 0x10 }; // TODO
		publish(0, topic.c_str(), 1, payload, 1); // QoS 1.	
	}
}


// --- ON SUBSCRIBE ---
void Listener::on_subscribe(int mid, int qos_count, const int* granted_qos) {
	// Report success, with details.
}


// --- SEND MESSAGE ---
void Listener::sendMessage(string topic, string &message) {
	publish(0, topic.c_str(), message.length(), message.c_str(), true);
}


// --- SEND MESSAGE ---
void Listener::sendMessage(string &topic, char* message, int msgLength) {
	publish(0, topic.c_str(), msgLength, message, true);
}
