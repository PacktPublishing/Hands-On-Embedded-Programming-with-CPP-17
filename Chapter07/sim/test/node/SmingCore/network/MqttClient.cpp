/****
 * Sming Framework Project - Open Source framework for high efficiency native ESP8266 development.
 * Created 2015 by Skurydin Alexey
 * http://github.com/anakod/Sming
 * All files of the Sming Core are provided under the LGPL v3 license.
 ****/

#include "MqttClient.h"
#include "../Clock.h"
#include <algorithm>
#include <cstring>

MqttClient::MqttClient(bool autoDestruct /* = false*/)
{
	memset(buffer, 0, MQTT_MAX_BUFFER_SIZE + 1);
	waitingSize = 0;
	posHeader = 0;
	current = NULL;
	
	mosqpp::lib_init();
}

// Deprecated . . .
MqttClient::MqttClient(String serverHost, int serverPort, MqttStringSubscriptionCallback callback /* = NULL*/)
	{
	url.Host = serverHost;
	url.Port = serverPort;
	this->callback = callback;
	waitingSize = 0;
	posHeader = 0;
	current = NULL;
	
	mosqpp::lib_init();
}

MqttClient::~MqttClient() {
	mqtt.loop_stop();
	mosqpp::lib_cleanup();
}

void MqttClient::setCallback(MqttStringSubscriptionCallback callback) {
	this->callback = callback;
}

void MqttClient::setCompleteDelegate(TcpClientCompleteDelegate completeCb) {
	completed = completeCb;
}

void MqttClient::setKeepAlive(int seconds) {
	keepAlive = seconds;
}

void MqttClient::setPingRepeatTime(int seconds) {
	if(pingRepeatTime > keepAlive) {
		pingRepeatTime = keepAlive;
	} else {
		pingRepeatTime = seconds;
	}
}

bool MqttClient::setWill(const String& topic, const String& message, int QoS, bool retained /* = false*/)
{
	return mqtt.will_set(topic.c_str(), message.length(), message.c_str(), QoS, retained);
}

bool MqttClient::connect(const URL& url, const String& clientName, uint32_t sslOptions) {
	this->url = url;
	if(!(url.Protocol == "mqtt" || url.Protocol == "mqtts")) {
		return false;
	}
	
	waitingSize = 0;
	posHeader = 0;
	current = NULL;

	bool useSsl = (url.Protocol == "mqtts");
	return privateConnect(clientName, url.User, url.Password, useSsl, sslOptions);
}

// Deprecated . . .
bool MqttClient::connect(const String& clientName, bool useSsl /* = false */, uint32_t sslOptions /* = 0 */)
{
	return MqttClient::connect(clientName, "", "", useSsl, sslOptions);
}

// Deprecated . . .
bool MqttClient::connect(const String& clientName, const String& username, const String& password,
						 bool useSsl /* = false */, uint32_t sslOptions /* = 0 */)
{
	return privateConnect(clientName, username, password, useSsl, sslOptions);
}

bool MqttClient::privateConnect(const String& clientName, const String& username, const String& password,
								bool useSsl /* = false */, uint32_t sslOptions /* = 0 */) {
	if (clientName.length() > 0) {
		mqtt.reinitialise(clientName.c_str(), false);
	}

	if (username.length() > 0) {
		mqtt.username_pw_set(username.c_str(), password.c_str());
	}
	
	// TODO: configure TLS settings.
	if (useSsl) {
		//
	}

	mqtt.connect(url.Host.c_str(), url.Port, keepAlive);
	mqtt.loop_start();
	return true;
}

bool MqttClient::publish(String topic, String message, bool retained /* = false*/) {
	int res = mqtt.publish(0, topic.c_str(), message.length(), message.c_str(), 0, retained);
	return res > 0;
}

bool MqttClient::publishWithQoS(String topic, String message, int QoS, bool retained /* = false*/,
								MqttMessageDeliveredCallback onDelivery /* = NULL */)
{
	int res = mqtt.publish(0, topic.c_str(), message.length(), message.c_str(), QoS, retained);
	
	return res > 0;
}

bool MqttClient::subscribe(const String& topic) {
	int res = mqtt.subscribe(0, topic.c_str());
	return res > 0;
}

bool MqttClient::unsubscribe(const String& topic) {
	int res = mqtt.unsubscribe(0, topic.c_str());
	return res > 0;
}


void MqttClient::on_message(const struct mosquitto_message* message) {
	if (callback) {
		callback(String(message->topic), String((char*) message->payload, message->payloadlen));
	}
}
