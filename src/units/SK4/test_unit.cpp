#include "test_unit.hpp"

// Project namespace
namespace hse {
/* Unit initialization override function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void testUnit::initUnit() {
    int indexBuffer[3];
    float vertexBuffer[3 * 2 * 3] = {0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
                                     -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
                                     0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f};

    for (int j = 0; j < 3; j++)
        indexBuffer[j] = j;

    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);
    glGenBuffers(1, &vertexBufferId);
    glGenBuffers(1, &indexBufferId);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(vertexBuffer), vertexBuffer, GL_STATIC_DRAW
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, sizeof(indexBuffer), indexBuffer,
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)static_cast<intptr_t>(0)
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}  // End of 'testUnit::initUnit' function

/* Unit response override function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void testUnit::responseUnit() {
    glUseProgram(testProgramId);
    int uniformLocation;
    if ((uniformLocation = glGetUniformLocation(testProgramId, "time")) != -1)
        glUniform1f(uniformLocation, render::renderInstance.getTime());
    if ((uniformLocation = glGetUniformLocation(testProgramId, "width")) != -1)
        glUniform1ui(uniformLocation, render::renderInstance.getWindowWidth());
    if ((uniformLocation = glGetUniformLocation(testProgramId, "height")) != -1)
        glUniform1ui(uniformLocation, render::renderInstance.getWindowHeight());
    glBindVertexArray(vertexArrayId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
//    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
    glDrawElements(GL_TRIANGLE_STRIP, 3, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glUseProgram(0);
}  // End of 'testUnit::responseUnit' function

// Class override destructor
testUnit::~testUnit() {
    glBindVertexArray(vertexArrayId);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &vertexBufferId);
    glDeleteBuffers(1, &indexBufferId);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vertexArrayId);
}
}  // namespace hse