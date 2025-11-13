#include "tests.h"

V test_RabbitMQComms(V) {
    U8 Result = 0;
    U8 IP[32] = "scmedia.ddns.net";
    S32 Port = 5672;
    U8 Username[32] = "ddes";
    U8 Password[32] = "ddeszxcvb";
    AMQP_CONN_T Connection = {0};

    //happy scenario
    Result |= (InitiateConnection(&Connection, IP, Port, Username, Password) << 0);
    CloseConnection(&Connection);

    //incorrect host
    sprintf(IP, "www.example.com");
    Result |= (InitiateConnection(&Connection, IP, Port, Username, Password) << 1);
    CloseConnection(&Connection);

    //incorrect username
    sprintf(IP, "scmedia.ddns.net");
    sprintf(Username, "e");
    Result |= (InitiateConnection(&Connection, IP, Port, Username, Password) << 2);
    CloseConnection(&Connection);

    //incorrect password
    sprintf(Username, "ddes");
    sprintf(Password, "e");
    Result |= (InitiateConnection(&Connection, IP, Port, Username, Password) << 3);
    CloseConnection(&Connection);

    TEST_ASSERT_EQUAL_UINT8(0b1110, Result);
}