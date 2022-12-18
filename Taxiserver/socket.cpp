#include "socket.h"

void Socket::socketcreate()
{
    sock = socket(PF_INET, SOCK_STREAM, 0);
}