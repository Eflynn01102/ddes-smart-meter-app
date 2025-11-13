#include "tests.h"

V test_ReadRabbitConfig(V) {
    U8 Result = 0;
    U8 IP[32] = "rabbitmq";
    S32 Port = 5672;
    U8 Username[32] = "guest";
    U8 Password[32] = "guest";

    U8 StoredIP[32] = {0};
    S32 StoredPort = {0};
    U8 StoredUsername[32] = {0};
    U8 StoredPassword[32] = {0};

    ReadRabbitConfig(StoredIP, &StoredPort, StoredUsername, StoredPassword);
    Result = (memcmp(IP, StoredIP, 32) << 0);
    Result = ((Port == StoredPort) << 1);
    Result = (memcmp(Username, StoredUsername, 32) << 2);
    Result = (memcmp(Password, StoredPassword, 32) << 3);

    TEST_ASSERT_EQUAL_UINT8(0b0000, Result);
}