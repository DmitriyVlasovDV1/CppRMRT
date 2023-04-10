#include "primitive.hpp"

// Project namespace
namespace hse {
// Class default constructor
Primitive::Primitive()
    : vertexArrayInstance(nullptr),
      shaderProgramId(0),
      renderType(VertexArray::renderType::TRIANGLES),
      isVisible(true) {
}  // End of 'Primitive::Primitive' function

/* Class constructor.
 * ARGUMENTS:
 *   - primitive's shader program id:
 *       uint primitiveShaderProgramId_;
 *   - vertex buffer data:
 *       const std::vector<float> &vertexBuffer;
 *   - vertex buffer format:
 *       const std::string &vertexBufferFormat;
 *   - index buffer data:
 *       const std::vector<int> &indexBuffer;
 *   - primitive's rendering type:
 *       VertexArray::renderType type_;
 * NOTE: vertexBufferFormat - use default type or "v3v3v3v2" == vertex
 * position, color, normal, texture coordinate.
 */
Primitive::Primitive(
    uint primitiveShaderProgramId_,
    const std::vector<float> &vertexBuffer,
    const std::string &vertexBufferFormat,
    const std::vector<int> &indexBuffer,
    VertexArray::renderType type_
)
    : shaderProgramId(primitiveShaderProgramId_), renderType(type_), isVisible(true) {
    vertexArrayInstance = new VertexArray(vertexBuffer, vertexBufferFormat, indexBuffer);
}  // End of 'Primitive::Primitive' function

/* Class constructor.
 * ARGUMENTS:
 *   - primitive's shader program id:
 *       uint primitiveShaderProgramId_;
 *   - vertex array:
 *       VertexArray *vertexArrayInstance_;
 *   - primitive's rendering type:
 *       VertexArray::renderType type_.
 */
Primitive::Primitive(uint primitiveShaderProgramId_, VertexArray *vertexArrayInstance_, VertexArray::renderType type_)
    : shaderProgramId(primitiveShaderProgramId_),
      vertexArrayInstance(vertexArrayInstance_),
      renderType(type_),
      isVisible(true) {
}  // End of 'Primitive::Primitive' function

/* Render primitive function.
 * ARGUMENTS:
 *   - camera for rendering primitive:
 *      const Camera &camera;
 * RETURNS: None.
 */
void Primitive::onRender(const Camera &camera) const {
    glUseProgram(shaderProgramId);
    int uniformLocation;
    // Handle all non-constant uniforms
    {
        for (auto &[uniformName, uniformValue] : shaderUniform1i) {
            uniformLocation = glGetUniformLocation(shaderProgramId, uniformName);
            if (uniformLocation != -1) glUniform1i(uniformLocation, *uniformValue);
        }
        for (auto &[uniformName, uniformValue] : shaderUniform1f) {
            uniformLocation = glGetUniformLocation(shaderProgramId, uniformName);
            if (uniformLocation != -1) glUniform1f(uniformLocation, *uniformValue);
        }
        for (auto &[uniformName, uniformValue] : shaderUniform3fv) {
            uniformLocation = glGetUniformLocation(shaderProgramId, uniformName);
            if (uniformLocation != -1) glUniform3fv(uniformLocation, 1, &(*uniformValue).x);
        }
        for (auto &[uniformName, uniformPair] : shaderUniform4fv) {
            uniformLocation = glGetUniformLocation(shaderProgramId, uniformName);
            if (uniformLocation != -1) glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, (float *)(*uniformPair).matrix);
        }
    }

    // Handle all constant uniforms
    {
        for (auto &[uniformName, uniformValue] : shaderUniform1iConstant) {
            uniformLocation = glGetUniformLocation(shaderProgramId, uniformName);
            if (uniformLocation != -1) glUniform1i(uniformLocation, uniformValue);
        }
        for (auto &[uniformName, uniformValue] : shaderUniform1fConstant) {
            uniformLocation = glGetUniformLocation(shaderProgramId, uniformName);
            if (uniformLocation != -1) glUniform1f(uniformLocation, uniformValue);
        }
        for (auto &[uniformName, uniformValue] : shaderUniform3fvConstant) {
            uniformLocation = glGetUniformLocation(shaderProgramId, uniformName);
            if (uniformLocation != -1) glUniform3fv(uniformLocation, 1, &uniformValue.x);
        }
        for (auto &[uniformName, uniformPair] : shaderUniform4fvConstant) {
            uniformLocation = glGetUniformLocation(shaderProgramId, uniformName);
            if (uniformLocation != -1) glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, (float *)uniformPair.matrix);
        }
    }

    // Common uniforms for each primitive
    uniformLocation = glGetUniformLocation(shaderProgramId, "cameraPosition");
    if (uniformLocation != -1) {
        math::vec3 tmp = camera.getPosition();
        glUniform3fv(uniformLocation, 1, &tmp.x);
    }
    uniformLocation = glGetUniformLocation(shaderProgramId, "cameraDirection");
    if (uniformLocation != -1) {
        math::vec3 tmp = camera.getDirection();
        glUniform3fv(uniformLocation, 1, &tmp.x);
    }
    uniformLocation = glGetUniformLocation(shaderProgramId, "viewProjection");
    if (uniformLocation != -1)
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, (float *)camera.getViewProjection().matrix);
    uniformLocation = glGetUniformLocation(shaderProgramId, "transformMatrix");
    if (uniformLocation != -1) glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, (float *)transformMatrix.matrix);

    vertexArrayInstance->onRender(renderType);
    glUseProgram(0);
}  // End of 'Primitive::onRender' function

