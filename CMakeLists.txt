cmake_minimum_required(VERSION 3.22)
set(PROJECT_NAME HSE_project)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

file(GLOB SOURCE_FILES
        src/main.cpp
        src/render/render.cpp
        src/render/src/scenes/scene.cpp
        src/render/src/primitives/primitive.cpp
        src/render/src/primitives/vertex_array.cpp
        src/render/src/shaders/shader.cpp
        src/units/SK4/test_unit.cpp
        src/units/DV1/rm_shd_unit.cpp)
file(GLOB HEADER_FILES
        src/def.hpp
        src/render/render.hpp
        src/render/src/scenes/scene.hpp
        src/render/src/primitives/primitive.hpp
        src/render/src/primitives/vertex_array.hpp
        src/render/src/shaders/shader.hpp
        src/units/SK4/test_unit.hpp
        src/units/DV1/rm_shd_unit.hpp)

find_package(OpenGL REQUIRED)

if (APPLE)
    message(STATUS ">>> Apple detected")
    find_package(GLEW REQUIRED)
    link_directories(bin/GLFW/macos)
    set(GLFW_LIBRARIES libglfw.3.dylib libglfw3.a)
    set(GLEW_LIBRARIES GLEW::GLEW)

elseif (MSVC OR MSYS OR MINGW)
    message(STATUS ">>> Windows detected")
    link_directories(bin/GLFW/windows)
    set(GLFW_LIBRARIES libglfw3.a)
    link_directories(bin/GLEW/windows)
    set(GLEW_LIBRARIES glew32s glew32 opengl32)

elseif (UNIX AND NOT APPLE)
    message(STATUS ">>> Linux detected")
    set(GLFW_LIBRARIES glfw GL glut GLU)
    set(GLEW_LIBRARIES GLEW)

else ()
    message(FATAL_ERROR "Not support this system (use MacOS/Linux/Windows)")
endif ()


# not needed for unix-system (for windows it's 'must have')
add_compile_definitions(GLEW_STATIC)

# for detecting memory leaks
#add_compile_options(-fsanitize=address)
#add_compile_options(-fsanitize=undefined)
#add_link_options(-fsanitize=address)
#add_link_options(-fsanitize=undefined)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARIES} ${GLEW_LIBRARIES})
