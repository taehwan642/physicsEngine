cmake_minimum_required(VERSION 3.18)

set(PROJECT_NAME source)

project(${PROJECT_NAME})

file(GLOB_RECURSE ENGINE_SRC_FILES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/Engine/*.cpp
)

file(GLOB_RECURSE CLIENT_SRC_FILES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/Client/*.cpp
)

file(GLOB_RECURSE IMGUI_FILES CONFIGURE_DEPENDS
  ${CMAKE_SOURCE_DIR}/dependency/imgui*.cpp
)

add_executable (${PROJECT_NAME} main.cpp ${ENGINE_SRC_FILES} ${CLIENT_SRC_FILES} ${IMGUI_FILES} ${CMAKE_SOURCE_DIR}/dependency/glad/src/glad.c)

# move shader file to build directory
# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD this only works at first build
# but this always going to happen
add_custom_target(copy_shader ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/Shader
        ${PROJECT_BINARY_DIR}/Shader
        COMMENT "Copy Shader")
add_dependencies(${PROJECT_NAME} copy_shader)

target_include_directories( ${PROJECT_NAME} PUBLIC ${GLFW_INCLUDE_DIRS} )
target_include_directories( ${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIR} )
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Engine)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Client)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/dependency/glad/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/dependency/imgui)
if(APPLE)
target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
target_link_libraries(${PROJECT_NAME} "-framework IOKit")
endif()
target_link_libraries(${PROJECT_NAME} math glfw ${OPENGL_gl_LIBRARY})
