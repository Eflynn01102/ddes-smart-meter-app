cmake_minimum_required(VERSION 3.15)
project(ingestion C)

include(FetchContent)

FetchContent_Declare(
    rabbitmq-c
    GIT_REPOSITORY https://github.com/alanxz/rabbitmq-c.git
    GIT_TAG master
    CMAKE_ARGS -DENABLE_SSL_SUPPORT=OFF
)

FetchContent_Declare(
  cjson
  GIT_REPOSITORY https://github.com/DaveGamble/cJSON.git
  GIT_TAG master
)

FetchContent_MakeAvailable(rabbitmq-c)
FetchContent_MakeAvailable(cjson)

find_package(OpenSSL REQUIRED)
add_executable(ingestion src/ingestion.c src/ingestion.h src/types.h src/read-config.c src/hmac-verify.c src/env-vars.c src/json-obj-validation.c src/rabbitmq-comms.c src/logging.c src/idempotency.c)
target_include_directories(ingestion
    PRIVATE
        ${cjson_SOURCE_DIR} 
)
target_link_libraries(ingestion PRIVATE rabbitmq::rabbitmq)
target_link_libraries(ingestion PRIVATE OpenSSL::Crypto)
target_link_libraries(ingestion PRIVATE cjson)
file(COPY ${CMAKE_SOURCE_DIR}/src/ingestion.conf DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/src/ingestion.env DESTINATION ${CMAKE_BINARY_DIR})