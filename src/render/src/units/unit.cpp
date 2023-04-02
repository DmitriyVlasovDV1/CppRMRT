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
 * RETURNS:
 *   (uint) - created buffer id;
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
 * RETURNS:
 *   (uint) - created buffer id;
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
 * RETURNS:
 *   (uint) - created buffer id.
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
 * RETURNS:
 *   (uint) - created buffer id.
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
}  // End of 'unit::createShader' function

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
}  // End of 'unit::createShader' function

/* Create primitive function.
 * ARGUMENTS:
 *   - path to primitive's shader:
 *       const ::std::string &shaderPath;
 *   - vertex buffer data:
 *       const ::std::vector<float> &vertexBufferData;
 *   - vertex buffer format:
 *       const ::std::string &vertexBufferFormat;
 *   - index buffer data:
 *       const ::std::vector<int> &indexBufferData;
 * RETURNS:
 *   (primitive *) - not-owning pointer to the created primitive;
 * NOTE: vertexBufferFormat - use default type or "v3v3v3v2" == vertex
 * position, color, normal, texture coordinate.
 */
primitive *unit::createPrimitive(
    const ::std::string &shaderPath,
    const ::std::vector<float> &vertexBufferData,
    const ::std::string &vertexBufferFormat,
    const ::std::vector<int> &indexBufferData
) {
    primitivesArray.emplace_back(new primitive(
        createShader(shaderPath), vertexBufferData, vertexBufferFormat,
        indexBufferData
    ));
    return primitivesArray.back();
}  // End of 'unit::createPrimitive' function

/* Create primitive function.
 * ARGUMENTS:
 *   - shader program id:
 *       uint shaderProgramId;
 *   - vertex buffer data:
 *       const ::std::vector<float> &vertexBufferData;
 *   - vertex buffer format:
 *       const ::std::string &vertexBufferFormat;
 *   - index buffer data:
 *       const ::std::vector<int> &indexBufferData;
 * RETURNS:
 *   (primitive *) - not-owning pointer to the created primitive;
 * NOTE: vertexBufferFormat - use default type or "v3v3v3v2" == vertex
 * position, color, normal, texture coordinate.
 */
primitive *unit::createPrimitive(
    uint shaderProgramId,
    const ::std::vector<float> &vertexBufferData,
    const ::std::string &vertexBufferFormat,
    const ::std::vector<int> &indexBufferData
) {
    primitivesArray.emplace_back(new primitive(
        shaderProgramId, vertexBufferData, vertexBufferFormat, indexBufferData
    ));
    return primitivesArray.back();
}  // End of 'unit::createPrimitive' function

/* Create model function.
 * ARGUMENTS:
 *   - path to the model's shader:
 *       const ::std::string &shaderPath;
 *   - models' file name:
 *       const ::std::string &modelFileName;
 * RETURNS:
 *   (model *) - not-owning pointer to the created model.
 */
model *unit::createModel(
    const ::std::string &shaderPath,
    const ::std::string &modelFileName
) {
    modelsArray.push_back(new model(createShader(shaderPath), modelFileName));
    return modelsArray.back();
}  // End of 'unit::createModel' function

/* Create model function.
 * ARGUMENTS:
 *   - model's shader program id:
 *       uint shaderProgramId;
 *   - models' file name:
 *       const ::std::string &modelFileName;
 * RETURNS:
 *   (model *) - not-owning pointer to the created model.
 */
model *unit::createModel(
    uint shaderProgramId,
    const ::std::string &modelFileName
) {
    modelsArray.push_back(new model(shaderProgramId, modelFileName));
    return modelsArray.back();
}  // End of 'unit::createModel' function

/* Create sphere primitive function.
 * ARGUMENTS:
 *   - sphere's radius:
 *       float radius;
 *   - sphere's position:
 *       const math::vec3 &position;
 *   - sphere's stacks:
 *       int stacks;
 *   - sphere's slices:
 *       int slices;
 * RETURNS:
 *   (primitive *) - not-owning pointer to the created sphere primitive;
 */
