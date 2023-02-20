#include "def.hpp"

#include "render/render.hpp"
#include "units/SK4/test_unit.hpp"

// Main program function
    int main() {
    // Glfw initialization and its version setting
    {
        if (!glfwInit()) assert("Error in glfw initialization");
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    }

    hse::render::getRenderInstance().init(600, 600);

    auto mainScene = ::std::make_unique<hse::scene>();
    mainScene->addUnit("Test unit", new hse::testUnit());

    hse::render::getRenderInstance().addScene("Main scene", mainScene);
    hse::render::getRenderInstance().response();

    //    int *a = new int[100];
    //    a[99] = 1;

    return 0;
}  // End of 'main' function