cmake_minimum_required(VERSION 3.15)
project(ingestion C)

include(FetchContent)

FetchContent_Declare(
    rabbitmq-c
    GIT_REPOSITORY https://github.com/alanxz/rabbitmq-c.git
    GIT_TAG master
    CMAKE_ARGS -DENABLE_SSL_SUPPORT=OFF
)

FetchContent_MakeAvailable(rabbitmq-c)
add_executable(ingestion src/ingestion.c)
target_link_libraries(ingestion PRIVATE rabbitmq::rabbitmq)