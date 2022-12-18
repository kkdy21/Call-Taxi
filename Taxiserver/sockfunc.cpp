#include "sockfunc.h"

void Sockfunc::binder()
{
    if (bind(sock, (struct sockaddr *)&sock_adr, sizeof(sock_adr)) == -1)
    {
        std::cout << "bind 오류 " << std::endl;
        exit(1);
    }
}

void Sockfunc::listener()
{
    if (listen(sock, 5) == -1)
    {
        std::cout << "listen 오류 " << std::endl;
        exit(1);
    }
}

void Sockfunc::connecter()
{
    if (connect(sock, (struct sockaddr *)&sock_adr, sizeof(sock_adr)) == -1)
    {
        std::cout << "connect 오류 " << std::endl;
        exit(1);
    }
}