primitive *unit::createSpherePrimitive(
    float radius,
    const math::vec3 &position,
    int stacks,
    int slices
) {
    std::vector<float> vertexBufferData;
    std::vector<int> indexBufferData;

    slices++;
    vertexBufferData.reserve((3 + 3 + 2) * stacks * slices);
    indexBufferData.reserve(stacks * slices);

    vertexBufferData.push_back(0);
    vertexBufferData.push_back(radius);
    vertexBufferData.push_back(0);
    vertexBufferData.push_back(0);
    vertexBufferData.push_back(1);
    vertexBufferData.push_back(0);
    vertexBufferData.push_back(0.5);
    vertexBufferData.push_back(0);

    int x, y;
    float alpha = 0, beta;

    for (y = 0; y < slices;
         y++, alpha += math::PI / static_cast<float>(slices)) {
        for (x = 0, beta = 0; x < stacks;
             x++, beta += 2 * math::PI / static_cast<float>(stacks)) {
            bool isND = y<(slices - 1), isNU = y> 0;
            int x0 = x, x1 = (x + 1) % stacks,
                y0 = (isNU ? (y - 1) * stacks + 1 : 0), y1 = y * stacks + 1;

            if (isNU) {
                math::vec3 pointNormal = math::vec3(
                    cos(beta) * sin(alpha), cos(alpha), sin(beta) * sin(alpha)
                );
                math::vec3 pointPosition = pointNormal * radius;
                vertexBufferData.push_back(pointPosition.x);
                vertexBufferData.push_back(pointPosition.y);
                vertexBufferData.push_back(pointPosition.z);
                vertexBufferData.push_back(pointNormal.x);
                vertexBufferData.push_back(pointNormal.y);
                vertexBufferData.push_back(pointNormal.z);
                vertexBufferData.push_back(
                    static_cast<float>(x) / (static_cast<float>(stacks) - 1.0f)
                );
                vertexBufferData.push_back(
                    (static_cast<float>(y) + 1.0f) / static_cast<float>(slices)
                );

                indexBufferData.push_back(y0 + x0 * isNU);
                indexBufferData.push_back(y0 + x1 * isNU);
                indexBufferData.push_back(y1 + x0 * isND);
            }
            indexBufferData.push_back(y1 + x0 * isND);
            indexBufferData.push_back(y0 + x1 * isNU);
            indexBufferData.push_back(y1 + x1 * isND);
        }
    }

    vertexBufferData.push_back(0);
    vertexBufferData.push_back(-radius);
    vertexBufferData.push_back(0);
    vertexBufferData.push_back(0);
    vertexBufferData.push_back(-1);
    vertexBufferData.push_back(0);
    vertexBufferData.push_back(0.5);
    vertexBufferData.push_back(1);

    primitivesArray.emplace_back(new primitive(
        createShader("shape"), vertexBufferData, "v3v3v2", indexBufferData
    ));
    primitivesArray.back()->transformMatrix = math::matr4::translate(position);

    return primitivesArray.back();
}  // End of 'unit::createSpherePrimitive' function

/* Generate vertexes for plane primitive function.
 * ARGUMENTS:
 *   - vertex buffer data array:
 *       ::std::vector<float> &vertexBufferData;
 *   - index buffer data array:
 *       ::std::vector<int> &indexBufferData;
 *   - plane's width:
 *       const math::vec3 &width;
 *   - plane's height:
 *       const math::vec3 &height;
 *   - plane's normal:
 *       const math::vec3 &normal;
 *   - plane's position:
 *       const math::vec3 &position;
 *   - indexes offset (for cube primitive):
 *       int indexesOffset;
 * RETURNS: None.
 */
void unit::generatePlaneVertexData(
    ::std::vector<float> &vertexBufferData,
    ::std::vector<int> &indexBufferData,
    const math::vec3 &width,
    const math::vec3 &height,
    const math::vec3 &normal,
    const math::vec3 &position,
    int indexesOffset
) {
    math::vec3 pointPosition(0);
    pointPosition = width / 2 + height / 2 + position;
    vertexBufferData.push_back(pointPosition.x);
    vertexBufferData.push_back(pointPosition.y);
    vertexBufferData.push_back(pointPosition.z);
    vertexBufferData.push_back(normal.x);
    vertexBufferData.push_back(normal.y);
    vertexBufferData.push_back(normal.z);
    vertexBufferData.push_back(1);
    vertexBufferData.push_back(1);
    pointPosition = -width / 2 + height / 2 + position;
    vertexBufferData.push_back(pointPosition.x);
    vertexBufferData.push_back(pointPosition.y);
    vertexBufferData.push_back(pointPosition.z);
    vertexBufferData.push_back(normal.x);
    vertexBufferData.push_back(normal.y);
    vertexBufferData.push_back(normal.z);
    vertexBufferData.push_back(0);
    vertexBufferData.push_back(1);
    pointPosition = -width / 2 - height / 2 + position;
    vertexBufferData.push_back(pointPosition.x);
    vertexBufferData.push_back(pointPosition.y);
    vertexBufferData.push_back(pointPosition.z);
    vertexBufferData.push_back(normal.x);
    vertexBufferData.push_back(normal.y);
    vertexBufferData.push_back(normal.z);
    vertexBufferData.push_back(0);
    vertexBufferData.push_back(0);
    pointPosition = width / 2 - height / 2 + position;
    vertexBufferData.push_back(pointPosition.x);
    vertexBufferData.push_back(pointPosition.y);
    vertexBufferData.push_back(pointPosition.z);
    vertexBufferData.push_back(normal.x);
    vertexBufferData.push_back(normal.y);
    vertexBufferData.push_back(normal.z);
    vertexBufferData.push_back(1);
    vertexBufferData.push_back(0);

    indexBufferData.push_back(indexesOffset + 0);
    indexBufferData.push_back(indexesOffset + 1);
    indexBufferData.push_back(indexesOffset + 2);
    indexBufferData.push_back(indexesOffset + 2);
    indexBufferData.push_back(indexesOffset + 0);
    indexBufferData.push_back(indexesOffset + 3);
}  // End of 'unit::generatePlaneVertexData' function

