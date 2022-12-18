#include "Bind.h"

void Binding::createaddr(int port)
{
    memset(&sock_adr, 0, sizeof(sock_adr));
    sock_adr.sin_family = AF_INET;
    sock_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    sock_adr.sin_port = htons(port);
}

void Binding::clnt_createaddr(char ip[], int port)
{
    memset(&sock_adr, 0, sizeof(sock_adr));
    sock_adr.sin_family = AF_INET;
    sock_adr.sin_addr.s_addr = inet_addr(ip);
    sock_adr.sin_port = htons(port);
}