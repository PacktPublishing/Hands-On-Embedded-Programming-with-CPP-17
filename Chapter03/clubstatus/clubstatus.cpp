/*
	clubstatus.cpp - Clubstatus service for monitoring a club room.
	
	Revision 0
	
	Features:
				- Interrupt-based switch monitoring.
				- REST API 
				
	Notes:
				- First argument to main is the location of the configuration
					file in INI format.
				
	2018/02/28, Maya Posch
*/


#include "listener.h"

#include <iostream>
#include <string>

using namespace std;

#include <Poco/Util/IniFileConfiguration.h>
#include <Poco/AutoPtr.h>
#include <Poco/Net/HTTPServer.h>

using namespace Poco::Util;
using namespace Poco;
using namespace Poco::Net;

#include "httprequestfactory.h"
#include "club.h"


int main(int argc, char* argv[]) {
	Club::log(LOG_INFO, "Starting ClubStatus server...");
	
	int rc;
	mosqpp::lib_init();
	
	Club::log(LOG_INFO, "Initialised C++ Mosquitto library.");
	
	// Read configuration.
	string configFile;
	if (argc > 1) { configFile = argv[1]; }
	else { configFile = "config.ini"; }
	
	AutoPtr<IniFileConfiguration> config;
	try {
		config = new IniFileConfiguration(configFile);
	}
	catch (Poco::IOException &e) {
		Club::log(LOG_ERROR, "Main: I/O exception when opening configuration file: " + configFile + ". Aborting...");
		return 1;
	}
	
	string mqtt_host = config->getString("MQTT.host", "localhost");
	int mqtt_port = config->getInt("MQTT.port", 1883);
	string mqtt_user = config->getString("MQTT.user", "");
	string mqtt_pass = config->getString("MQTT.pass", "");
	string mqtt_topic = config->getString("MQTT.clubStatusTopic", "/public/clubstatus");
	bool relaypresent = config->getBool("Relay.present", true);
	uint8_t relayaddress = config->getInt("Relay.address", 0x20);
	
	// Start the MQTT listener.
	Listener listener("ClubStatus", mqtt_host, mqtt_port, mqtt_user, mqtt_pass);
	
	Club::log(LOG_INFO, "Created listener, entering loop...");
	
	// Initialise the HTTP server.
	// TODO: catch IOException if HTTP port is already taken.
	UInt16 port = config->getInt("HTTP.port", 80);
	HTTPServerParams* params = new HTTPServerParams;
	params->setMaxQueued(100);
	params->setMaxThreads(10);
	HTTPServer httpd(new RequestHandlerFactory, port, params);
	try {
		httpd.start();
	}
	catch (Poco::IOException &e) {
		Club::log(LOG_ERROR, "I/O Exception on HTTP server: port already in use?");
		return 1;
	}
	catch (...) {
		Club::log(LOG_ERROR, "Exception thrown for HTTP server start. Aborting.");
		return 1;
	}
	
	Club::log(LOG_INFO, "Initialised the HTTP server.");
	
	// Initialise the GPIO and i2c-related handlers.
	Club::mqtt = &listener;
	Club::start(relaypresent, relayaddress, mqtt_topic);
	
	//cout << "Started the Club." << endl;
	
	while(1) {
		rc = listener.loop();
		if (rc){
			Club::log(LOG_ERROR, "Disconnected. Trying to reconnect...");
			listener.reconnect();
		}
	}
	
	Club::log(LOG_INFO, "Cleanup...");

	mosqpp::lib_cleanup();
	httpd.stop();
	Club::stop();

	return 0;
}
