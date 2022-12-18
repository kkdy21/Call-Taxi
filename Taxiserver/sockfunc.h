#ifndef __SOCKFUNC_H_
#define __SOCKFUNC_H_

#include "socket.h"
#include "Bind.h"

class Sockfunc : public Socket, public Binding
{

public:
    void binder();
    void listener();
    void connecter();
};

#endif