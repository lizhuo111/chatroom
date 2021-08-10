#include "client.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include"stdio.h"
#include"sys/stat.h"
extern "C" {
#include"hiredis/hiredis.h"
}
#include<sstream>
#include<fstream>

client::client() {
	isConnect = false;
	username = "";
}
void client::Conn()
{
	int ret=socket(AF_INET,SOCK_STREAM,0);
	if (ret < 0)	cout << "failed cre port";
	sockaddr_in clientsockaddr;
	clientsockaddr.sin_family = AF_INET;
	clientsockaddr.sin_port = htons(0);
	clientsockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(connect(ret, (struct sockaddr* )&clientsockaddr, sizeof(clientsockaddr))<0) cout<<"conn failed";
	else
	{
		isConnect = true;
	}
}

bool client::SendFile(string filename) {
	string a = "../resources/" + filename;
	char* p = const_cast<char*>(a.c_str());
	if (stat(p, &buf) != 0) {
		cout << "file not exist";
		return false;
	}
	else{
		ofstream out;
		out.open(a);
		stringstream res;
		res<<out.rdbuf();
		out.close();

	}
}

bool client::connRedis()
{
	this-> pRedisContext = (redisContext*)redisConnect("127.0.0.1", 6379);
	if (pRedisContext->err != 0) { cout << "redis err"; return false; }
	else {
		cout << "connecti successfully";
		return true; }
	
}

void client::exitRedis()
{
	redisFree(pRedisContext);
}
