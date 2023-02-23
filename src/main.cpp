#include "def.hpp"
#include "render/render.hpp"
#include "units/SK4/test_unit.hpp"
#include "units/DV1/rm_shd_unit.hpp"

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

    hse::render::renderInstance.init(600, 600);

    auto mainScene = ::std::make_unique<hse::scene>();
    //mainScene->addUnit("Rm shd unit", new hse::rmShdUnit());

    hse::render::renderInstance.addScene("Main scene", mainScene);
    hse::render::renderInstance.response();

    //    int *a = new int[100];
    //    a[99] = 1;

    return 0;
}  // End of 'main' function