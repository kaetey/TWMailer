cmake_minimum_required(VERSION 3.20)
project(TWMailer)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(TWMailer
        Client/Client.cpp
        Server/Server.cpp
        Message.cpp
        Message.h
        Server.cpp
        TWMailer.cpp
        User.cpp
        User.h)
