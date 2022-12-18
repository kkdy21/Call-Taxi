#include "readthread.h"
using namespace std;
void ReadThread::ReadThread_func(int clnt_sock)
{
    char myID[20] = {0};
    int MyClntsock = clnt_sock;
    cout << "클라이언트 디스크립터 : " << clnt_sock << endl;

    //ClientList.insert(pair<int, string>(MyClntsock, "testid"));

    while (true)
    {
        char buf[128] = "\0";

        int len = read(MyClntsock, buf, sizeof(buf));
        cout << "read받음" << endl;
        cout << "buf :" << buf << endl;
        if (len == 0 || len == -1)
        {
            Deleteinfo_func(MyClntsock);
            break;
        }
        else if (!strcmp(buf, "SendMap"))
            SendMap_func(MyClntsock);
        else if (!strcmp(buf, "register"))
        { //회원가입
            register_func(MyClntsock);
        }
        else if (!strcmp(buf, "login"))
        { //로그인
            Login_func(MyClntsock, myID);
        }
        else if (!strcmp(buf, "MyInfo"))
            MyInfo_func(MyClntsock, myID);
        else if (!strcmp(buf, "call_add"))
        { //콜 호출
            call_add(MyClntsock);
        }
        else if (!strcmp(buf, "call_delete"))
        { //호출 거절
            call_delete(MyClntsock);
        }
        else if (!strcmp(buf, "delete"))
        { //호출 취소
        }
        else if (!strcmp(buf, "drive"))
        { //호출 대기
            drive(MyClntsock);
        }
        else if (!strcmp(buf, "waite"))
        { //운행 대기
            waite(MyClntsock);
        }
        else if (!strcmp(buf, "accept_drive"))
        { //호출 수락
            accept_drive(MyClntsock);
        }
        else if (!strcmp(buf, "payment"))
        { //계산
            payment(MyClntsock);
        }
        else if(!strcmp(buf,"ReviewData"))
        {//리뷰보기
            ReviewData(MyClntsock);
        }
        else if (!strcmp(buf, "geton"))
            geton_func(MyClntsock);

        else if (!strcmp(buf, "EndThread") || !strcmp(buf,"history_C1"))
            EndThread(MyClntsock);

        else if (!strcmp(buf, "ResetCallList"))
            ResetCallList_func(MyClntsock);
        else if (!strcmp(buf, "signIn_C"))
            SignInCustomer(MyClntsock, myID);
        else if (!strcmp(buf, "signUp_C"))
            SignUpCustomer(MyClntsock);
        else if (!strcmp(buf, "map_C"))
            MapCustomer(MyClntsock);


        else if (!strcmp(buf, "call_C"))
            CallTaxiCustomer(MyClntsock, myID);
        else if (!strcmp(buf, "history_C"))
            HistoryCustomer(MyClntsock, myID);
        else if (!strcmp(buf, "failed_C") || !strcmp(buf, "cancel_C"))
            CallFailedCustomer(MyClntsock, myID);
        else if (!strcmp(buf, "take_C"))
            TakeTaxiCustomer(MyClntsock, myID);
        else if (!strcmp(buf, "getOn_C"))
            GetOnCustomer(MyClntsock);
        else if (!strcmp(buf, "getOff_C"))
            GetOffCustomer(MyClntsock, myID);
        else if (!strcmp(buf, "review_C"))
            ReviewCustomer(MyClntsock, myID);

        else
            std::cout << "오류" << std::endl;
    }
}

void ReadThread::geton_func(int MyClntsock)
{
    cout << "geton들어옴" << endl;
    char ClientIdBuf[40] = "\0";

    read(MyClntsock, ClientIdBuf, sizeof(ClientIdBuf));

    int sock = FindcustomerSock(ClientIdBuf);
    cout<<"소켓 , 아이디 "<< sock<<ClientIdBuf<<endl;



    char flag[] = "geton";
    write(sock, flag, strlen(flag));
}

