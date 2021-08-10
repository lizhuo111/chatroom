#pragma once
#include<sys/fcntl.h>
#include<iostream>
#include<string>
#include "../buffer/buffer.h"
#include"hiredis/hiredis.h"
using namespace std;
class client
{
public:
	client();
	void Conn();
	bool SendFile(string filename);
	bool connRedis();
	void exitRedis();
	
	/*bool DownloadFile(int fileId);
	void chatPub();
	void chatPubExit();
	void chatPrivate(string username);
	void chatPriExit();*/

private:
	string username;
	bool isConnect;
	Buffer wbuf;
	Buffer rbuf;
	redisContext* pRedisContext;
	struct stat buf;

};

