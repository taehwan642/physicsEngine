cmake_minimum_required(VERSION 3.18)

set(PROJECT_NAME source)

project(${PROJECT_NAME})

file(GLOB_RECURSE ENGINE_SRC_FILES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/Engine/*.cpp
)

file(GLOB_RECURSE CLIENT_SRC_FILES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/Client/*.cpp
)

add_executable (${PROJECT_NAME} main.cpp ${ENGINE_SRC_FILES} ${CLIENT_SRC_FILES})

target_include_directories( ${PROJECT_NAME} PUBLIC ${GLFW_INCLUDE_DIRS} )
target_include_directories( ${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIR} )
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Engine)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Client)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/glad)
target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
target_link_libraries(${PROJECT_NAME} "-framework IOKit")
target_link_libraries(${PROJECT_NAME} math glfw  ${OPENGL_gl_LIBRARY})

