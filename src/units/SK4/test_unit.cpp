#include "test_unit.hpp"

// Project namespace
namespace hse {
/* Unit initialization override function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void testUnit::init() {
    for (int i = 0; i < 400; i++) {
        float x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) *
                  100,
              y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) *
                  100,
              z = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) *
                  100;
        primitive *tmp = createSpherePrimitive(1, math::vec3(x - 50, y - 50, z - 50), 50, 50);
        tmp->addUniform(&render::renderInstance.getTime(), "time");
        math::matr4 a;
        math::vec3 color = math::vec3(x / 100, y / 100, z / 100);
        tmp->addUniform(color, "sphereColor");
    }

    // One of the tests
    ::math::matr4 M(1, 2, 3, 4, 4, 3, 2, 1, 2, 3, 4, 1, 4, 1, 2, 3);
    M.inverse();
    M.print();
}  // End of 'testUnit::initUnit' function

/* Unit response override function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void testUnit::response() {
    math::vec3 newCameraLocation = math::vec3(
        (cos(render::renderInstance.getTime() / 2) + 1) * 50, 50,
        (sin(render::renderInstance.getTime() / 2) + 1) * 50
    );
    mainCamera.setView(newCameraLocation, math::vec3(0) - newCameraLocation);
    if (render::renderInstance.keys[GLFW_KEY_W].action == GLFW_PRESS)
        ::std::cout << 'w' << ::std::endl;
    if (render::renderInstance.keys[GLFW_KEY_A].action == GLFW_PRESS)
        ::std::cout << 'a' << ::std::endl;
    if (render::renderInstance.keys[GLFW_KEY_S].action == GLFW_PRESS)
        ::std::cout << 's' << ::std::endl;
    if (render::renderInstance.keys[GLFW_KEY_D].action == GLFW_PRESS)
        ::std::cout << 'd' << ::std::endl;
}  // End of 'testUnit::responseUnit' function

// Class override destructor
testUnit::~testUnit() {
    ::std::cout << "Clear test unit" << ::std::endl;
}  // End of 'testUnit::~testUnit' function
}  // namespace hse
