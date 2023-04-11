#include "scene.hpp"

#include <utility>

// Project namespace
namespace hse {
/* Class constructor.
 * ARGUMENTS:
 *   - scene's name;
 *       const std::string &sceneName_.
 */
Scene::Scene(std::string sceneName_)
    : isVisible(true),
      mainCamera(math::vec3(0), math::vec3(1, 0, 0), math::vec3(0, 0, 1), math::vec3(0, 1, 0), 400, 400),
      sceneName(std::move(sceneName_)) {
}  // End of 'Scene::Scene' function

/* Render scene function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void Scene::onRender() const {
    for (auto &modelInstance : modelsArray)
        if (modelInstance->getVisibility()) modelInstance->onRender(mainCamera);
    for (auto &primitiveInstance : primitivesArray)
        if (primitiveInstance->getVisibility()) primitiveInstance->onRender(mainCamera);
}  // End of 'Scene::onRender' function

/* Delete scene function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void Scene::onDelete() {
    for (auto &bufferInstance : vertexBuffersArray)
        bufferInstance.reset();
    for (auto &bufferInstance : indexBuffersArray)
        bufferInstance.reset();
    for (auto &bufferInstance : vertexArraysArray)
        bufferInstance.reset();
    for (auto &bufferInstance : shaderStorageBuffersArray)
        bufferInstance.reset();
    for (auto &[shaderName, shaderInstance] : shadersArray)
        shaderInstance.reset();
    for (auto &primitiveInstance : primitivesArray)
        primitiveInstance.reset();
    for (auto &modelInstance : modelsArray)
        modelInstance.reset();
}  // End of 'Scene::onDelete' function

/* Get scene's visibility flag function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (bool) - visibility flag.
 */
bool Scene::getVisibility() const {
    return isVisible;
}  // End of 'Scene::getVisibility' function

/* Set scene's visibility flag function.
 * ARGUMENTS:
 *   - new flag:
 *       bool isVisible_;
 * RETURNS: None.
 */
void Scene::setVisibility(bool isVisible_) {
    isVisible = isVisible_;
}  // End of 'Scene::setVisibility' function

/* Create vertex buffer function.
 * ARGUMENTS:
 *   - buffer's data:
 *       const std::vector<float> &vertexBufferData;
 *   - buffer's format:
 *       const std::string &vertexBufferFormat;
 * RETURNS:
 *   (VertexBuffer *) - not-owning pointer of created buffer;
 * NOTE: vertexBufferFormat - use default type or "v3v3v3v2" == vertex
 * position, color, normal, texture coordinate.
 */
VertexBuffer *Scene::createVertexBuffer(
    const std::vector<float> &vertexBufferData,
    const std::string &vertexBufferFormat
) {
    vertexBuffersArray.emplace_back(std::make_unique<VertexBuffer>(vertexBufferData, vertexBufferFormat));
    return vertexBuffersArray.back().get();
}  // End of 'Scene::createVertexBuffer' function

/* Create index buffer function.
 * ARGUMENTS:
 *   - buffer's data;
 *       const std::vector<int> &indexBufferData.
 * RETURNS:
 *   (IndexBuffer *) - not-owning pointer of created buffer;
 */
IndexBuffer *Scene::createIndexBuffer(const std::vector<int> &indexBufferData) {
    indexBuffersArray.emplace_back(std::make_unique<IndexBuffer>(indexBufferData));
    return indexBuffersArray.back().get();
}  // End of 'Scene::createIndexBuffer' function

/* Create vertex array function.
 * ARGUMENTS:
 *   - vertex buffer data:
 *       const std::vector<float> &vertexBufferData;
 *   - vertex buffer format:
 *       const std::string &vertexBufferFormat;
 *   - index buffer data:
 *       const std::vector<int> &indexBufferData;
 * RETURNS:
 *   (VertexArray *) - not-owning pointer of created buffer.
 * NOTE: vertexBufferFormat - use default type or "v3v3v3v2" == vertex
 * position, color, normal, texture coordinate.
 */
VertexArray *Scene::createVertexArray(
    const std::vector<float> &vertexBufferData,
    const std::string &vertexBufferFormat,
    const std::vector<int> &indexBufferData
) {
    vertexArraysArray.emplace_back(std::make_unique<VertexArray>(vertexBufferData, vertexBufferFormat, indexBufferData)
    );
    return vertexArraysArray.back().get();
}  // End of 'Scene::createVertexArray' function

/* Create shader function.
 * ARGUMENTS:
 *   - path to shader's realization (read shader class constructor note)
 *       const std::string &shaderPath;
 * RETURNS:
 *   (Shader *) - not-owning pointer of shader program id.
 */
Shader *Scene::createShader(const std::string &shaderPath) {
    if (shadersArray[shaderPath]) return shadersArray[shaderPath].get();
    shadersArray[shaderPath] = std::make_unique<Shader>(shaderPath);
    return shadersArray[shaderPath].get();
}  // End of 'Scene::createShader' function

/* Create primitive function.
 * ARGUMENTS:
 *   - path to primitive's shader:
 *       const std::string &shaderPath;
 *   - vertex buffer data:
 *       const std::vector<float> &vertexBufferData;
 *   - vertex buffer format:
 *       const std::string &vertexBufferFormat;
 *   - index buffer data:
 *       const std::vector<int> &indexBufferData;
 * RETURNS:
 *   (Primitive *) - not-owning pointer to the created primitive;
 * NOTE: vertexBufferFormat - use default type or "v3v3v3v2" == vertex
 * position, color, normal, texture coordinate.
 */
Primitive *Scene::createPrimitive(
    const std::string &shaderPath,
    const std::vector<float> &vertexBufferData,
    const std::string &vertexBufferFormat,
    const std::vector<int> &indexBufferData
) {
    primitivesArray.emplace_back(std::make_unique<Primitive>(
        createShader(shaderPath)->getShaderProgramId(), vertexBufferData, vertexBufferFormat, indexBufferData
    ));
    return primitivesArray.back().get();
}  // End of 'Scene::createPrimitive' function

/* Create primitive function.
 * ARGUMENTS:
 *   - shader program id:
 *       uint shaderProgramId;
 *   - vertex buffer data:
 *       const std::vector<float> &vertexBufferData;
 *   - vertex buffer format:
 *       const std::string &vertexBufferFormat;
 *   - index buffer data:
 *       const std::vector<int> &indexBufferData;
 * RETURNS:
 *   (Primitive *) - not-owning pointer to the created primitive;
 * NOTE: vertexBufferFormat - use default type or "v3v3v3v2" == vertex
 * position, color, normal, texture coordinate.
 */
Primitive *Scene::createPrimitive(
    uint shaderProgramId,
    const std::vector<float> &vertexBufferData,
    const std::string &vertexBufferFormat,
    const std::vector<int> &indexBufferData
) {
    primitivesArray.emplace_back(
        std::make_unique<Primitive>(shaderProgramId, vertexBufferData, vertexBufferFormat, indexBufferData)
    );
    return primitivesArray.back().get();
}  // End of 'Scene::createPrimitive' function

/* Create model function.
 * ARGUMENTS:
 *   - path to the model's shader:
 *       const std::string &shaderPath;
 *   - models' file name:
 *       const std::string &modelFileName;
 * RETURNS:
 *   (Model *) - not-owning pointer to the created model.
 */
Model *Scene::createModel(const std::string &shaderPath, const std::string &modelFileName) {
    modelsArray.push_back(std::make_unique<Model>(createShader(shaderPath)->getShaderProgramId(), modelFileName));
    return modelsArray.back().get();
}  // End of 'Scene::createModel' function

/* Create model function.
 * ARGUMENTS:
 *   - model's shader program id:
 *       uint shaderProgramId;
 *   - models' file name:
 *       const std::string &modelFileName;
 * RETURNS:
 *   (model *) - not-owning pointer to the created model.
 */
Model *Scene::createModel(uint shaderProgramId, const std::string &modelFileName) {
    modelsArray.push_back(std::make_unique<Model>(shaderProgramId, modelFileName));
    return modelsArray.back().get();
}  // End of 'Scene::createModel' function

/* Create sphere primitive function.
 * ARGUMENTS:
 *   - sphere's radius:
 *       float radius;
 *   - sphere's position:
 *       const math::vec3 &position;
 *   - sphere's color:
 *       const math::vec3 &color;
 *   - sphere's stacks:
 *       int stacks;
 *   - sphere's slices:
 *       int slices;
 * RETURNS:
 *   (Primitive *) - not-owning pointer to the created sphere primitive.
 */
Primitive *Scene::createSpherePrimitive(
    float radius,
    const math::vec3 &position,
    const math::vec3 &color,
    int stacks,
    int slices
) {
    std::vector<float> vertexBufferData;
    std::vector<int> indexBufferData;

    slices++;
    vertexBufferData.reserve((3 + 3 + 2) * stacks * slices);
    indexBufferData.reserve(stacks * slices);

    vertexBufferData.push_back(color.x);
    vertexBufferData.push_back(color.y);
    vertexBufferData.push_back(color.z);
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

    for (y = 0; y < slices; y++, alpha += math::PI / static_cast<float>(slices)) {
        for (x = 0, beta = 0; x < stacks; x++, beta += 2 * math::PI / static_cast<float>(stacks)) {
            bool isND = y<(slices - 1), isNU = y> 0;
            int x0 = x, x1 = (x + 1) % stacks, y0 = (isNU ? (y - 1) * stacks + 1 : 0), y1 = y * stacks + 1;

            if (isNU) {
                math::vec3 pointNormal = math::vec3(cos(beta) * sin(alpha), cos(alpha), sin(beta) * sin(alpha));
                math::vec3 pointPosition = pointNormal * radius;
                vertexBufferData.push_back(color.x);
                vertexBufferData.push_back(color.y);
                vertexBufferData.push_back(color.z);
                vertexBufferData.push_back(pointPosition.x);
                vertexBufferData.push_back(pointPosition.y);
                vertexBufferData.push_back(pointPosition.z);
                vertexBufferData.push_back(pointNormal.x);
                vertexBufferData.push_back(pointNormal.y);
                vertexBufferData.push_back(pointNormal.z);
                vertexBufferData.push_back(static_cast<float>(x) / (static_cast<float>(stacks) - 1.0f));
                vertexBufferData.push_back((static_cast<float>(y) + 1.0f) / static_cast<float>(slices));

                indexBufferData.push_back(y0 + x0 * isNU);
                indexBufferData.push_back(y0 + x1 * isNU);
                indexBufferData.push_back(y1 + x0 * isND);
            }
            indexBufferData.push_back(y1 + x0 * isND);
            indexBufferData.push_back(y0 + x1 * isNU);
            indexBufferData.push_back(y1 + x1 * isND);
        }
    }

    vertexBufferData.push_back(color.x);
    vertexBufferData.push_back(color.y);
    vertexBufferData.push_back(color.z);
    vertexBufferData.push_back(0);
    vertexBufferData.push_back(-radius);
    vertexBufferData.push_back(0);
    vertexBufferData.push_back(0);
    vertexBufferData.push_back(-1);
    vertexBufferData.push_back(0);
    vertexBufferData.push_back(0.5);
    vertexBufferData.push_back(1);

    primitivesArray.emplace_back(std::make_unique<Primitive>(
        createShader("shape")->getShaderProgramId(), vertexBufferData, "v3v3v3v2", indexBufferData
    ));
    primitivesArray.back()->transformMatrix = math::matr4::translate(position);

    return primitivesArray.back().get();
}  // End of 'Scene::createSpherePrimitive' function

/* Generate vertexes for plane primitive function.
 * ARGUMENTS:
 *   - vertex buffer data array:
 *       std::vector<float> &vertexBufferData;
 *   - index buffer data array:
 *       std::vector<int> &indexBufferData;
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
 *   - plane's color:
 *       const math::vec3 &color;
 * RETURNS: None.
 */
void Scene::generatePlaneVertexData(
    std::vector<float> &vertexBufferData,
    std::vector<int> &indexBufferData,
    const math::vec3 &width,
    const math::vec3 &height,
    const math::vec3 &normal,
    const math::vec3 &position,
    const math::vec3 &color,
    int indexesOffset
) {
    math::vec3 pointPosition(0);
    pointPosition = width / 2 + height / 2 + position;
    vertexBufferData.push_back(color.x);
    vertexBufferData.push_back(color.y);
    vertexBufferData.push_back(color.z);
    vertexBufferData.push_back(pointPosition.x);
    vertexBufferData.push_back(pointPosition.y);
    vertexBufferData.push_back(pointPosition.z);
    vertexBufferData.push_back(normal.x);
    vertexBufferData.push_back(normal.y);
    vertexBufferData.push_back(normal.z);
    vertexBufferData.push_back(1);
    vertexBufferData.push_back(1);
    pointPosition = -width / 2 + height / 2 + position;
    vertexBufferData.push_back(color.x);
    vertexBufferData.push_back(color.y);
    vertexBufferData.push_back(color.z);
    vertexBufferData.push_back(pointPosition.x);
    vertexBufferData.push_back(pointPosition.y);
    vertexBufferData.push_back(pointPosition.z);
    vertexBufferData.push_back(normal.x);
    vertexBufferData.push_back(normal.y);
    vertexBufferData.push_back(normal.z);
    vertexBufferData.push_back(0);
    vertexBufferData.push_back(1);
    pointPosition = -width / 2 - height / 2 + position;
    vertexBufferData.push_back(color.x);
    vertexBufferData.push_back(color.y);
    vertexBufferData.push_back(color.z);
    vertexBufferData.push_back(pointPosition.x);
    vertexBufferData.push_back(pointPosition.y);
    vertexBufferData.push_back(pointPosition.z);
    vertexBufferData.push_back(normal.x);
    vertexBufferData.push_back(normal.y);
    vertexBufferData.push_back(normal.z);
    vertexBufferData.push_back(0);
    vertexBufferData.push_back(0);
    pointPosition = width / 2 - height / 2 + position;
    vertexBufferData.push_back(color.x);
    vertexBufferData.push_back(color.y);
    vertexBufferData.push_back(color.z);
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
}  // End of 'Scene::generatePlaneVertexData' function

/* Create plane primitive function.
 * ARGUMENTS:
 *   - plane's width:
 *       float width;
 *   - plane's height:
 *       float height;
 *   - plane's position:
 *       const math::vec3 &position;
 *   - plane's color:
 *       const math::vec3 &color;
 * RETURNS:
 *   (Primitive *) - not-owning pointer to the created plane primitive.
 */
Primitive *Scene::createPlanePrimitive(float width, float height, const math::vec3 &position, const math::vec3 &color) {
    std::vector<float> vertexBufferData;
    std::vector<int> indexBufferData;

    generatePlaneVertexData(
        vertexBufferData, indexBufferData, math::vec3(width, 0, 0), math::vec3(0, 0, height), math::vec3(0, 1, 0),
        math::vec3(0), color, 0
    );
    primitivesArray.emplace_back(std::make_unique<Primitive>(
        createShader("shape")->getShaderProgramId(), vertexBufferData, "v3v3v3v2", indexBufferData
    ));
    primitivesArray.back()->transformMatrix = math::matr4::translate(position);

    return primitivesArray.back().get();
}  // End of 'Scene::createPlanePrimitive' function

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
 *   - cube's color:
 *       const math::vec3 &color;
 * RETURNS:
 *   (Primitive *) - not-owning pointer to the created cube primitive.
 */
Primitive *Scene::createCubePrimitive(
    float length,
    float width,
    float height,
    const math::vec3 &position,
    const math::vec3 &color
) {
    std::vector<float> vertexBufferData;
    std::vector<int> indexBufferData;

    generatePlaneVertexData(
        vertexBufferData, indexBufferData, math::vec3(length, 0, 0), math::vec3(0, 0, -width), math::vec3(0, 1, 0),
        math::vec3(0, height / 2, 0), color, 0
    );
    indexBufferData.push_back(-1);
    generatePlaneVertexData(
        vertexBufferData, indexBufferData, math::vec3(length, 0, 0), math::vec3(0, 0, width), math::vec3(0, -1, 0),
        math::vec3(0, -height / 2, 0), color, 4
    );
    indexBufferData.push_back(-1);
    generatePlaneVertexData(
        vertexBufferData, indexBufferData, math::vec3(length, 0, 0), math::vec3(0, height, 0), math::vec3(0, 0, 1),
        math::vec3(0, 0, width / 2), color, 8
    );
    indexBufferData.push_back(-1);
    generatePlaneVertexData(
        vertexBufferData, indexBufferData, math::vec3(-length, 0, 0), math::vec3(0, height, 0), math::vec3(0, 0, -1),
        math::vec3(0, 0, -width / 2), color, 12
    );
    indexBufferData.push_back(-1);
    generatePlaneVertexData(
        vertexBufferData, indexBufferData, math::vec3(0, 0, -width), math::vec3(0, height, 0), math::vec3(1, 0, 0),
        math::vec3(length / 2, 0, 0), color, 16
    );
    indexBufferData.push_back(-1);
    generatePlaneVertexData(
        vertexBufferData, indexBufferData, math::vec3(0, 0, width), math::vec3(0, height, 0), math::vec3(-1, 0, 0),
        math::vec3(-length / 2, 0, 0), color, 20
    );
    primitivesArray.emplace_back(std::make_unique<Primitive>(
        createShader("shape")->getShaderProgramId(), vertexBufferData, "v3v3v3v2", indexBufferData
    ));
    primitivesArray.back()->transformMatrix = math::matr4::translate(position);

    return primitivesArray.back().get();
}  // End of 'Scene::createCubePrimitive' function
}  // namespace hse