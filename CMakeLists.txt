cmake_minimum_required(VERSION 3.14)
project(DockerDbApp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(KafkaConsumerApp kafka-test/KafkaConsumerApp.cpp)
add_executable(KafkaProducerApp kafka-test/KafkaProducerApp.cpp)

target_link_libraries(KafkaConsumerApp PRIVATE rdkafka)
target_link_libraries(KafkaProducerApp PRIVATE rdkafka)