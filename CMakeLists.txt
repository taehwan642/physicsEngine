cmake_minimum_required(VERSION 3.18)

set(PROJECT_NAME physicsEngine)

project(${PROJECT_NAME})

if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif()
if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

set(CXX_FLAGS "-Wall" "-Werror" "-Wextra" "-fsanitize=undefined,address")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")
set(CMAKE_BUILD_TYPE Debug)

find_package( OpenGL REQUIRED )

add_subdirectory(math)
add_subdirectory(dependency/glfw)
add_subdirectory(source)
