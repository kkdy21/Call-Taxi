#ifndef __TCPMAIN_H_
#define __TCPMAIN_H_

#include "sockfunc.h"
#include "readthread.h"
using namespace std;

class Tcpmain : public Sockfunc, public ReadThread
{
private:
    int port = 5001;
    int adr_sz;
    int clnt_sock;
    struct sockaddr_in clnt_adr;

public:
    void MainConnect();
    void Servermain();
    void accepter();
};

#endif