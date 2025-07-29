#include "Connection.hpp"

Connection::Connection()
{
	socket = 0;
	isReady = false;
	bodyLength = 0;
	contentLength = 0;
	
	//expiresOn = time(NULL) + (CON_SOC_TIMEOUT_SECS);
	rawPostBody.clear();

}

Connection::Connection(int socket, ServerConfig config):socket(socket), serverConfig(config),isReady(false)
{
	bodyLength = 0;
	contentLength = 0;
	
	//expiresOn = time(NULL) + (CON_SOC_TIMEOUT_SECS);
    int flags = fcntl(socket, F_GETFL, 0);
	if (flags >= 0) {
		if( fcntl(socket, F_SETFL, flags | O_NONBLOCK) == -1)
			throw std::runtime_error("Failed  to set socket to non-blocking mode");
	}
	rawPostBody.clear();


}

void Connection::clear()
{
	rawPostBody.clear();
	isReady = false;
	headerIsCompleted = false;
	requestIsCompleted = false;
	responseBuffer = "";
	contentLength = 0;
	bodyLength = 0;
	
}
