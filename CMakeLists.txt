cmake_minimum_required(VERSION 3.18)

set(PROJECT_NAME physicsEngine)

project(${PROJECT_NAME})

find_package( OpenGL REQUIRED )

include_directories( ${OPENGL_INCLUDE_DIRS} )

add_subdirectory(math)
add_subdirectory(glfw)
add_subdirectory(source)
