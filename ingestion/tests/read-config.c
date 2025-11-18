#include "tests.h"

V test_ReadConfig(V) {
    U8 Result = 0;
    ConfigType ExpectedConfig = {"rabbitmq", 5672, "guest", "guest", "127.0.0.1", 3000};
    ConfigType ActualConfig = {0};
    ReadConfig(&ActualConfig);
    
    TEST_ASSERT_EQUAL_MEMORY(&ExpectedConfig, &ActualConfig, sizeof(ConfigType));
}