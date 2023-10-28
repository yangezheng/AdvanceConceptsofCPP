cmake_minimum_required(VERSION 3.14)
project(exercise_hw02)

# require C++20
set(CMAKE_CXX_STANDARD 20)

set(EXECUTABLE_NAME runhw02)

add_library(hw02 combinatorics.cpp)

add_executable(${EXECUTABLE_NAME} run.cpp)

target_link_directories(${EXECUTABLE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${EXECUTABLE_NAME} hw02)