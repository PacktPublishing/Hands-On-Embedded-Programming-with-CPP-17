/*
	nymph_socket_listener.h	- Declares the NymphRPC Socket Listener class.
	
	Revision 0
	
	Notes:
			- 
			
	History:
	2017/06/24, Maya Posch : Initial version.
	
	(c) Nyanko.ws
*/

#include "nymph_socket_listener.h"
#include "nymph_logger.h"
#include "nymph_listener.h"

using namespace std;

#include <Poco/NumberFormatter.h>

using namespace Poco;


// --- CONSTRUCTOR ---
NymphSocketListener::NymphSocketListener(NymphSocket socket, Condition* cnd, Mutex* mtx) {
	loggerName = "NymphSocketListener";
	listen = true;
	init = true;
	this->nymphSocket = socket;
	this->socket = socket.socket;
	this->readyCond = cnd;
	this->readyMutex = mtx;
}


// --- DECONSTRUCTOR ---
NymphSocketListener::~NymphSocketListener() {
	//
}


// --- RUN ---
void NymphSocketListener::run() {
	Poco::Timespan timeout(0, 100); // 100 microsecond timeout
	
	NYMPH_LOG_INFORMATION("Start listening...");
	
	char headerBuff[8];
	while (listen) {
		if (socket->poll(timeout, Net::Socket::SELECT_READ)) {
			// Attempt to receive the entire message.
			// First validate the header (0x4452474e), then read the uint32
			// following it. This contains the data length (LE format).
			int received = socket->receiveBytes((void*) &headerBuff, 8);
			if (received == 0) {
				// Remote disconnnected. Socket should be discarded.
				NYMPH_LOG_INFORMATION("Received remote disconnected notice. Terminating listener thread.");
				break;
			}
			else if (received < 8) {
				// TODO: try to wait for more bytes.
				NYMPH_LOG_WARNING("Received <8 bytes: " + NumberFormatter::format(received));
				
				continue;
			}
			
			UInt32 signature = *((UInt32*) &headerBuff[0]);
			if (signature != 0x4452474e) { // 'DRGN' ASCII in LE format.
				// TODO: handle invalid header.
				NYMPH_LOG_ERROR("Invalid header: " + NumberFormatter::formatHex(signature));
				
				continue;
			}
			
			UInt32 length = 0;
			length = *((UInt32*) &headerBuff[4]);
			/* for (int k = 4; k < 8; ++k) {
				length = length | ((UInt8) headerBuff[k] << ((7 - k) * 8));
			} */
			
			NYMPH_LOG_DEBUG("Message length: 0x" + NumberFormatter::formatHex(length));
			
			char* buff = new char[length];
			
			// Read the entire message into a string which is then used to
			// construct an NymphMessage instance.
			received = socket->receiveBytes((void*) buff, length);
			string* binMsg;
			if (received != length) {
				// Handle incomplete message.
				NYMPH_LOG_WARNING("Incomplete message: " + NumberFormatter::format(received) + " of " + NumberFormatter::format(length));
				
				// Loop until the rest of the message has been received.
				// TODO: Set a maximum number of loops/timeout? Reset when 
				// receiving data, timeout when poll times out N times?
				binMsg = new string((const char*) buff, received);
				binMsg->reserve(length);
				int unread = length - received;
				while (1) {
					if (socket->poll(timeout, Net::Socket::SELECT_READ)) {
						char* buff1 = new char[unread];
						received = socket->receiveBytes((void*) buff1, unread);
						if (received == 0) {
							// Remote disconnnected. Socket should be discarded.
							NYMPH_LOG_INFORMATION("Received remote disconnected notice. Terminating listener thread.");
							delete[] buff1;
							break;
						}
						else if (received != unread) {
							binMsg->append((const char*) buff1, received);
							delete[] buff1;
							unread -= received;
							NYMPH_LOG_WARNING("Incomplete message: " + NumberFormatter::format(unread) + "/" + NumberFormatter::format(length) + " unread.");
							continue;
						}
						
						// Full message was read. Continue with processing.
						binMsg->append((const char*) buff1, received);
						delete[] buff1;
						break;
					} // if
				} //while
			}
			else { 
				NYMPH_LOG_DEBUG("Read 0x" + NumberFormatter::formatHex(received) + " bytes.");
				binMsg = new string(((const char*) buff), length);
			}
			
			delete[] buff;
			
			// Parse the string into an NymphMessage instance.
			NymphMessage* msg = new NymphMessage(binMsg);
			delete binMsg;
			
			// The 'In Reply To' message ID in this message is now used to notify
			// the waiting thread that a response has arrived, along with the
			// received message.
			UInt64 msgId = msg->getResponseId();
			if (msg->isCallback()) {
				NYMPH_LOG_INFORMATION("Callback received. Trying to find registered method.");
				
				if (!NymphListener::callCallback(msg, nymphSocket.data)) {
					NYMPH_LOG_ERROR("Calling callback failed. Skipping message.");
					delete msg;
					continue;
				}
				
				NYMPH_LOG_INFORMATION("Calling callback succeeded.");
				delete msg;
				continue; // We're done with this request.
			}
			
			NYMPH_LOG_DEBUG("Found message ID: 0x" + NumberFormatter::formatHex(msgId) + ".");
			
			messagesMutex.lock();
			map<UInt64, NymphRequest*>::iterator it;
			it = messages.find(msgId);
			if (it == messages.end()) {
				// Message ID not found.
				NYMPH_LOG_ERROR("Message ID 0x" + NumberFormatter::formatHex(msgId) + " not found.");
				messagesMutex.unlock();
				delete msg;
				continue;
			}
			
			NymphRequest* req = it->second;
			req->mutex.lock();
			if (msg->isReply()) { req->response = msg->getResponse(); }
			else if (msg->isException())  {
				req->exception = true;
				req->response = 0;
				req->exceptionData = msg->getException();
			}				
			else { req->response = 0; }
			req->condition.signal();
			req->mutex.unlock();
			
			NYMPH_LOG_INFORMATION("Signalled condition for message ID " + NumberFormatter::formatHex(msgId) + ".");
			
			messagesMutex.unlock();
			delete msg;
		}
		
		// Check whether we're still initialising.
		if (init) {
			// Signal that this listener thread is ready.
			readyMutex->lock();
			readyCond->signal();
			readyMutex->unlock();
			
			timeout.assign(1, 0); // Change timeout to 1 second.
			init = false;
		}
	}
	
	NYMPH_LOG_INFORMATION("Stopping thread...");
	
	// Clean-up.
	delete readyCond;
	delete readyMutex;
	nymphSocket.semaphore->wait();	// Wait for the connection to be closed.
	delete socket;
	delete nymphSocket.semaphore;
	nymphSocket.semaphore = 0;
	delete this; // Call the destructor ourselves.
}


// --- STOP ---
void NymphSocketListener::stop() {
	listen = false;
}


// --- ADD MESSAGE ---
// Add a message this listener instance will be waiting for.
bool NymphSocketListener::addMessage(NymphRequest* &request) {
	messagesMutex.lock();
	messages.insert(std::pair<UInt64, NymphRequest*>(request->messageId, request));
	messagesMutex.unlock();
	
	return true;
}


// --- REMOVE MESSAGE ---
bool NymphSocketListener::removeMessage(UInt64 messageId) {
	messagesMutex.lock();
	map<UInt64, NymphRequest*>::iterator it;
	it = messages.find(messageId);
	if (it == messages.end()) { messagesMutex.unlock(); return true; }
	
	messages.erase(it);
	messagesMutex.unlock();
	
	return true;
}
