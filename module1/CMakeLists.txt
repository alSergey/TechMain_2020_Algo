cmake_minimum_required(VERSION 3.17)
project(module1)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wpedantic")

add_executable(m1_2 main1_2.cpp)
add_executable(m2_2 main2_2.cpp)
add_executable(m3_1 main3_1.cpp)
add_executable(m4_1 main4_1.cpp)
add_executable(m5_3 main5_3.cpp)
add_executable(m6_4 main6_4.cpp)
add_executable(m7_1 main7_1.cpp)