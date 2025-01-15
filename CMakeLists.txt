cmake_minimum_required(VERSION 3.14)
project(DockerDbApp)

set(CMAKE_CXX_STANDARD 23)

add_executable(DockerDbApp main.cpp)

target_include_directories(DockerDbApp PRIVATE /usr/include/pqxx)
target_link_directories(DockerDbApp PRIVATE /usr/lib)
target_link_libraries(DockerDbApp PRIVATE pqxx pq)