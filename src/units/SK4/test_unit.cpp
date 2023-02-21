#include "test_unit.hpp"

// Project namespace
namespace hse {
/* Unit initialization override function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void testUnit::initUnit() {
    ::std::vector<int> indexBuffer(3);
    ::std::vector<float> vertexBuffer = {0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
                                         -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
                                         0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f};

    for (int j = 0; j < 3; j++)
        indexBuffer[j] = j;
    render::renderInstance.primitiveAdd(
        "test", vertexBuffer, "v3v3", indexBuffer
    );
}  // End of 'testUnit::initUnit' function

/* Unit response override function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void testUnit::responseUnit() {
}  // End of 'testUnit::responseUnit' function

// Class override destructor
testUnit::~testUnit() {
    ::std::cout << "Clear unit" << ::std::endl;
}  // End of 'testUnit::~testUnit' function
}  // namespace hse