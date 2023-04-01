cmake_minimum_required(VERSION 3.18)

set(PROJECT_NAME physicsEngine)

project(${PROJECT_NAME})

add_definitions(/std:c++17)

find_package( OpenGL REQUIRED )

add_subdirectory(math)
add_subdirectory(glfw)
add_subdirectory(source)
