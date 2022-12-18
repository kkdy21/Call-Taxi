#include "tcpmain.h"
#include <thread>

void Tcpmain::MainConnect()
{
    socketcreate();
    createaddr(port);
    binder();
    listener();
}

void Tcpmain::accepter()
{
    while (true)
    {
        
        adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(sock, (struct sockaddr *)&clnt_adr, (socklen_t *)&adr_sz);
        cout<<"hi"<<endl;
        // std::thread t1([&]()
        //               { recv(clnt_sock); });
        thread t1([&]()
                       { ReadThread_func(clnt_sock); });

        t1.detach();
    }
}

void Tcpmain::Servermain()
{
    MainConnect();
    accepter();
}