#ifndef __BIND_H_
#define __BIND_H_

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>

class Binding
{
public:
    struct sockaddr_in sock_adr;
    void createaddr(int port);
    void clnt_createaddr(char ip[], int port);
};

#endif