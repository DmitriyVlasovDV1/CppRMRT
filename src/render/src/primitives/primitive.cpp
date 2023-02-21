#include "primitive.hpp"

// Project namespace
namespace hse {
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
 * NOTE: vertexBufferFormat - use default type or "v3v3v3v2" == vertex
 * position, color, normal, texture coordinate.
 */
primitive::primitive(
    uint primitiveShaderProgramId_,
    const ::std::vector<float> &vertexBuffer,
    const ::std::string &vertexBufferFormat,
    const ::std::vector<int> &indexBuffer
)
    : primitiveShaderProgramId(primitiveShaderProgramId_) {
    primitiveVertexArrayInstance =
        new vertexArray(vertexBuffer, vertexBufferFormat, indexBuffer);
}  // End of 'primitive::primitive' function

/* Draw primitive function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void primitive::primitiveDraw() {
    glUseProgram(primitiveShaderProgramId);
    int uniformLocation;
    for (auto &[uniformName, uniformValue] : shaderUniform1i) {
        uniformLocation =
            glGetUniformLocation(primitiveShaderProgramId, uniformName);
        if (uniformLocation != -1) glUniform1i(uniformLocation, *uniformValue);
    }
    for (auto &[uniformName, uniformValue] : shaderUniform1f) {
        uniformLocation =
            glGetUniformLocation(primitiveShaderProgramId, uniformName);
        if (uniformLocation != -1) glUniform1f(uniformLocation, *uniformValue);
    }
    for (auto &[uniformName, uniformPair] : shaderUniform3fv) {
        uniformLocation =
            glGetUniformLocation(primitiveShaderProgramId, uniformName);
        if (uniformLocation != -1)
            glUniform3fv(
                uniformLocation, uniformPair.second, *uniformPair.first
            );
    }
    primitiveVertexArrayInstance->drawVertexArray();
    glUseProgram(0);
}  // End of 'primitive::primitiveDraw' function

// Class destructor
primitive::~primitive() {
    delete primitiveVertexArrayInstance;
    ::std::cout << "Clear primitive" << ::std::endl;
}  // End of 'primitive::~primitive' function

/* Add uniform to the shader function.
 * ARGUMENTS:
 *   - uniform value:
 *       int *uniformValue;
 *   - uniform name on the shader:
 *       const char *uniformName;
 * RETURNS: None.
 */
void primitive::shaderAddUniform1i(int *uniformValue, const char *uniformName) {
    shaderUniform1i[uniformName] = uniformValue;
}  // End of 'shader::shaderAddUniform1i' function

/* Add uniform to the shader function.
 * ARGUMENTS:
 *   - uniform value:
 *       float *uniformValue;
 *   - uniform name on the shader:
 *       const char *uniformName;
 * RETURNS: None.
 */
void primitive::shaderAddUniform1f(
    float *uniformValue,
    const char *uniformName
) {
    shaderUniform1f[uniformName] = uniformValue;
}  // End of 'shader::shaderAddUniform1f' function

/* Add uniform to the shader function.
 * ARGUMENTS:
 *   - uniform value:
 *       float **uniformValue;
 *   - uniform name on the shader:
 *       const char *uniformName;
 *   - uniforms number:
 *       int uniformCount;
 * RETURNS: None.
 */
void primitive::shaderAddUniform3fv(
    float **uniformValue,
    const char *uniformName,
    int uniformCount
) {
    shaderUniform3fv[uniformName] = {uniformValue, uniformCount};
}  // End of 'shader::shaderAddUniform3fv' function

}  // namespace hse