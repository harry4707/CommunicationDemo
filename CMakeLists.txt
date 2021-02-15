cmake_minimum_required(VERSION 3.17)
project(CommunicationDemo)

set(CMAKE_CXX_STANDARD 14)

add_executable(CommunicationDemo_client client.c)
add_executable(CommunicationDemo_server server.c)