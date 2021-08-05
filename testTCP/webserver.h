#include "../epoller/epoller.h"
#include <sys/types.h>
#include <sys/uio.h>     // readv/writev
#include <arpa/inet.h>   // sockaddr_in
#include <stdlib.h>      // atoi()
#include <errno.h>
#include"../pool/pool.h"
using namespace std;
class webserver
{
public:
    webserver(int port, int sqlPort, const char* sqlUser, const  char* sqlPwd,
        const char* dbName);
    int initSocketAndSqlPool();
    void addClient(sockaddr_in addr);
    void getClientPort();
    void start();
    ~webserver();
private:
    int port;
    int sqlPort;
    const char* sqlUser;
    const  char* sqlPwd;
    const char* dbName;
    int listenfd;

    int usercount=0;
   
};