void ReadThread::MyInfo_func(int MyClntsock, char myID[])
{
    string MyID(myID);
    cout << "MyInfo_func 들어옴" << endl;
    double total = 0;
    string query1 = "select * from usedlist where DriverID = ";
    query1 += "'" + MyID + "'";
    mysql_query(connection, query1.c_str());
    sql_result = mysql_store_result(connection);
    cout << "거리 query : " << query1 << endl;
    while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
    {
        string temp = sql_row[2];
        double inttemp = stod(temp);
        total += inttemp;
    }
    mysql_free_result(sql_result);

    //차량번호, 총 운행, 총 가격
    string query2 = "select * from driver where DriverID = ";
    query2 += "'" + MyID + "'";
    cout << "차량번호 쿼리 : " << query2 << endl;

    mysql_query(connection, query2.c_str());
    sql_result = mysql_store_result(connection);

    string temp_carnum = "";
    string temp_totalcount = "";
    string temp_totalprice = "";
    string s_total = to_string(total);

    while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
    {
        temp_carnum = sql_row[3];
        temp_totalcount = sql_row[4];
        temp_totalprice = sql_row[5];
    }
    mysql_free_result(sql_result);

    string send = s_total + "@" + temp_carnum + "@" + temp_totalcount + "@" + temp_totalprice;
    cout << "거리 | 차량번호 | 운행 횟수 | 총 금액 | " << send << endl;
    write(MyClntsock, send.c_str(), strlen(send.c_str()));
}

void ReadThread::ResetCallList_func(int MyClntsock)
{

    char query[70] = "select * from calllist where Status ='wait'";
    mysql_query(connection, query);
    sql_result = mysql_store_result(connection);

    string result = "^";
    while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
    {
        result += sql_row[4];
        result += "@";
        result += sql_row[0];
        result += "@";
        result += sql_row[1];
        result += "@";
        result += sql_row[7];
        result += "@";
        result += sql_row[8];
        result += "^";
    }

    cout << result << endl;
    mysql_free_result(sql_result);

    char data[2048];
    strcpy(data, result.c_str());

    char flag[] = "ResetCallList";
    write(MyClntsock, flag, strlen(flag));
    usleep(10000);
    write(MyClntsock, data, strlen(data));
}

void ReadThread::Deleteinfo_func(int MyClntsock) //map 삭제
{
    if (ClientList.find(MyClntsock) != ClientList.end())
    {
        ClientList.erase(MyClntsock);
    }
    else
    {
        DriverList.erase(MyClntsock);
    }
    close(MyClntsock);
}

void ReadThread::EndThread(int MyClntsock)
{
    char buf[] = "EndThread";
    write(MyClntsock, buf, strlen(buf));
}

void ReadThread::SendMap_func(int MyClntsock)
{
    size_t bufsize = 256 * 256;
    int nbyte = 256;
    char buf[256 * 256] = "\0";
    int size;

    read(MyClntsock, buf, bufsize);
    size = atoi(buf);
    printf("총 파일 크기 %d\n", size);

    map<int, string>::iterator iter;

    for (iter = ClientList.begin(); iter != ClientList.end(); iter++)
    {
        cout << iter->first << endl;

        if (iter->first != MyClntsock)
        {
            while (size > 0)
            {
                nbyte = read(MyClntsock, buf, sizeof(buf));
                size -= nbyte;
                printf("받은 파일크기 : %d\n", nbyte);
                write(iter->first, buf, nbyte);
                cout << "남은 파일 크기 : " << size << endl;
                usleep(500000);
            }
            char flag[] = "end";
            write(iter->first, flag, strlen(flag));
            cout << "end보냄" << endl;
        }
    }
}

