cmake_minimum_required(VERSION 3.14)
project(DockerDbApp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(PostgresTestApp postgresql-test/PostgresTestApp.cpp)

target_include_directories(PostgresTestApp PRIVATE /usr/include/pqxx)
target_link_directories(PostgresTestApp PRIVATE /usr/lib)
target_link_libraries(PostgresTestApp PRIVATE pqxx pq)

add_executable(KafkaConsumerApp kafka-test/KafkaConsumerApp.cpp)
add_executable(KafkaProducerApp kafka-test/KafkaProducerApp.cpp)

target_link_libraries(KafkaConsumerApp PRIVATE rdkafka pqxx pq)
target_link_libraries(KafkaProducerApp PRIVATE rdkafka pqxx pq)