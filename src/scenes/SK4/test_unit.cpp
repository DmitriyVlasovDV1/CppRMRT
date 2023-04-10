#include "test_unit.hpp"

// Project namespace
namespace hse {
/* Sphere generation function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void TestScene::spheresGeneration() {
    const size_t spheresNumber = 1000;
    for (int i = 0; i < spheresNumber; i++) {
        float x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 100,
              y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 100,
              z = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 100;
        Primitive *tmp = createSpherePrimitive(1, math::vec3(x - 50, y - 50, z - 50), 20, 20);
        tmp->addUniform(&time, "time");
        math::vec3 color = math::vec3(x / 100, y / 100, z / 100);
        tmp->addConstantUniform(color, "vertexColor");
    }
}  // End of 'TestScene::spheresGeneration' function

/* Cubes generation function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void TestScene::cubesGeneration() {
    const size_t cubesNumber = 1000;
    for (int i = 0; i < cubesNumber; i++) {
        float x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 100,
              y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 100,
              z = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 100;
        Primitive *tmp = createCubePrimitive(1, 1, 1, math::vec3(x - 50, y - 50, z - 50));
        tmp->setTransformMatrix(
            math::matr4::rotateX(z) * math::matr4::rotateY(x) * math::matr4::rotateZ(y) * tmp->getTransformMatrix()
        );
        tmp->addUniform(&time, "time");
        math::vec3 color = math::vec3(x / 100, y / 100, z / 100);
        tmp->addConstantUniform(color, "vertexColor");
    }
}  // End of 'TestScene::cubesGeneration' function

/* Camera update function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void TestScene::cameraUpdate() {
    math::vec3 newCameraLocation = math::vec3((cos(time / 1.5f) + 1) * 50, 50, (sin(time / 1.5f) + 1) * 50), up, right;
    up = (newCameraLocation + math::vec3(0, 1, 0)).normalize();
    right = (up % newCameraLocation).normalize();
    up = (newCameraLocation % right).normalize();
    mainCamera.setAllAxis(newCameraLocation, math::vec3(0) - newCameraLocation, up, right);
}  // End of 'TestScene::cameraUpdate' function

/* Input (keyboard/mouse) update function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void TestScene::inputUpdate() {
    if (keys[GLFW_KEY_W].action == GLFW_PRESS) ::std::cout << 'w' << ::std::endl;
    if (keys[GLFW_KEY_A].action == GLFW_PRESS) ::std::cout << 'a' << ::std::endl;
    if (keys[GLFW_KEY_S].action == GLFW_PRESS) ::std::cout << 's' << ::std::endl;
    if (keys[GLFW_KEY_D].action == GLFW_PRESS) ::std::cout << 'd' << ::std::endl;

    // Pause functional
    static int oldAction;
    if (keys[GLFW_KEY_SPACE].action == GLFW_PRESS && oldAction == GLFW_RELEASE) {
        isPause = !isPause;
        oldAction = GLFW_PRESS;
    } else if (keys[GLFW_KEY_SPACE].action == GLFW_RELEASE)
        oldAction = GLFW_RELEASE;
}  // End of 'TestScene::inputUpdate' function

/* Unit creation override function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void TestScene::onCreate() {
    unitModel = createModel("obj_model", "tea_cup/tea_cup.obj");
    unitModel->setRenderType(VertexArray::renderType::LINES);
    unitModel->addConstantUniform(math::vec3(0.3, 0.5, 0.7), "vertexColor");
    unitModel->setTransformMatrix(math::matr4::translate(math::vec3(0, -1, 0)) * math::matr4::scale(math::vec3(10)));
    spheresGeneration();
    cubesGeneration();
}  // End of 'TestScene::onCreate' function

/* Unit update override function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void TestScene::onUpdate() {
    unitModel->setTransformMatrix(
        math::matr4::translate(math::vec3(0, -1, 0)) * math::matr4::scale(math::vec3(10)) *
        math::matr4::rotateY(time * 50) * math::matr4::rotateX(-time * 30) * math::matr4::rotateZ(time * 20)
    );
    cameraUpdate();
    inputUpdate();
}  // End of 'TestScene::onUpdate' function

// Class override destructor
TestScene::~TestScene() {
}  // End of 'TestScene::~TestScene' function
}  // namespace hse