void ReadThread::register_func(int clnt_sock) //회원가입
{
    char message[MSG] = "\0";
    char query[MSG] = "\0";
    cout << "register 들어옴" << endl;
    if (read(clnt_sock, message, sizeof(message)) != -1)
    {
        cout << "message : " << message << endl;
        char *ID = strtok(message, "@");
        char *Password = strtok(NULL, "@");
        char *Name = strtok(NULL, "@");
        char *Car_name = strtok(NULL, "@");

        std::cout << "ID : " << ID << " Password : " << Password << " Car_name :" << Car_name << std::endl;

        memset(query, 0, sizeof(query));
        sprintf(query, "insert into driver (DriverID, PW, Name, VehicleNumber, DrivingCount, TotalPrice, Currentstate) values('%s', '%s', '%s', '%s', '0', '0','wait');", ID, Password, Name, Car_name);
        int t = mysql_query(connection, query);
        if (t != 0)
        {
            std::cout << "error : " << query << endl;
              char res = '0';
            write(clnt_sock, &res, sizeof(res));
        }
        else
        {
            char res = '1';
            write(clnt_sock, &res, sizeof(res));
        }
    }
}

void ReadThread::Login_func(int clnt_sock, char myID[]) //로그인
{
    char message[MSG] = "\0";
    //char query[MSG] = "\0";

    cout << "login 들어옴" << endl;
    if (read(clnt_sock, message, sizeof(message)) != -1)
    {
        cout << "message : " << message << endl;
        char *ID = strtok(message, "@");
        char *Password = strtok(NULL, "@");
        string tempID(ID);
        cout << "ID : " << ID << " Password : " << Password << std::endl;
        cout << "hi" << endl;
        string Query = "select count(*) from driver where DriverID = '";

        Query += tempID + "'and PW = '" + Password + "'";
        cout << "query 전: " << Query << endl;
        mysql_query(connection, Query.c_str());
        sql_result = mysql_store_result(connection);
        //===================방법 1
        sql_row = mysql_fetch_row(sql_result);
        string temp = sql_row[0];

        mysql_free_result(sql_result);
        if (temp == "1")
        {
            cout << "로그인성공" << endl;
            DriverList.insert(make_pair(clnt_sock, ID));
            char res = '1';
            strcpy(myID, ID);
            // cout<<"터짐?"<<endl;
            write(clnt_sock, &res, sizeof(res));
        }
        else
        {
            cout << "로그인실패" << endl;
            char res = '2';
            write(clnt_sock, &res, sizeof(res));
        }
    }
}

void ReadThread::call_add(int clnt_sock) //손님 호출
{
    cout << "call_add" << endl;
    char id[20] = {0};

    // run인 기사들한테 호출신호
    map<int, string>::iterator drive_iter;
    for (drive_iter = DriverList.begin(); drive_iter != DriverList.end(); drive_iter++)
    {
        char tempquery[70] = "select Currentstate from driver where DriverID";
        char query[100] = "\0";
        sprintf(query, "%s = '%s'", tempquery, drive_iter->second.c_str()); // iter의 first값 (clnt_sock)
        cout << query << endl;

        mysql_query(connection, query);
        sql_result = mysql_store_result(connection);
        sql_row = mysql_fetch_row(sql_result);

        string temp = sql_row[0];
        cout << "호출 함수에서 temp : " << temp << endl;

        mysql_free_result(sql_result);
        if (temp == "run")
        {
            ResetCallList_func(drive_iter->first);
        }
    }
}

void ReadThread::call_delete(int clnt_sock) //호출 거절
{
    //호출 거절
    //손님 아이디 받아와서 찾아서 해주기
}

void ReadThread::drive(int clnt_sock) //호출대기
{
    char query[MSG] = "\0";
    char start[6] = "start";
    char id[20] = "\0";
    string t = "";

    map<int, string>::iterator Driver_iter;
    for (Driver_iter = DriverList.begin(); Driver_iter != DriverList.end(); Driver_iter++)
    {
        if (clnt_sock == Driver_iter->first)
        {
            t = Driver_iter->second;
            sprintf(id, "%s", t.c_str());
            break;
        }
    }

    memset(query, 0, sizeof(query));
    sprintf(query, "update driver set Currentstate = 'run' where DriverID = '%s'", id);
    int res = mysql_query(connection, query);
    if (res != 0)
        cout << query << endl;
}

