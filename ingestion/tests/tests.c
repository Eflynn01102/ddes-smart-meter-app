#include "tests.h"

V setUp(V) {
    ReadEnvVars();
    return ;
}

V tearDown(V) {
    return ;
}

V main(V) {
    UNITY_BEGIN();
    RUN_TEST(test_ReadEnvVars);
}