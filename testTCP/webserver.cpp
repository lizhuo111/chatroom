#include "webserver.h"
#include "../epoller/epoller.h"
#include <sys/types.h>
#include <sys/uio.h>     // readv/writev
#include <arpa/inet.h>   // sockaddr_in
#include <stdlib.h>      // atoi()
#include <errno.h>


webserver::webserver(int port, int sqlPort, const char* sqlUser, const char* sqlPwd, const char* dbName)
{
	this->port = port;
	this->sqlPort = sqlPort;
	this->sqlUser = sqlUser;
	this->sqlPwd = sqlPwd;
	this->dbName = dbName;
}

int webserver::initSocketAndSqlPool()
{	

	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port = port;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);   ///* Address to accept any incoming messages.  */

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	bind(listenfd, (sockaddr*)&addr, sizeof(addr));
	listen(listenfd, 10);
	fcntl(listenfd, F_SETFL, fcntl(listenfd, F_GETFD, 0) | O_NONBLOCK);
	pool::Instance()->Init("localhost", sqlPort, sqlUser, sqlPwd, dbName,10);

}

void webserver::addClient(sockaddr_in addr)
{
	usercount++;
}

void webserver::getClientPort()
{
	sockaddr_in client_addr;
	socklen_t len = sizeof(client_addr);
	accept(listenfd, (sockaddr*)&client_addr, &len);
	addClient(client_addr);
}

void webserver::start()
{
	initSocketAndSqlPool;
	getClientPort();
}

webserver::~webserver()
{
	close(listenfd);
	pool::Instance()->ClosePool();
}