void ReadThread::waite(int clnt_sock) //운행 대기
{
    char query[MSG] = "\0";
    char waite[6] = "waite";

    string id = DriverList[clnt_sock];

    memset(query, 0, sizeof(query));
    sprintf(query, "update driver set Currentstate = 'wait' where DriverID = '%s';", id.c_str());
    int res = mysql_query(connection, query);
    if (res != 0)
        cout << "오류";
}

void ReadThread::accept_drive(int clnt_sock) //호출 수락
{
    cout << "호출수락 들어옴" << endl;
    char id[30] = "\0";

    read(clnt_sock, id, sizeof(id)); //손님아이디
    cout << "손님아이디 :" << id << endl;

    string tempID(id);

    //손님한테 보낼정보
    int CustomerSock = FindcustomerSock(tempID);
    char driverID[100] = "\0";

    cout << "드라이버아이디" << ClientList[CustomerSock] << endl;

    strcpy(driverID, DriverList[clnt_sock].c_str());
    cout << driverID << endl;
    char signal[] = "accept";

    cout<<CustomerSock<<tempID<<"손님 소켓, 손님아이디"<<endl;
    write(CustomerSock, signal, strlen(signal));
    usleep(40000);
    write(CustomerSock, driverID, strlen(driverID));

    //다른 기사들한테 보내는 거
    map<int, string>::iterator drive_iter;
    for (drive_iter = DriverList.begin(); drive_iter != DriverList.end(); drive_iter++)
    {
        if (clnt_sock != drive_iter->first)
        {
            char signal[] = "DeleteCallList";

            char tempquery[70] = "select Currentstate from driver where DriverID";
            char query[100] = "\0";
            sprintf(query, "%s = '%s'", tempquery, drive_iter->second.c_str()); // iter의 first값 (아이디)
            cout << query << endl;

            mysql_query(connection, query);

            sql_result = mysql_store_result(connection);

            sql_row = mysql_fetch_row(sql_result);

            string temp = sql_row[0];
            cout << "호출 함수에서 temp : " << temp << endl;

            if (temp == "run")
            {
                cout << "들어옴" << endl;
                write(drive_iter->first, signal, strlen(signal)); //flag 보내고
                cout << "sock : " << drive_iter->second << endl;
                usleep(10000);
                write(drive_iter->first, id, strlen(id)); //손님아이디
                cout << "이놈한테 보냄 : " << drive_iter->first << endl;
            }
            mysql_free_result(sql_result);
        }
        else if (clnt_sock == drive_iter->first)
        {
            char query[200] = "\0";
            sprintf(query, "update calllist set Status = 'accept', DriverID = '%s' where CustomerID = '%s'", drive_iter->second.c_str(), id);
            cout << query << endl;

            int res = mysql_query(connection, query);
            if (res != 0)
                cout << query << " 오류! " << endl;
        }
    }
}

void ReadThread::payment(int clnt_sock) //결제
{
    //결제
    char buf[200] = "\0";
    read(clnt_sock, buf, sizeof(buf));

    cout << buf << endl;

    char *distance = strtok(buf, "_");
    char *price = strtok(NULL, "_");
    char query[500] = "\0";

    sprintf(query, "update driver set DrivingCount = DrivingCount + 1, TotalPrice = TotalPrice +'%s' where DriverID = '%s'", price, DriverList[clnt_sock].c_str());
    cout << query << endl;
    int res = mysql_query(connection, query);
    if (res != 0)
        cout << query << " 오류! " << endl;
}

int ReadThread::FindcustomerSock(string ID)
{
    map<int, string>::iterator client_iter;
    for (client_iter = ClientList.begin(); client_iter != ClientList.end(); client_iter++)
    {
        if (ID == client_iter->second)
        {
            return client_iter->first;
        }
    }
}

