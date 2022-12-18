Bind.o : Bind.h Bind.cpp
	g++ -c Bind.cpp
socket.o : socket.cpp socket.h
	g++ -c socket.cpp
sockfunc.o : sockfunc.h sockfunc.cpp
	g++ -c sockfunc.cpp
tcpmain.o : tcpmain.h tcpmain.cpp 
	g++ -c tcpmain.cpp -lpthread
readthread.o : readthread.h readthread.cpp
	g++ -c readthread.cpp -lmysqlclient
main.o : main.cpp tcpmain.h
	g++ -c main.cpp
start: Bind.o socket.o sockfunc.o tcpmain.o readthread.o main.o
	g++ -o start Bind.o socket.o sockfunc.o tcpmain.o readthread.o main.o -lpthread -lmysqlclient
