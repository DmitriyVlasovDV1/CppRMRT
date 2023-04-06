#include "def.hpp"
#include "render/render.hpp"
#include "units/DV1/rm_shd_unit.hpp"
#include "units/SK4/test_unit.hpp"

float hse::time;                      // Global time variable
float hse::deltaTime;                 // Global delta time variable
bool hse::isPause;                    // Pause flag
unsigned int hse::windowWidth,        // Window width
    hse::windowHeight;                // Window height
::std::map<int, hse::Key> hse::keys;  // Keys map for input response

// Function for generate render instance variable
void hse::factory() {
    // Glfw initialization and its version setting
    {
        if (!glfwInit()) EXCEPTION("Error in glfw initialization");
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    }

    hse::Render renderInstance;

    renderInstance.onCreate(800, 600);
    renderInstance.addUnit("Test unit", new hse::TestUnit());
    renderInstance.onUpdate();
}  // End of 'hse::factory' function

// Main program function
int main() {
    hse::factory();
    return 0;
}  // End of 'main' function
