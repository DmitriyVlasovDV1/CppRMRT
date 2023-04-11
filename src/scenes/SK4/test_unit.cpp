#include "test_unit.hpp"

// Project namespace
namespace hse {
/* Sphere generation function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void TestScene::spheresGeneration() {
    //    const size_t spheresNumber = 1000;
    //    for (int i = 0; i < spheresNumber; i++) {
    //        float x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 100,
    //              y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 100,
    //              z = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 100;
    //        Primitive *tmp =
    //            createSpherePrimitive(1, math::vec3(x - 50, y - 50, z - 50), math::vec3(x / 100, y / 100, z / 100),
    //            20, 20);
    //        tmp->addUniform(&time, "time");
    //    }
    scenePrimitive = createSpherePrimitive(0.6, math::vec3(-0.5, 1, 0.5), math::vec3(0.6, 0.3, 0.2));
    scenePrimitive = createSpherePrimitive(0.5, math::vec3(0.5, 1.5, -1.7), math::vec3(0.6, 0.2, 0.3));
}  // End of 'TestScene::spheresGeneration' function

/* Spheres update function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void TestScene::spheresUpdate() {
    scenePrimitive->setTransformMatrix(scenePrimitive->getTransformMatrix() * math::matr4::rotateY(deltaTime * 100));
}  // End of 'TestScene::spheresUpdate' function

/* Cubes generation function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void TestScene::cubesGeneration() {
    //    const size_t cubesNumber = 1000;
    //    for (int i = 0; i < cubesNumber; i++) {
    //        float x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 100,
    //              y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 100,
    //              z = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 100;
    //        Primitive *tmp =
    //            createCubePrimitive(1, 1, 1, math::vec3(x - 50, y - 50, z - 50), math::vec3(x / 100, y / 100, z /
    //            100));
    //        tmp->setTransformMatrix(
    //            math::matr4::rotateX(z) * math::matr4::rotateY(x) * math::matr4::rotateZ(y) *
    //            tmp->getTransformMatrix()
    //        );
    //        tmp->addUniform(&time, "time");
    //    }
    scenePrimitive = createCubePrimitive(1.3, 1.3, 1.3, math::vec3(0), math::vec3(0.3, 0.6, 0.2));
    scenePrimitive->setTransformMatrix(
        math::matr4::rotateX(45) * math::matr4::rotateZ(45) * math::matr4::translate(math::vec3(0, 1, 0))
    );
}  // End of 'TestScene::cubesGeneration' function

/* Cubes update function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void TestScene::cubesUpdate() {
    scenePrimitive->setTransformMatrix(scenePrimitive->getTransformMatrix() * math::matr4::rotateY(deltaTime * 100));
}  // End of 'TestScene::cubesUpdate' function

/* Tree generation function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void TestScene::treeGeneration() {
    scenePrimitive = createPlanePrimitive(2, 2, math::vec3(0, 0, 0), math::vec3(0.256861, 0.440506, 0.110769));
    sceneModel = createModel("obj_model", "low_poly_tree/low_poly_tree.obj");
    sceneModel->setTransformMatrix(math::matr4::scale(math::vec3(0.1)));
}  // End of 'TestScene::treeGeneration' function

/* Tree update function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void TestScene::treeUpdate() {
    sceneModel->setTransformMatrix(sceneModel->getTransformMatrix() * math::matr4::rotateY(deltaTime * 100));
    scenePrimitive->setTransformMatrix(scenePrimitive->getTransformMatrix() * math::matr4::rotateY(deltaTime * 100));
}  // End of 'TestScene::treeUpdate' function

/* Cup generation function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void TestScene::cupGeneration() {
    sceneModel = createModel("obj_model", "tea_cup/tea_cup.obj");
    sceneModel->setTransformMatrix(
        math::matr4::scale(math::vec3(0.4)) * math::matr4::rotateX(20) * math::matr4::translate(math::vec3(0, 0.6, 0))
    );
}  // End of 'TestScene::cupGeneration' function

/* Cup update function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void TestScene::cupUpdate() {
    sceneModel->setTransformMatrix(sceneModel->getTransformMatrix() * math::matr4::rotateY(deltaTime * 100));
}  // End of 'TestScene::cupUpdate' function

/* Objects (primitives/models) generation function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void TestScene::objectsGeneration() {
    //    unitModel = createModel("obj_model", "low_poly_tree/low_poly_tree.obj");
    //    unitModel->setRenderType(VertexArray::renderType::TRIANGLES);
    //    unitModel->setTransformMatrix(math::matr4::translate(math::vec3(0, 0, 0)) *
    //    math::matr4::scale(math::vec3(1.5))); for (int count = 0; count < 15; count++) {
    //        float x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 100 - 50,
    //              z = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 100 - 50,
    //              scale = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) + 1) / 2;
    //        unitModel = createModel("obj_model", "low_poly_tree/low_poly_tree.obj");
    //        unitModel->setRenderType(VertexArray::renderType::TRIANGLES);
    //        unitModel->setTransformMatrix(
    //            math::matr4::scale(math::vec3(scale)) * math::matr4::translate(math::vec3(x, 0, z))
    //        );
    //    }
}  // End of 'TestScene::objectsGeneration' function

/* Objects (primitives/models) update function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void TestScene::objectsUpdate() {
}  // End of 'TestScene::objectsUpdate' function

/* Camera update function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void TestScene::cameraUpdate() {
    mainCamera.setAllAxis(math::vec3(4, 1.2, 0), math::vec3(-3, 0, 0), math::vec3(0, 1, 0), math::vec3(0, 0, -1));
    //    math::vec3 newCameraLocation = math::vec3((cos(time / 1.5f) + 1) * 50, 50, (sin(time / 1.5f) + 1) * 50), up,
    //    right; up = (newCameraLocation + math::vec3(0, 1, 0)).normalize(); right = (up %
    //    newCameraLocation).normalize(); up = (newCameraLocation % right).normalize();
    //    mainCamera.setAllAxis(newCameraLocation, math::vec3(0) - newCameraLocation, up, right);
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
    //    cupGeneration();
    treeGeneration();
    //    cubesGeneration();
    //    spheresGeneration();
}  // End of 'TestScene::onCreate' function

/* Unit update override function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void TestScene::onUpdate() {
    //    cupUpdate();
    treeUpdate();
    //    cubesUpdate();
    //    spheresUpdate();
    cameraUpdate();
    inputUpdate();
}  // End of 'TestScene::onUpdate' function

// Class override destructor
TestScene::~TestScene() {
}  // End of 'TestScene::~TestScene' function
}  // namespace hse
