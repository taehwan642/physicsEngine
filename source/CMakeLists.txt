cmake_minimum_required(VERSION 3.18)

set(PROJECT_NAME source)

project(${PROJECT_NAME})

add_executable (${PROJECT_NAME} test.cpp)

target_link_libraries(${PROJECT_NAME} math glfw)