int ReadThread::FindDriverSock(string ID)
{
    map<int, string>::iterator d_iter;
    for (d_iter = DriverList.begin(); d_iter != DriverList.end(); d_iter++)
    {
        if (ID == d_iter->second)
        {
            return d_iter->first;
        }
    }
}

void ReadThread::SignUpCustomer(int clntSock)
{
    char *split;   // name
    char *info[2]; // id / pw
    char buf[100] = {0};
    char query[200] = {0};
    char flag[10] = {0};

    read(clntSock, buf, sizeof(buf));
    split = strtok_r(buf, "@", &info[0]);
    info[0] = strtok_r(info[0], "@", &info[1]);

    sprintf(query, "select CustomerID from customer where CustomerID = '%s'", info[0]);
    mysql_query(connection, query);

    sql_result = mysql_store_result(connection);
    int row_cnt = mysql_num_rows(sql_result);

    if (row_cnt > 0)
        strcpy(flag, "fail");
    else
        strcpy(flag, "done");

    write(clntSock, flag, strlen(flag));

    if (!strcmp(flag, "done"))
    {
        memset(query, 0, sizeof(query));
        sprintf(query, "insert into customer values ('%s', '%s', '%s', '0', '0', '0')", info[0], info[1], split);
        mysql_query(connection, query);
    }
}

void ReadThread::SignInCustomer(int clntSock, char myID[])
{
    char *id;
    char *pw;
    char buf[100] = {0};
    char query[200] = {0};
    char flag[10] = {0};

    read(clntSock, buf, sizeof(buf));
    id = strtok_r(buf, "@", &pw);

    cout<<"hi<<endl";

    sprintf(query, "select * from customer where CustomerID = '%s' and PW = '%s'", id, pw);
    mysql_query(connection, query);
    sql_result = mysql_store_result(connection);
    int row_cnt = mysql_num_rows(sql_result);
    cout<<"hi<<endl";
    
    if (row_cnt > 0)
        strcpy(flag, "done");
    else
        strcpy(flag, "fail");

    write(clntSock, flag, strlen(flag));

    if (!strcmp(flag, "done"))
    {
        strcpy(myID, id);
        ClientList.insert(pair<int, string>(clntSock, id));
    }
}

void ReadThread::MapCustomer(int clntSock) // edit later /////////////////////////
{
    size_t bufsize = 256 * 256;
    int nbyte = 256;
    char buf[256 * 256] = "\0";
    int size;
    char Driver[20] = {0};

    read(clntSock, buf, sizeof(buf)); //Driver ID
    strcpy(Driver, buf);

    cout << "driver id : " << Driver << endl;

    read(clntSock, buf, bufsize);
    size = atoi(buf);
    printf("총 파일 크기 %d", size); // size

    map<int, string>::iterator iter;

    for (iter = DriverList.begin(); iter != DriverList.end(); iter++)
    {
        cout << "iter돕니다." << endl;
        if (iter->second == Driver)
        {
            cout << iter->second<<"에게 전송합니다." << endl;
            while (size > 0)
            {
                nbyte = read(clntSock, buf, sizeof(buf));
                size -= nbyte;
                printf("받은 파일크기 : %d\n", nbyte);
                write(iter->first, buf, nbyte);
                cout << "남은 파일 크기 : " << size << endl;
                usleep(20000);
            }
            char flag[] = "end";
            write(iter->first, flag, strlen(flag));
            cout << "end보냄" << endl;
            break;
        }
    }
}

