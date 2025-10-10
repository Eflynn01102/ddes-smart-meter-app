cmake_minimum_required(VERSION 3.15)
project(ingestion C)

# Add executable
add_executable(ingestion
    src/ingestion.c
    src/ingestion.h
    src/types.h
    src/read-config.c
    src/hmac-verify.c
    src/env-vars.c
    src/json-obj-validation.c
    src/rabbitmq-comms.c
    src/logging.c
    src/idempotency.c
)

find_package(OpenSSL REQUIRED)
find_path(RABBITMQ_INCLUDE_DIR
    NAMES amqp.h amqp_tcp_socket.h
    PATHS /usr/include /usr/include/rabbitmq-c /usr/local/include /usr/local/include/rabbitmq-c
)
find_library(RABBITMQ_LIBRARY
    NAMES rabbitmq
    PATHS /usr/lib /usr/lib/x86_64-linux-gnu /usr/local/lib
)

target_include_directories(ingestion PRIVATE ${RABBITMQ_INCLUDE_DIR})

target_link_libraries(ingestion PRIVATE ${RABBITMQ_LIBRARY} OpenSSL::Crypto cjson)

file(COPY ${CMAKE_SOURCE_DIR}/src/ingestion.conf DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/src/ingestion.env DESTINATION ${CMAKE_BINARY_DIR})