cmake_minimum_required(VERSION 3.22)
set(PROJECT_NAME HSE_project)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

file(GLOB SOURCE_FILES src/*.cpp)
file(GLOB HEADER_FILES src/*.hpp)

find_package(OpenGL REQUIRED)

# maybe all unix-systems need this way linking libraries
if(APPLE)
    message(STATUS ">>> Apple detected")
    # brew is needed for this command
    execute_process(COMMAND brew install glew)
    # maybe all unix-systems need this way linking glew
    find_package(GLEW REQUIRED)
    link_directories(lib/macos)
    set(GLFW_LIBRARIES libglfw.3.dylib libglfw3.a)
    set(GLEW_LIBRARIES GLEW::GLEW)
endif()

if(MSVC OR MSYS OR MINGW)
    message(STATUS ">>> Windows detected")
    link_directories(lib/windows)
    set(GLFW_LIBRARIES libglfw3.a)
    set(GLEW_LIBRARIES glew32s glew32 opengl32)
endif()

if(UNIX AND NOT APPLE)
    message(STATUS ">>> Linux detected")
    set(GLFW_LIBRARIES glfw GL glut GLU)
    set(GLEW_LIBRARIES GLEW)
endif()

# not needed for unix-system (for windows it's 'must have')
add_compile_definitions(GLEW_STATIC)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARIES} ${GLEW_LIBRARIES})
