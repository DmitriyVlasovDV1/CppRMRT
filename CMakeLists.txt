cmake_minimum_required(VERSION 3.23)
set(PROJECT_NAME HSE_project)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    execute_process(COMMAND pip install conan)
    execute_process(COMMAND conan remove --locks)
    execute_process(COMMAND conan install --build=missing ..)
endif()

set(CONAN_DISABLE_CHECK_COMPILER 1)

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

file(GLOB SOURCE_FILES src/*.cpp)
file(GLOB HEADER_FILES src/*.hpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} GLEW::GLEW glfw::glfw)
