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


// Static initialisations.
string NymphServer::loggerName = "NymphServer";
Net::TCPServer* NymphServer::server;
bool NymphServer::running;


// --- START ---
bool NymphServer::start(int port) {
	try {
		server = new Net::TCPServer(new Net::TCPServerConnectionFactoryImpl<NymphSession>(),
								port
								);
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
