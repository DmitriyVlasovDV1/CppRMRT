#include "unit.hpp"

// Project namespace
namespace hse {
/* Get unit's visibility flag function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (bool) - visibility flag.
 */
bool unit::getVisibility() const {
    return isVisible;
}  // End of 'unit::getVisibility' function

/* Set unit's visibility flag function.
 * ARGUMENTS:
 *   - new flag:
 *       bool isVisible_;
 * RETURNS: None.
 */
void unit::setVisibility(bool isVisible_) {
    isVisible = isVisible_;
}  // End of 'unit::setVisibility' function

/* Create vertex buffer function.
 * ARGUMENTS:
 *   - buffer's data:
 *       const ::std::vector<float> &vertexBufferData;
 *   - buffer's format:
 *       const ::std::string &vertexBufferFormat;
 * NOTE: vertexBufferFormat - use default type or "v3v3v3v2" == vertex
 * position, color, normal, texture coordinate.
 */
uint unit::createVertexBuffer(
    const ::std::vector<float> &vertexBufferData,
    const ::std::string &vertexBufferFormat
) {
    buffersArray.emplace_back(
        new vertexBuffer(vertexBufferData, vertexBufferFormat)
    );
    return buffersArray.back()->getBufferId();
}  // End of 'unit::createVertexBuffer' function

/* Create index buffer function.
 * ARGUMENTS:
 *   - buffer's data;
 *       const ::std::vector<int> &indexBufferData.
 */
uint unit::createIndexBuffer(const ::std::vector<int> &indexBufferData) {
    buffersArray.emplace_back(new indexBuffer(indexBufferData));
    return buffersArray.back()->getBufferId();
}  // End of 'unit::createIndexBuffer' function

/* Create vertex array function.
 * ARGUMENTS:
 *   - vertex buffer data:
 *       const ::std::vector<float> &vertexBufferData;
 *   - vertex buffer format:
 *       const ::std::string &vertexBufferFormat;
 *   - index buffer data:
 *       const ::std::vector<int> &indexBufferData;
 * NOTE: vertexBufferFormat - use default type or "v3v3v3v2" == vertex
 * position, color, normal, texture coordinate.
 */
uint unit::createVertexArray(
    const ::std::vector<float> &vertexBufferData,
    const ::std::string &vertexBufferFormat,
    const ::std::vector<int> &indexBufferData
) {
    buffersArray.emplace_back(
        new vertexArray(vertexBufferData, vertexBufferFormat, indexBufferData)
    );
    return buffersArray.back()->getBufferId();
}  // End of 'unit::createVertexArray' function

/* Create shader storage buffer function.
 * ARGUMENTS:
 *   - buffer's data:
 *       const ::std::vector<T> &bufferData;
 *   - buffer's binding value:
 *       uint bufferBinding.
 */
template <typename T>
uint unit::createShaderStorageBuffer(
    const ::std::vector<T> &bufferData,
    uint bufferBinding
) {
    buffersArray.emplace_back(new shaderStorageBuffer(bufferData, bufferBinding)
    );
    return buffersArray.back()->getBufferId();
}  // End of 'unit::createShaderStorageBuffer' function

/* Create shader function.
 * ARGUMENTS:
 *   - path to shader's realization (read shader class constructor note)
 *       const ::std::string &shaderPath;
 * RETURNS:
 *   (uint) - shader program id.
 */
uint unit::createShader(const ::std::string &shaderPath) {
    if (shadersArray[shaderPath])
        return shadersArray[shaderPath]->getShaderProgramId();
    shadersArray[shaderPath] = new shader(shaderPath);
    return shadersArray[shaderPath]->getShaderProgramId();
}  // End of 'render::shaderAdd' function

/* Create shader function.
 * ARGUMENTS:
 *   - vertex shader source in string:
 *       const ::std::string &vertexShaderSource;
 *   - fragment shader source in string:
 *       const ::std::string &fragmentShaderSource;
 *   - path to shader's realization (read shader class constructor note)
 *       const ::std::string &shaderPath;
 * RETURNS:
 *   (uint) - shader program id.
 */
uint unit::createShader(
    const ::std::string &vertexShaderSource,
    const ::std::string &fragmentShaderSource,
    const ::std::string &shaderPath
) {
    if (!shaderPath.empty() && shadersArray[shaderPath])
        return shadersArray[shaderPath]->getShaderProgramId();
    shadersArray[shaderPath] =
        new shader(vertexShaderSource, fragmentShaderSource, shaderPath);
    return shadersArray[shaderPath]->getShaderProgramId();
}

/* Create primitive function.
 * ARGUMENTS:
 *   - path to primitive's shader:
 *       const ::std::string &shaderPath;
 *   - vertex buffer data:
 *       const ::std::vector<float> &vertexBuffer;
 *   - vertex buffer format:
 *       const ::std::string &vertexBufferFormat;
 *   - index buffer data:
 *       const ::std::vector<int> &indexBuffer;
 * RETURNS:
 *   (primitive *) - not-owning pointer to the created primitive;
 * NOTE: vertexBufferFormat - use default type or "v3v3v3v2" == vertex
 * position, color, normal, texture coordinate.
 */
primitive *unit::createPrimitive(
    const ::std::string &shaderPath,
    const ::std::vector<float> &vertexBuffer,
    const ::std::string &vertexBufferFormat,
    const ::std::vector<int> &indexBuffer
) {
    primitivesArray.emplace_back(new primitive(
        createShader(shaderPath), vertexBuffer, vertexBufferFormat, indexBuffer
    ));
    return primitivesArray.back();
}  // End of 'render::primitiveAdd' function

// Class constructor
unit::unit() : isInitialized(false), isVisible(true) {
}  // End of 'unit::unit' function

/* Render unit function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void unit::render() const {
    for (auto &primitiveInstance : primitivesArray)
        if (primitiveInstance->getVisibility()) primitiveInstance->render();
}  // End of 'unit::render' function

/* Clear unit function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void unit::clear() {
    for (auto &bufferInstance : buffersArray)
        delete bufferInstance;
    for (auto &[shaderName, shaderInstance] : shadersArray)
        delete shaderInstance;
    for (auto &primitiveInstance : primitivesArray)
        delete primitiveInstance;
}  // End of 'unit::clear' function
}  // namespace hse
