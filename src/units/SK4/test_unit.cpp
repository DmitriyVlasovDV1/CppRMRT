#include "test_unit.hpp"

// Project namespace
namespace hse {
/* Unit initialization override function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void testUnit::init() {
    ::std::vector<int> indexBuffer(3);
    ::std::vector<float> vertexBuffer = {0.5,  -0.5, 0, 0.6, 1, 0,
                                         -0.5, -0.5, 0, 0, 1, 1,
                                         0,    0.5,  0, 1, 0, 1};

    for (int j = 0; j < 3; j++)
        indexBuffer[j] = j;
    unitPrimitive = createPrimitive("test", vertexBuffer, "v3v3", indexBuffer);
    unitPrimitive->setRenderType(buffer::renderType::LINES);
    unitPrimitive->addUniform(&render::renderInstance.getTime(), "time");
    unitPrimitive->setRenderType(buffer::renderType::LINES);

    mainCamera.moveTo(math::vec3(0, 10, 0));

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
