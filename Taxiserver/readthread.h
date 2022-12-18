#ifndef __READTHREAD_H_
#define __READTHREAD_H_

#include <map>
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include "connect.h"
#include <thread>

#define BUFF 1024
#define MSG 200
using namespace std;

class ReadThread : public Connect
{
public:
    map<int, string> ClientList;
    map<int, string> DriverList; //기사
    MYSQL_RES *sql_result;
    MYSQL_ROW sql_row;
    int size;

    void ReadThread_func(int clnt_sock);
    void SendMap_func(int MyClntsock);

    void Login_func(int clnt_sock, char myID[]);
    void register_func(int clnt_sock);
    void call_add(int clnt_sock);
    void call_delete(int clnt_sock);
    void drive(int clnt_sock);
    void waite(int clnt_sock);
    void accept_drive(int clnt_sock);
    void payment(int clnt_sock);
    void Deleteinfo_func(int MyClntsock);
    void ResetCallList_func(int MyClntsock);
    void MyInfo_func(int MyClntsock, char myID[]);
    void EndThread(int MyClntsock);
    void geton_func(int MyClntsock);
    int FindcustomerSock(string ID);
    int FindDriverSock(string ID);
    void ReviewData(int MyClntsock);


    void SignUpCustomer(int clntSock);
    void SignInCustomer(int clntSock, char myID[]);
    void MapCustomer(int clntSock);
    void CallTaxiCustomer(int clntSock, char myID[]);
    void HistoryCustomer(int clntSock, char myID[]);
    void CallFailedCustomer(int clntSock, char myID[]);
    void TakeTaxiCustomer(int clntSock, char myID[]);
    void GetOnCustomer(int clntSock);
    void GetOffCustomer(int clntSock, char myID[]);
    void ReviewCustomer(int clntSock, char myID[]);
};

#endif
