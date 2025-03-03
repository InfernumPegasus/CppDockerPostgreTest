cmake_minimum_required(VERSION 3.14)
project(DockerDbApp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID MATCHES GNU)
    add_compile_options(-W)
    add_compile_options(-Wall)
    add_compile_options(-Wextra)
    add_compile_options(-Wpedantic)
    add_compile_options(-Werror)

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-O0)
        add_compile_options(-g3)
        add_compile_options(-fsanitize=address)
        add_link_options(-fsanitize=address)
        add_compile_options(-fsanitize=undefined)
        add_link_options(-fsanitize=undefined)
    endif ()

    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(-O2)
        add_compile_options(-g3)
    endif ()
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES MSVC)
    add_compile_options(/W3)
    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(/0t)
    else ()
        add_compile_options(/Zi)
    endif ()
endif ()

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(PostgresTestApp postgresql-test/PostgresTestApp.cpp)

target_include_directories(PostgresTestApp PRIVATE /usr/include/pqxx)
target_link_directories(PostgresTestApp PRIVATE /usr/lib)
target_link_libraries(PostgresTestApp PRIVATE pqxx pq)

add_executable(KafkaConsumerApp kafka-test/KafkaConsumerApp.cpp)
add_executable(KafkaProducerApp kafka-test/KafkaProducerApp.cpp)

target_link_libraries(KafkaConsumerApp PRIVATE rdkafka pqxx pq)
target_link_libraries(KafkaProducerApp PRIVATE rdkafka pqxx pq)