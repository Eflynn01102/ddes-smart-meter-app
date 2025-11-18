#include "tests.h"

V test_RabbitMQComms(V) {
    U8 Result = 0;
    ConfigType Conf = {"scmedia.ddns.net", 5672, "ddes", "ddeszxcvb"};
    AMQP_CONN_T Connection = {0};

    //happy scenario
    Result |= (InitiateConnection(&Connection, Conf) << 0);
    CloseConnection(&Connection);

    //incorrect host
    sprintf(Conf.RabbitHost, "www.example.com");
    Result |= (InitiateConnection(&Connection, Conf) << 1);
    CloseConnection(&Connection);

    //incorrect username
    sprintf(Conf.RabbitHost, "scmedia.ddns.net");
    sprintf(Conf.RabbitUsername, "e");
    Result |= (InitiateConnection(&Connection, Conf) << 2);
    CloseConnection(&Connection);

    //incorrect password
    sprintf(Conf.RabbitUsername, "ddes");
    sprintf(Conf.RabbitPassword, "e");
    Result |= (InitiateConnection(&Connection, Conf) << 3);
    CloseConnection(&Connection);

    TEST_ASSERT_EQUAL_UINT8(0b1110, Result);
}