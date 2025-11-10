include(FetchContent)
FetchContent_Declare(
  unity
  GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
  GIT_TAG master
)

FetchContent_MakeAvailable(unity)
add_executable(tests
  env-vars.c
  hmac-verify.c
  idempotency.c
  json-obj-validation.c
  rabbitmq-comms.c
  read-config.c
  tests.c
  tests.h

  ${unity_SOURCE_DIR}/src/unity.c
)

# Include directories for Unity and your source code
target_include_directories(tests PRIVATE
 ${unity_SOURCE_DIR}/src
 ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(tests PRIVATE ingestion_lib)

# Register the test with CTest
add_test(NAME run_unit_tests COMMAND tests)