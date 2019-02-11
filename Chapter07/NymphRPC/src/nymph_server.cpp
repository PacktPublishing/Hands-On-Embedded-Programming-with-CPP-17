/*
	nymph_server.cpp	- implementation for the NymphRPC Server class.
	
	Revision 0
	
	Notes:
			- This class implements the server class to be used by Nymph servers.
			
	History:
	2017/06/24, Maya Posch : Initial version.
	
	(c) Nyanko.ws
*/


#include "nymph_server.h"
#include "nymph_logger.h"
#include "nymph_session.h"

#include <Poco/Net/NetException.h>

using namespace Poco;

using namespace std;


// Static initialisations.
string NymphServer::loggerName = "NymphServer";
Poco::Net::ServerSocket NymphServer::ss;
Net::TCPServer* NymphServer::server;
bool NymphServer::running;


// --- START ---
bool NymphServer::start(int port) {
	try {
		// Create a server socket that listens on all interfaces, IPv4 and IPv6.
		// Assign it to the new TCPServer.
		ss.bind6(port, true, false); // Port, SO_REUSEADDR, IPv6-only.
		ss.listen();
		server = new Net::TCPServer(new Net::TCPServerConnectionFactoryImpl<NymphSession>(),
									ss);
		server->start();
	}
	catch (Net::NetException& e) {
		NYMPH_LOG_ERROR("Error starting TCP server: " + e.message());
		return false;
	}
	
	running = true;
	return true;
}


// --- STOP ---
bool NymphServer::stop() {
	server->stop();
	running = false;
	return true;
}
