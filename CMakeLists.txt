cmake_minimum_required(VERSION 3.16)

project(opengl CXX)
include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)   # 生成 compile_commands.json
set(CMAKE_C_COMPILE_OBJECT ON)

set(SOURCE_FILES 
  src/main.cpp
  src/Render.cpp
  src/Shader.cpp
  src/VertexBuffer.cpp
  src/IndexBuffer.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

FetchContent_Declare(
    glfw
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw
)

FetchContent_MakeAvailable(glfw)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw) 
target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW) 
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL) 

if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

