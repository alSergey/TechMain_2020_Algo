CMAKE_MINIMUM_REQUIRED(VERSION 3.14.3)
PROJECT(module3)

SET(CMAKE_CXX_STANDARD 14)
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wpedantic")

ADD_EXECUTABLE(m1 main1.cpp)
ADD_EXECUTABLE(m2 main2.cpp)
ADD_EXECUTABLE(m3 main3.cpp)