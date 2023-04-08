#include "scene.hpp"

// Project namespace
namespace hse {
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
 *       const ::std::vector<float> &vertexBufferData;
 *   - buffer's format:
 *       const ::std::string &vertexBufferFormat;
 * RETURNS:
 *   (VertexBuffer *) - not-owning pointer of created buffer;
 * NOTE: vertexBufferFormat - use default type or "v3v3v3v2" == vertex
 * position, color, normal, texture coordinate.
 */
VertexBuffer *Scene::createVertexBuffer(
    const ::std::vector<float> &vertexBufferData,
    const ::std::string &vertexBufferFormat
) {
    auto vertexBuffer = new VertexBuffer(vertexBufferData, vertexBufferFormat);
    buffersArray.emplace_back(vertexBuffer);
    return vertexBuffer;
}  // End of 'Scene::createVertexBuffer' function

/* Create index buffer function.
 * ARGUMENTS:
 *   - buffer's data;
 *       const ::std::vector<int> &indexBufferData.
 * RETURNS:
 *   (IndexBuffer *) - not-owning pointer of created buffer;
 */
IndexBuffer *Scene::createIndexBuffer(const ::std::vector<int> &indexBufferData
) {
    auto indexBuffer = new IndexBuffer(indexBufferData);
    buffersArray.emplace_back(indexBuffer);
    return indexBuffer;
}  // End of 'Scene::createIndexBuffer' function

/* Create vertex array function.
 * ARGUMENTS:
 *   - vertex buffer data:
 *       const ::std::vector<float> &vertexBufferData;
 *   - vertex buffer format:
 *       const ::std::string &vertexBufferFormat;
 *   - index buffer data:
 *       const ::std::vector<int> &indexBufferData;
 * RETURNS:
 *   (VertexArray *) - not-owning pointer of created buffer.
 * NOTE: vertexBufferFormat - use default type or "v3v3v3v2" == vertex
 * position, color, normal, texture coordinate.
 */
VertexArray *Scene::createVertexArray(
    const ::std::vector<float> &vertexBufferData,
    const ::std::string &vertexBufferFormat,
    const ::std::vector<int> &indexBufferData
) {
    auto vertexArray =
        new VertexArray(vertexBufferData, vertexBufferFormat, indexBufferData);
    buffersArray.emplace_back(vertexArray);
    return vertexArray;
}  // End of 'Scene::createVertexArray' function

/* Create shader storage buffer function.
 * ARGUMENTS:
 *   - buffer's data:
 *       const ::std::vector<T> &bufferData;
 *   - buffer's binding value:
 *       uint bufferBinding.
 * RETURNS:
 *   (ShaderStorageBuffer *) - not-owning pointer of created buffer.
 */
template <typename T>
ShaderStorageBuffer *Scene::createShaderStorageBuffer(
    const ::std::vector<T> &bufferData,
    uint bufferBinding
) {
    auto shaderStorageBuffer =
        new ShaderStorageBuffer(bufferData, bufferBinding);
    buffersArray.emplace_back(shaderStorageBuffer);
    return shaderStorageBuffer;
}  // End of 'Scene::createShaderStorageBuffer' function

/* Create shader function.
 * ARGUMENTS:
 *   - path to shader's realization (read shader class constructor note)
 *       const ::std::string &shaderPath;
 * RETURNS:
 *   (Shader *) - not-owning pointer of shader program id.
 */
Shader *Scene::createShader(const ::std::string &shaderPath) {
    if (shadersArray[shaderPath]) return shadersArray[shaderPath];
    shadersArray[shaderPath] = new Shader(shaderPath);
    return shadersArray[shaderPath];
}  // End of 'Scene::createShader' function

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
Primitive *Scene::createPrimitive(
    const ::std::string &shaderPath,
    const ::std::vector<float> &vertexBufferData,
    const ::std::string &vertexBufferFormat,
    const ::std::vector<int> &indexBufferData
) {
    primitivesArray.emplace_back(new Primitive(
        createShader(shaderPath)->programId, vertexBufferData,
        vertexBufferFormat, indexBufferData
    ));
    return primitivesArray.back();
}  // End of 'Scene::createPrimitive' function

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
Primitive *Scene::createPrimitive(
    uint shaderProgramId,
    const ::std::vector<float> &vertexBufferData,
    const ::std::string &vertexBufferFormat,
    const ::std::vector<int> &indexBufferData
) {
    primitivesArray.emplace_back(new Primitive(
        shaderProgramId, vertexBufferData, vertexBufferFormat, indexBufferData
    ));
    return primitivesArray.back();
}  // End of 'Scene::createPrimitive' function

/* Create model function.
 * ARGUMENTS:
 *   - path to the model's shader:
 *       const ::std::string &shaderPath;
 *   - models' file name:
 *       const ::std::string &modelFileName;
 * RETURNS:
 *   (model *) - not-owning pointer to the created model.
 */
Model *Scene::createModel(
    const ::std::string &shaderPath,
    const ::std::string &modelFileName
) {
    modelsArray.push_back(
        new Model(createShader(shaderPath)->programId, modelFileName)
    );
    return modelsArray.back();
}  // End of 'Scene::createModel' function

/* Create model function.
 * ARGUMENTS:
 *   - model's shader program id:
 *       uint shaderProgramId;
 *   - models' file name:
 *       const ::std::string &modelFileName;
 * RETURNS:
 *   (model *) - not-owning pointer to the created model.
 */
Model *Scene::createModel(
    uint shaderProgramId,
    const ::std::string &modelFileName
) {
    modelsArray.push_back(new Model(shaderProgramId, modelFileName));
    return modelsArray.back();
}  // End of 'Scene::createModel' function

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
Primitive *Scene::createSpherePrimitive(
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

    primitivesArray.emplace_back(new Primitive(
        createShader("shape")->programId, vertexBufferData, "v3v3v2",
        indexBufferData
    ));
    primitivesArray.back()->transformMatrix = math::matr4::translate(position);

    return primitivesArray.back();
}  // End of 'Scene::createSpherePrimitive' function

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
void Scene::generatePlaneVertexData(
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
}  // End of 'Scene::generatePlaneVertexData' function

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
Primitive *Scene::createPlanePrimitive(
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
    primitivesArray.emplace_back(new Primitive(
        createShader("shape")->programId, vertexBufferData, "v3v3v2",
        indexBufferData
    ));
    primitivesArray.back()->transformMatrix = math::matr4::translate(position);

    return primitivesArray.back();
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
 * RETURNS:
 *   (primitive *) - not-owning pointer to the created cube primitive.
 */
Primitive *Scene::createCubePrimitive(
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
    primitivesArray.emplace_back(new Primitive(
        createShader("shape")->programId, vertexBufferData, "v3v3v2",
        indexBufferData
    ));
    primitivesArray.back()->transformMatrix = math::matr4::translate(position);

    return primitivesArray.back();
}  // End of 'Scene::createCubePrimitive' function

// Class constructor
Scene::Scene()
    : isVisible(true),
      mainCamera(
          ::math::vec3(0),
          ::math::vec3(1, 0, 0),
          ::math::vec3(0, 0, 1),
          ::math::vec3(0, 1, 0),
          400,
          400
      ) {
}  // End of 'Scene::Scene' function

/* Render scene function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void Scene::onRender() const {
    for (auto &modelInstance : modelsArray)
        if (modelInstance->getVisibility()) modelInstance->onRender(mainCamera);
    for (auto &primitiveInstance : primitivesArray)
        if (primitiveInstance->getVisibility())
            primitiveInstance->onRender(mainCamera);
}  // End of 'Scene::onRender' function

/* Delete scene function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void Scene::onDelete() {
    for (auto &bufferInstance : buffersArray)
        delete bufferInstance;
    for (auto &[shaderName, shaderInstance] : shadersArray)
        delete shaderInstance;
    for (auto &primitiveInstance : primitivesArray)
        delete primitiveInstance;
    for (auto &modelInstance : modelsArray)
        delete modelInstance;
}  // End of 'Scene::Clear' function
}  // namespace hse