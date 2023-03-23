#include "primitive.hpp"

// Project namespace
namespace hse {
// Class default constructor
primitive::primitive()
    : vertexArrayInstance(nullptr),
      shaderProgramId(0),
      renderingType(buffer::renderType::TRIANGLES),
      isVisible(true) {
}  // End of 'primitive::primitive' function

/* Class constructor.
 * ARGUMENTS:
 *   - primitive's shader program id:
 *       uint primitiveShaderProgramId_;
 *   - vertex buffer data:
 *       const ::std::vector<float> &vertexBuffer;
 *   - vertex buffer format:
 *       const ::std::string &vertexBufferFormat;
 *   - index buffer data:
 *       const ::std::vector<int> &indexBuffer;
 *   - primitive's rendering type:
 *       renderType type_;
 * NOTE: vertexBufferFormat - use default type or "v3v3v3v2" == vertex
 * position, color, normal, texture coordinate.
 */
primitive::primitive(
    uint primitiveShaderProgramId_,
    const ::std::vector<float> &vertexBuffer,
    const ::std::string &vertexBufferFormat,
    const ::std::vector<int> &indexBuffer,
    buffer::renderType type_
)
    : shaderProgramId(primitiveShaderProgramId_),
      renderingType(type_),
      isVisible(true) {
    vertexArrayInstance =
        new vertexArray(vertexBuffer, vertexBufferFormat, indexBuffer);
}  // End of 'primitive::primitive' function

/* Class constructor.
 * ARGUMENTS:
 *   - primitive's shader program id:
 *       uint primitiveShaderProgramId_;
 *   - vertex array:
 *       vertexArray *vertexArrayInstance_;
 *   - primitive's rendering type:
 *       renderType type_.
 */
primitive::primitive(
    uint primitiveShaderProgramId_,
    vertexArray *vertexArrayInstance_,
    buffer::renderType type_
)
    : shaderProgramId(primitiveShaderProgramId_),
      vertexArrayInstance(vertexArrayInstance_),
      renderingType(type_),
      isVisible(true) {
}  // End of 'primitive::primitive' function

/* Draw primitive function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void primitive::render() const {
    glUseProgram(shaderProgramId);
    int uniformLocation;
    for (auto &[uniformName, uniformValue] : shaderUniform1i) {
        uniformLocation = glGetUniformLocation(shaderProgramId, uniformName);
        if (uniformLocation != -1) glUniform1i(uniformLocation, *uniformValue);
    }
    for (auto &[uniformName, uniformValue] : shaderUniform1f) {
        uniformLocation = glGetUniformLocation(shaderProgramId, uniformName);
        if (uniformLocation != -1) glUniform1f(uniformLocation, *uniformValue);
    }
    for (auto &[uniformName, uniformPair] : shaderUniform3fv) {
        uniformLocation = glGetUniformLocation(shaderProgramId, uniformName);
        if (uniformLocation != -1)
            glUniform3fv(
                uniformLocation, uniformPair.second, *uniformPair.first
            );
    }
    for (auto &[uniformName, uniformPair] : shaderUniform4fv) {
        uniformLocation = glGetUniformLocation(shaderProgramId, uniformName);
        if (uniformLocation != -1)
            glUniformMatrix4fv(
                uniformLocation, uniformPair.second, GL_FALSE, uniformPair.first
            );
    }
    vertexArrayInstance->render(renderingType);
    glUseProgram(0);
}  // End of 'primitive::primitiveDraw' function

// Class destructor
primitive::~primitive() {
    delete vertexArrayInstance;
    ::std::cout << "Clear primitive" << ::std::endl;
}  // End of 'primitive::~primitive' function

/* Attach shader program id to the primitive function.
 * ARGUMENTS:
 *   - shader program id:
 *       uint shaderProgramId_;
 * RETURNS: None.
 */
void primitive::setShaderProgram(uint shaderProgramId_) {
    shaderProgramId = shaderProgramId_;
}  // End of 'primitive::setShaderProgram' function

/* Get primitive rendering type function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (buffer::renderType) - rendering type.
 */
buffer::renderType primitive::getRenderType() const {
    return renderingType;
}  // End of 'primitive::getRenderType' function

/* Set primitive rendering type function.
 * ARGUMENTS:
 *   - new rendering type:
 *       buffer::renderType type_:
 * RETURNS: None.
 */
void primitive::setRenderType(buffer::renderType type_) {
    renderingType = type_;
}  // End of 'primitive::setRenderType' function

/* Get primitive's visibility flag function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (bool) - visibility flag.
 */
bool primitive::getVisibility() const {
    return isVisible;
}  // End of 'primitive::getVisibility' function

/* Set primitive's visibility flag function.
 * ARGUMENTS:
 *   - new flag:
 *       bool isVisible_;
 * RETURNS: None.
 */
void primitive::setVisibility(bool isVisible_) {
    isVisible = isVisible_;
}  // End of 'primitive::setVisibility' function

/* Add uniform of one int variable to the shader function.
 * ARGUMENTS:
 *   - uniform value:
 *       const int *uniformValue;
 *   - uniform name on the shader:
 *       const char *uniformName;
 * RETURNS: None.
 */
void primitive::addUniform(const int *uniformValue, const char *uniformName) {
    shaderUniform1i[uniformName] = const_cast<int *>(uniformValue);
}  // End of 'primitive::addUniform' function

/* Add uniform of one float variable to the shader function.
 * ARGUMENTS:
 *   - uniform value:
 *       const float *uniformValue;
 *   - uniform name on the shader:
 *       const char *uniformName;
 * RETURNS: None.
 */
void primitive::addUniform(const float *uniformValue, const char *uniformName) {
    shaderUniform1f[uniformName] = const_cast<float *>(uniformValue);
}  // End of 'primitive::addUniform' function

/* Add uniform of 3-component geom vector to the shader function.
 * ARGUMENTS:
 *   - uniform value:
 *       const float **uniformValue;
 *   - uniform name on the shader:
 *       const char *uniformName;
 *   - uniforms number:
 *       int uniformCount;
 * RETURNS: None.
 */
void primitive::addUniform(
    const float **uniformValue,
    const char *uniformName,
    int uniformCount
) {
    shaderUniform3fv[uniformName] = {
        const_cast<float **>(uniformValue), uniformCount};
}  // End of 'shader::shaderAddUniform3fv' function

void primitive::addUniform4fv(
    const float *uniformValue,
    const char *uniformName,
    int uniformCount
) {
    shaderUniform4fv[uniformName] = {
        const_cast<float *>(uniformValue), uniformCount};
}  // End of 'primitive::addUniform' function

}  // namespace hse