/* Create plane primitive function.
 * ARGUMENTS:
 *   - plane's width:
 *       float width;
 *   - plane's height:
 *       float height;
 *   - plane's position:
 *       const math::vec3 &position;
 * RETURNS:
 *   (primitive *) - not-owning pointer to the created plane primitive.
 */
primitive *unit::createPlanePrimitive(
    float width,
    float height,
    const math::vec3 &position
) {
    ::std::vector<float> vertexBufferData;
    ::std::vector<int> indexBufferData;

    generatePlaneVertexData(
        vertexBufferData, indexBufferData, math::vec3(width, 0, 0),
        math::vec3(0, 0, height), math::vec3(0), math::vec3(0, 1, 0)
    );
    primitivesArray.emplace_back(new primitive(
        createShader("shape"), vertexBufferData, "v3v3v2", indexBufferData
    ));
    primitivesArray.back()->transformMatrix = math::matr4::translate(position);

    return primitivesArray.back();
}  // End of 'unit::createPlanePrimitive' function

/* Create cube primitive function.
 * ARGUMENTS:
 *   - cube's length:
 *       float length;
 *   - cube's width:
 *       float width;
 *   - cube's height:
 *       float height;
 *   - cube's position:
 *       const math::vec3 &position;
 * RETURNS:
 *   (primitive *) - not-owning pointer to the created cube primitive.
 */
primitive *unit::createCubePrimitive(
    float length,
    float width,
    float height,
    const math::vec3 &position
) {
    ::std::vector<float> vertexBufferData;
    ::std::vector<int> indexBufferData;

    generatePlaneVertexData(
        vertexBufferData, indexBufferData, math::vec3(length, 0, 0),
        math::vec3(0, 0, -width), math::vec3(0, 1, 0),
        math::vec3(0, height / 2, 0), 0
    );
    indexBufferData.push_back(-1);
    generatePlaneVertexData(
        vertexBufferData, indexBufferData, math::vec3(length, 0, 0),
        math::vec3(0, 0, width), math::vec3(0, -1, 0),
        math::vec3(0, -height / 2, 0), 4
    );
    indexBufferData.push_back(-1);
    generatePlaneVertexData(
        vertexBufferData, indexBufferData, math::vec3(length, 0, 0),
        math::vec3(0, height, 0), math::vec3(0, 0, 1),
        math::vec3(0, 0, width / 2), 8
    );
    indexBufferData.push_back(-1);
    generatePlaneVertexData(
        vertexBufferData, indexBufferData, math::vec3(-length, 0, 0),
        math::vec3(0, height, 0), math::vec3(0, 0, -1),
        math::vec3(0, 0, -width / 2), 12
    );
    indexBufferData.push_back(-1);
    generatePlaneVertexData(
        vertexBufferData, indexBufferData, math::vec3(0, 0, -width),
        math::vec3(0, height, 0), math::vec3(1, 0, 0),
        math::vec3(length / 2, 0, 0), 16
    );
    indexBufferData.push_back(-1);
    generatePlaneVertexData(
        vertexBufferData, indexBufferData, math::vec3(0, 0, width),
        math::vec3(0, height, 0), math::vec3(-1, 0, 0),
        math::vec3(-length / 2, 0, 0), 20
    );
    primitivesArray.emplace_back(new primitive(
        createShader("shape"), vertexBufferData, "v3v3v2", indexBufferData
    ));
    primitivesArray.back()->transformMatrix = math::matr4::translate(position);

    return primitivesArray.back();
}  // End of 'unit::createCubePrimitive' function

// Class constructor
unit::unit()
    : isInitialized(false),
      isVisible(true),
      mainCamera(
          ::math::vec3(0),
          ::math::vec3(1, 0, 0),
          ::math::vec3(0, 0, 1),
          ::math::vec3(0, 1, 0),
          400,
          400
      ) {
}  // End of 'unit::unit' function

/* Render unit function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void unit::render() const {
    for (auto &modelInstance : modelsArray)
        if (modelInstance->getVisibility()) modelInstance->render(mainCamera);
    for (auto &primitiveInstance : primitivesArray)
        if (primitiveInstance->getVisibility())
            primitiveInstance->render(mainCamera);
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
    for (auto &modelInstance : modelsArray)
        delete modelInstance;
}  // End of 'unit::clear' function
}  // namespace hse