#include "test_unit.hpp"

// Project namespace
namespace hse {
/* Sphere generation function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void testUnit::spheresGeneration() {
    const size_t spheresNumber = 1000;
    for (int i = 0; i < spheresNumber; i++) {
        float x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) *
                  100,
              y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) *
                  100,
              z = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) *
                  100;
        primitive *tmp = createSpherePrimitive(
            1, math::vec3(x - 50, y - 50, z - 50), 20, 20
        );
        tmp->addUniform(render::renderInstance.getTime(), "time");
        math::vec3 color = math::vec3(x / 100, y / 100, z / 100);
        tmp->addUniform(color, "vertexColor");
    }
}  // End of 'testUnit::spheresGeneration' function

/* Cubes generation function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void testUnit::cubesGeneration() {
    const size_t cubesNumber = 1000;
    for (int i = 0; i < cubesNumber; i++) {
        float x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) *
                  100,
              y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) *
                  100,
              z = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) *
                  100;
        primitive *tmp =
            createCubePrimitive(1, 1, 1, math::vec3(x - 50, y - 50, z - 50));
        tmp->transformMatrix = math::matr4::rotateX(z) *
                               math::matr4::rotateY(x) *
                               math::matr4::rotateZ(y) * tmp->transformMatrix;
        tmp->addUniform(render::renderInstance.getTime(), "time");
        math::vec3 color = math::vec3(x / 100, y / 100, z / 100);
        tmp->addUniform(color, "vertexColor");
    }
}  // End of 'testUnit::cubesGeneration' function

/* Camera response function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void testUnit::cameraResponse() {
    math::vec3 newCameraLocation = math::vec3(
        (cos(render::renderInstance.getTime() / 1.5f) + 1) * 50, 50,
        (sin(render::renderInstance.getTime() / 1.5f) + 1) * 50
    );
    mainCamera.setPositionWithDirection(
        newCameraLocation, math::vec3(0) - newCameraLocation
    );
}  // End of 'testUnit::cameraResponse' function

/* Input (keyboard/mouse) response function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void testUnit::inputResponse() {
    if (render::renderInstance.keys[GLFW_KEY_W].action == GLFW_PRESS)
        ::std::cout << 'w' << ::std::endl;
    if (render::renderInstance.keys[GLFW_KEY_A].action == GLFW_PRESS)
        ::std::cout << 'a' << ::std::endl;
    if (render::renderInstance.keys[GLFW_KEY_S].action == GLFW_PRESS)
        ::std::cout << 's' << ::std::endl;
    if (render::renderInstance.keys[GLFW_KEY_D].action == GLFW_PRESS)
        ::std::cout << 'd' << ::std::endl;

    // Pause functional
    static int oldAction;
    if (render::renderInstance.keys[GLFW_KEY_SPACE].action == GLFW_PRESS &&
        oldAction == GLFW_RELEASE) {
        render::renderInstance.setPauseFlag(
            !render::renderInstance.getPauseFlag()
        );
        oldAction = GLFW_PRESS;
    } else if (render::renderInstance.keys[GLFW_KEY_SPACE].action == GLFW_RELEASE)
        oldAction = GLFW_RELEASE;
}  // End of 'testUnit::inputResponse' function

/* Unit initialization override function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void testUnit::init() {
    unitModel = createModel("obj_model", "tea_cup/tea_cup.obj");
    unitModel->addUniform(math::vec3(0.3, 0.5, 0.7), "vertexColor");
    unitModel->transformMatrix = math::matr4::translate(math::vec3(0, -1, 0)) *
                                 math::matr4::scale(math::vec3(10));
    spheresGeneration();
    cubesGeneration();
}  // End of 'testUnit::initUnit' function

/* Unit response override function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void testUnit::response() {
    unitModel->transformMatrix =
        math::matr4::translate(math::vec3(0, -1, 0)) *
        math::matr4::scale(math::vec3(10)) *
        math::matr4::rotateY(render::renderInstance.getTime() * 50) *
        math::matr4::rotateX(-render::renderInstance.getTime() * 30) *
        math::matr4::rotateZ(render::renderInstance.getTime() * 20);
    cameraResponse();
    inputResponse();
}  // End of 'testUnit::responseUnit' function

// Class override destructor
testUnit::~testUnit() {
    ::std::cout << "Clear test unit" << ::std::endl;
}  // End of 'testUnit::~testUnit' function

// End of 'testUnit::~testUnit' function
}  // namespace hse
