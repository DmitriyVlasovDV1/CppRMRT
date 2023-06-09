cmake_minimum_required(VERSION 3.22)
set(PROJECT_NAME HSE_project)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

file(GLOB SOURCE_FILES
        src/main.cpp
        src/render/render.cpp
        src/render/src/resources/buffers/buffer.cpp
        src/render/src/resources/primitives/primitive.cpp
        src/render/src/resources/models/model.cpp
        src/render/src/resources/shaders/shader.cpp
        src/render/src/resources/scenes/scene.cpp
        src/render/src/figures/figure_scene.cpp
        src/render/src/figures/figure.cpp
        src/render/src/figures/figure_render.cpp
        src/render/src/figures/figure_transformation.cpp
        src/render/src/figures/figure_material.cpp
        src/scenes/SK4/test_unit.cpp
        src/scenes/DV1/rm_shd_unit.cpp)

file(GLOB HEADER_FILES
        src/utilities/math/math.hpp
        src/utilities/math/math_def.hpp
        src/utilities/math/vectors/vec3.hpp
        src/utilities/math/vectors/vec2.hpp
        src/utilities/math/matrices/matr4.hpp
        src/utilities/camera/camera.hpp
        src/def.hpp
        src/render/render.hpp
        src/render/src/resources/buffers/buffer.hpp
        src/render/src/resources/primitives/primitive.hpp
        src/render/src/resources/models/model.hpp
        src/render/src/resources/shaders/shader.hpp
        src/render/src/resources/scenes/scene.hpp
        src/render/src/figures/figure_scene.hpp
        src/render/src/figures/figure.hpp
        src/render/src/figures/figure_render.hpp
        src/render/src/figures/figure_transformation.hpp
        src/render/src/figures/figure_material.hpp
        src/scenes/SK4/test_unit.hpp
        src/scenes/DV1/rm_shd_unit.hpp)

find_package(OpenGL REQUIRED)

if (APPLE)
    message(STATUS ">>> Apple detected")
    find_package(GLEW REQUIRED)
    add_subdirectory(bin/glfw)
    set(GLFW_LIBRARIES glfw)
    set(GLEW_LIBRARIES GLEW::GLEW)

elseif (MSVC OR MSYS OR MINGW)
    message(STATUS ">>> Windows detected")
    add_subdirectory(bin/glfw)
    set(GLFW_LIBRARIES glfw)
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

add_executable(${PROJECT_NAME} ${SOURCE_FILES} )


# Build tests
add_executable(matrices-test tests/doctest_main.cpp tests/math/matrices_test.cpp)
add_executable(vectors-test tests/doctest_main.cpp tests/math/vectors_test.cpp)

target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARIES} ${GLEW_LIBRARIES})
