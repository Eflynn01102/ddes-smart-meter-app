#include "tests.h"

V test_ReadEnvVars(V) {
    TEST_ASSERT_EQUAL_CHAR_ARRAY(getenv("HMACKEY"), "00deb39aaf33c17977e0c858148dca5483703f554da146637b35914772cae5b2", 64);
}