void ReadThread::CallTaxiCustomer(int clntSock, char myID[])
{
    char data[5][50] = {0}; // 0 출발지     1 목적지   2 거리      3 소요시간      4 요금
    char buf[300] = {0};
    char query[400] = {0};
    char saveBuf[250] = {0};

    read(clntSock, buf, sizeof(buf));
    strcpy(saveBuf, buf);
    char *ptr = strtok(buf, "@");
    for (int i = 0; ptr != NULL; i++)
    {
        strcpy(data[i], ptr);
        ptr = strtok(NULL, "@");
    }

    sprintf(query, "insert into calllist values ('%s', '%s', 'wait', 'NULL', '%s', 'NULL', '%s', '%s', '%s')", data[0], data[1], myID, data[3], data[2], data[4]);
    mysql_query(connection, query);
    puts(query);

    memset(query, 0, sizeof(query));
    sprintf(query, "insert into usedlist values ('NULL', null, '%s', '%s', '%s', '%s', '%s')", data[2], data[4], myID, data[0], data[1]);
    mysql_query(connection, query);
    puts(query);

    call_add(clntSock);

    memset(query, 0, sizeof(0));
    sprintf(query, "update customer set CallCount = CallCount + 1 where CustomerID = '%s'", myID);
    puts(query);
    mysql_query(connection, query);
}

void ReadThread::HistoryCustomer(int clntSock, char myID[])
{
    char buf[300] = {0};
    char query[200] = {0};

    sprintf(query, "select Time, `From`, `To`, Price from usedlist where CustomerID = '%s' ORDER by usedlist.`Time` asc limit 3", myID);
    mysql_query(connection, query);

    sql_result = mysql_store_result(connection);
    int cnt = mysql_num_rows(sql_result);
    sprintf(buf, "%d", cnt);
    usleep(10000);
    write(clntSock, buf, strlen(buf));

    usleep(20000);

    while (sql_row = mysql_fetch_row(sql_result))
    {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s@%s@%s@%s", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);
        write(clntSock, buf, strlen(buf));
        usleep(50000);
    }

    memset(query, 0, sizeof(query));
    sprintf(query, "select CallCount, PaymentCount, TotalPrice from customer where CustomerID = '%s'", myID);
    mysql_query(connection, query);

    sql_result = mysql_store_result(connection);
    while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
    {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s@%s@%s 원", sql_row[0], sql_row[1], sql_row[2]);

        write(clntSock, buf, strlen(buf));
        puts(buf);
        usleep(10000);
    }
}

