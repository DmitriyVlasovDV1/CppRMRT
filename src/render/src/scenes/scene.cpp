#include "scene.hpp"

// Project namespace
namespace hse {
/* Create vertex buffer function.
 * ARGUMENTS:
 *   - buffer's data:
 *       const ::std::vector<float> &vertexBufferData;
 *   - buffer's format:
 *       const ::std::string &vertexBufferFormat;
 * NOTE: vertexBufferFormat - use default type or "v3v3v3v2" == vertex
 * position, color, normal, texture coordinate.
 */
uint unit::createVertexBuffer(const ::std::vector<float> &vertexBufferData, const ::std::string &vertexBufferFormat) {
    buffersArray.emplace_back(new vertexBuffer(vertexBufferData, vertexBufferFormat));
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
uint unit::createVertexArray(const ::std::vector<float> &vertexBufferData, const ::std::string &vertexBufferFormat, const ::std::vector<int> &indexBufferData) {
    buffersArray.emplace_back(new vertexArray(vertexBufferData, vertexBufferFormat, indexBufferData));
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
uint unit::createShaderStorageBuffer(const ::std::vector<T> &bufferData, uint bufferBinding) {
    buffersArray.emplace_back(new shaderStorageBuffer(bufferData, bufferBinding));
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
    if (shadersArray[shaderPath]) return shadersArray[shaderPath]->getShaderProgramId();
    shadersArray[shaderPath] = new shader(shaderPath);
    return shadersArray[shaderPath]->getShaderProgramId();
}  // End of 'render::shaderAdd' function

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
        createShader(shaderPath), vertexBuffer,
        vertexBufferFormat, indexBuffer
    ));
    return primitivesArray.back();
}  // End of 'render::primitiveAdd' function

// Class constructor
unit::unit()
    : isInitialized(false) {
}  // End of 'unit::unit' function

/* Render unit function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void unit::render() const {
    for (auto &primitiveInstance : primitivesArray)
        primitiveInstance->render();
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

// Class default constructor
scene::scene()
    : sceneVisibility(true) {
}  // End of 'scene' function

/* Initialize scene function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void scene::init() {
    for (auto &[unitName, unitInstance] : unitsArray)
        if (!unitInstance->isInitialized)
            unitInstance->init(), unitInstance->isInitialized = true;
}

/* Response scene function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void scene::response() {
    for (auto &[unitName, unitInstance] : unitsArray)
        unitInstance->response();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (auto &[unitName, unitInstance] : unitsArray)
        unitInstance->render();
}  // End of 'scene::sceneResponse' function

/* Add unit to the scene function.
 * ARGUMENTS:
 *   - unit name:
 *       ::std::string &&unitName;
 *   - unit instance:
 *       unit &&unitInstance;
 * RETURNS: None.
 */
void scene::addUnit(const ::std::string &unitName, const unit *unitInstance) {
    unitsArray[unitName] = const_cast<unit *>(unitInstance);
    init();
}  // End of 'scene::addUnit' function

/* Delete unit from the scene function.
 * ARGUMENTS:
 *   - unit name:
 *       const ::std::string &unitName;
 * RETURNS:
 *   (unit *) - not-owning pointer to deleted unit.
 */
unit *scene::deleteUnit(const ::std::string &unitName) {
    unit *tmp = unitsArray[unitName];
    unitsArray.erase(unitName);
    return tmp;
}  // End of 'scene::deleteUnitFromScene' function

/* Get scene status function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (bool) - true if scene is enabled, false if not.
 */
bool scene::getSceneStatus() const {
    return sceneVisibility;
}  // End of 'scene::getSceneStatus' function

/* Set scene status function.
 * ARGUMENTS:
 *   - flag indicating that the scene is enabled:
 *       bool enableScene_;
 * RETURNS: None.
 */
void scene::setSceneStatus(bool enableScene_) {
    sceneVisibility = enableScene_;
}  // End of 'scene::setSceneStatus' function

// Class destructor
scene::~scene() {
    for (auto &[unitName, unitInstance] : unitsArray) {
        unitInstance->clear();
        delete unitInstance;
    }
    ::std::cout << "Clear scene" << ::std::endl;
}  // End of 'scene::~scene' function
}  // namespace hse