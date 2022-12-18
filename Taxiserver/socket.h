#ifndef __SOCKET_H_
#define __SOCKET_H_

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>


class Socket
{
private:
    
protected:
    int sock;

public:
    void socketcreate();

};

#endif