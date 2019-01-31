/*
	nymph_server.h	- header file for the NymphRPC Server class.
	
	Revision 0
	
	Notes:
			- This class declares the server class to be used by Nymph servers.
			
	History:
	2017/06/24, Maya Posch : Initial version.
	
	(c) Nyanko.ws
*/


#pragma once
#ifndef NYMPH_SERVER_H
#define NYMPH_SERVER_H

#include <string>

using namespace std;

#include <Poco/Net/TCPServer.h>


class NymphServer {
	static string loggerName;
	static Poco::Net::TCPServer* server;
	
public:
	static bool running;
	
	static bool start(int port = 4004);
	static bool stop();
};

#endif