void ReadThread::CallFailedCustomer(int clntSock, char myID[])
{
    char query[300] = {0};
    char flag[15] = "DeleteCallList";
    char CustomerID[20] = {0};
    strcpy(CustomerID, myID);

    map<int, string>::iterator iter;
    for (iter = DriverList.begin(); iter != DriverList.end(); iter++)
    {
        write(iter->first, flag, strlen(flag));
        usleep(10000);
        write(iter->first, myID, strlen(CustomerID));
    }

    sprintf(query, "delete from usedlist where CustomerID = '%s' and DriverID = 'NULL'", myID);
    mysql_query(connection, query);

    memset(query, 0, sizeof(query));
    sprintf(query, "update calllist set Status = 'cancel' where CustomerID = '%s' and Status = 'wait'", myID);
    mysql_query(connection, query);
}
void ReadThread::TakeTaxiCustomer(int clntSock, char myID[])
{
    char buf[300] = "\0";
    char query[350] = "\0";
    char DriverID[30] = "\0";

    read(clntSock, buf, sizeof(buf));
    strcpy(DriverID, buf);

    sprintf(query, "update usedlist set DriverID = '%s' where CustomerID = '%s' and DriverID = 'NULL'", DriverID, myID);
    mysql_query(connection, query);

    memset(query, 0, sizeof(query));
    sprintf(query, "update calllist set DriverID = '%s', Status = 'accept' where CustomerID = '%s' and Status = 'wait'", DriverID, myID);
    puts(query);
    mysql_query(connection, query);

    memset(query, 0, sizeof(query));
    sprintf(query, "select cl.TakingTime, cl.Distance, cl.Charge, dr.Name, dr.VehicleNumber from driver as dr join calllist as cl on cl.DriverID = dr.DriverID where cl.Status = 'accept' and cl.CustomerID = '%s';", myID);
    puts(query);
    mysql_query(connection, query);

    sql_result = mysql_store_result(connection);

    // sql_row = mysql_fetch_row(sql_result);
    // sprintf(buf, "%s@%s@%s@%s@%s", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4]);
    // write(clntSock, buf, strlen(buf));
    // puts(buf);

    while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
    {

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s@%s@%s@%s@%s", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4]);
        write(clntSock, buf, strlen(buf));
        puts(buf);
    }
    mysql_free_result(sql_result);
}
void ReadThread::GetOnCustomer(int clntSock)
{
    char buf[200] = {0};
    char query[200] = {0};

    sprintf(query, "select cl.Departure, cl.Destination, dr.VehicleNumber, cl.TakingTime from driver as dr join calllist as cl on cl.DriverID = dr.DriverID where cl.Status = 'accept'");
    mysql_query(connection, query);
    sql_result = mysql_store_result(connection);
    while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
    {
        sprintf(buf, "%s@%s@%s@%s", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);
        write(clntSock, buf, strlen(buf));
        puts(buf);
    }
}
void ReadThread::GetOffCustomer(int clntSock, char myID[])
{
    char buf[20] = {0};
    char query[200] = {0};
    char driverID[20] = {0};

    sprintf(query, "select DriverID from usedlist where CustomerID = '%s' and Time is null", myID);
    mysql_query(connection, query);
    sql_result = mysql_store_result(connection);
    while (sql_row = mysql_fetch_row(sql_result))
        strcpy(driverID, sql_row[0]);

    map<int, string>::iterator iter;
    for (iter = DriverList.begin(); iter != DriverList.end(); iter++)
    {
        if (iter->second == driverID)
        {
            strcpy(buf, "paid");
            write(iter->first, buf, strlen(buf));
        }
    }
    usleep(1000);

    memset(query, 0, sizeof(query));
    sprintf(query, "update customer set PaymentCount = PaymentCount + 1, TotalPrice = TotalPrice + (select Charge from calllist where CustomerID = '%s' and Status = 'accept')", myID);
    mysql_query(connection, query);

    usleep(1000);

    memset(query, 0, sizeof(query));
    sprintf(query, "update usedlist set Time = now() where CustomerID = '%s' and Time is null", myID);
    int result;
    if((result = mysql_query(connection, query)) !=0)
        cout<< "오류" << query<<endl;
    usleep(1000);

    memset(query, 0, sizeof(query));
    sprintf(query, "update calllist set Status = 'paid' where CustomerID = '%s' and Status = 'accept'", myID);
    if((result = mysql_query(connection, query)) !=0)
        cout<< "오류" << query<<endl;
}
void ReadThread::ReviewCustomer(int clntSock, char myID[])
{
    char buf[200] = {0};
    char query[300] = {0};

    read(clntSock, buf, sizeof(buf));
    puts(buf);
    sprintf(query, "update calllist set Review = '%s' where CustomerID = '%s' and Status = 'paid' and Review = 'NULL'", buf, myID);
    mysql_query(connection, query);
}

void ReadThread::ReviewData(int MyClntsock)
{
    char id[30] = "\0";
    char query[100] = "\0";
    read(MyClntsock, id, sizeof(id));

    cout<<" 리뷰 아이디 : "<<id<<endl;

    sprintf(query,"select Review from calllist WHERE DriverID = '%s'",id);
    mysql_query(connection, query);
    sql_result = mysql_store_result(connection);
    cout<<"query : "<<query<<endl;

    string review="@";
    while((sql_row = mysql_fetch_row(sql_result))!=NULL){
        review +=  sql_row[0];
        review += "@";
    }
    cout<<"review " << review<<endl;
    mysql_free_result(sql_result);

    write(MyClntsock, review.c_str(), strlen(review.c_str()));
}