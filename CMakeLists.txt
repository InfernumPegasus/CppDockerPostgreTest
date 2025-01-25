cmake_minimum_required(VERSION 3.14)
project(DockerDbApp)

set(CMAKE_CXX_STANDARD 23)

add_executable(PostgresTestApp postgresql-test/PostgresTestApp.cpp)

target_include_directories(PostgresTestApp PRIVATE /usr/include/pqxx)
target_link_directories(PostgresTestApp PRIVATE /usr/lib)
target_link_libraries(PostgresTestApp PRIVATE pqxx pq)