// Class destructor
Primitive::~Primitive() {
    delete vertexArrayInstance;
}  // End of 'Primitive::~Primitive' function

/* Attach shader program id to the primitive function.
 * ARGUMENTS:
 *   - shader program id:
 *       uint shaderProgramId_;
 * RETURNS: None.
 */
void Primitive::setShaderProgram(uint shaderProgramId_) {
    shaderProgramId = shaderProgramId_;
}  // End of 'Primitive::setShaderProgram' function

/* Get primitive's transform matrix function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (math::matr4) - primitive's transform matrix.
 */
math::matr4 Primitive::getTransformMatrix() const {
    return transformMatrix;
}  // End of 'Primitive::getTransformMatrix' function

/* Set primitive's transform matrix function.
 * ARGUMENTS:
 *   - new primitive transform matrix:
 *       const math::matr4 &transformMatrix_;
 * RETURNS: None.
 */
void Primitive::setTransformMatrix(const math::matr4 &transformMatrix_) {
    transformMatrix = transformMatrix_;
}  // End of 'Primitive::setTransformMatrix' function

/* Get primitive rendering type function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (VertexArray::renderType) - rendering type.
 */
VertexArray::renderType Primitive::getRenderType() const {
    return renderType;
}  // End of 'Primitive::getRenderType' function

/* Set primitive rendering type function.
 * ARGUMENTS:
 *   - new rendering type:
 *       VertexArray::renderType type_:
 * RETURNS: None.
 */
void Primitive::setRenderType(VertexArray::renderType renderType_) {
    renderType = renderType_;
}  // End of 'Primitive::setRenderType' function

/* Get primitive's visibility flag function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (bool) - visibility flag.
 */
bool Primitive::getVisibility() const {
    return isVisible;
}  // End of 'Primitive::getVisibility' function

/* Set primitive's visibility flag function.
 * ARGUMENTS:
 *   - new flag:
 *       bool isVisible_;
 * RETURNS: None.
 */
void Primitive::setVisibility(bool isVisible_) {
    isVisible = isVisible_;
}  // End of 'Primitive::setVisibility' function

/* Add uniform of one int variable to the shader function.
 * ARGUMENTS:
 *   - uniform value:
 *       const int *uniformValue;
 *   - uniform name on the shader:
 *       const char *uniformName;
 * RETURNS: None.
 */
void Primitive::addUniform(const int *uniformValue, const char *uniformName) {
    shaderUniform1i[uniformName] = uniformValue;
}  // End of 'Primitive::addUniform' function

/* Add uniform of one float variable to the shader function.
 * ARGUMENTS:
 *   - uniform value:
 *       const float *uniformValue;
 *   - uniform name on the shader:
 *       const char *uniformName;
 * RETURNS: None.
 */
void Primitive::addUniform(const float *uniformValue, const char *uniformName) {
    shaderUniform1f[uniformName] = uniformValue;
}  // End of 'Primitive::addUniform' function

/* Add uniform of 3-component geom vector to the shader function.
 * ARGUMENTS:
 *   - uniform value:
 *       const math::vec3 *vector;
 *   - uniform name on the shader:
 *       const char *uniformName;
 * RETURNS: None.
 */
void Primitive::addUniform(const math::vec3 *vector, const char *uniformName) {
    shaderUniform3fv[uniformName] = vector;
}  // End of 'Primitive::addUniform' function

/* Add uniform of matrix4x4 variable to the shader function.
 * ARGUMENTS:
 *   - uniform value:
 *       const math::matr4 *matrix;
 *   - uniform name on the shader:
 *       const char *uniformName;
 * RETURNS: None.
 */
void Primitive::addUniform(const math::matr4 *matrix, const char *uniformName) {
    shaderUniform4fv[uniformName] = matrix;
}  // End of 'Primitive::addUniform' function

/* Add constant uniform of one integer variable to the shader function.
 * ARGUMENTS:
 *   - uniform value:
 *       int uniformValue;
 *   - uniform name on the shader:
 *       const char *uniformName;
 * RETURNS: None.
 */
void Primitive::addConstantUniform(int uniformValue, const char *uniformName) {
    shaderUniform1iConstant[uniformName] = uniformValue;
}  // End of 'Primitive::addConstantUniform' function

/* Add constant uniform of one float variable to the shader function.
 * ARGUMENTS:
 *   - uniform value:
 *       float uniformValue;
 *   - uniform name on the shader:
 *       const char *uniformName;
 * RETURNS: None.
 */
void Primitive::addConstantUniform(float uniformValue, const char *uniformName) {
    shaderUniform1fConstant[uniformName] = uniformValue;
}  // End of 'Primitive::addConstantUniform' function

/* Add constant uniform of 3-component geom vector to the shader function.
 * ARGUMENTS:
 *   - uniform value:
 *       const math::vec3 &vector;
 *   - uniform name on the shader:
 *       const char *uniformName;
 * RETURNS: None.
 */
void Primitive::addConstantUniform(const math::vec3 &vector, const char *uniformName) {
    shaderUniform3fvConstant[uniformName] = vector;
}  // End of 'Primitive::addConstantUniform' function

/* Add constant uniform of matrix4x4 variable to the shader function.
 * ARGUMENTS:
 *   - uniform value:
 *       const math::matr4 &matrix;
 *   - uniform name on the shader:
 *       const char *uniformName;
 * RETURNS: None.
 */
void Primitive::addConstantUniform(const math::matr4 &matrix, const char *uniformName) {
    shaderUniform4fvConstant[uniformName] = matrix;
}  // End of 'Primitive::addConstantUniform' function
}  // namespace hse