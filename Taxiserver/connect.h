#ifndef __CONNECT_H_
#define __CONNECT_H_
#include "/usr/include/mysql/mysql.h"

class Connect
{
protected:
    MYSQL *connection = NULL, conn;

public:
    Connect()
    {
        mysql_init(&conn);
        connection = mysql_real_connect(&conn, "127.0.0.1", "root", "1234", "TAXI", 3306, (char *)NULL, 0);
    }
};
#endif