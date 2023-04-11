#ifndef SCENE_HPP
#define SCENE_HPP

#include "../../../../def.hpp"
#include "../../../../utilities/camera/camera.hpp"
#include "../buffers/buffer.hpp"
#include "../models/model.hpp"
#include "../primitives/primitive.hpp"
#include "../shaders/shader.hpp"

namespace hse {
// Unit class declaration
class Scene {
    // Friend classes
    friend class Render;

    bool isVisible = true;  // Scene's visibility flag
    std::string sceneName;  // Scene's name

    std::map<std::string, std::unique_ptr<Shader>> shadersArray;                  // Scene's shader programs array
    std::vector<std::unique_ptr<Primitive>> primitivesArray;                      // Scene's primitives array
    std::vector<std::unique_ptr<Model>> modelsArray;                              // Scene's models array
    std::vector<std::unique_ptr<VertexBuffer>> vertexBuffersArray;                // Scene's vertex buffers array
    std::vector<std::unique_ptr<IndexBuffer>> indexBuffersArray;                  // Scene's index buffers array
    std::vector<std::unique_ptr<VertexArray>> vertexArraysArray;                  // Scene's VA array
    std::vector<std::unique_ptr<ShaderStorageBuffer>> shaderStorageBuffersArray;  // Scene's SSBO array

public:
    Camera mainCamera;  // Scene's main camera, can be changed by user,
                        // all render works from this camera in each scene

protected:
    // Class default constructor
    explicit Scene() = default;

    /* Class constructor.
     * ARGUMENTS:
     *   - scene's name;
     *       const std::string &sceneName_.
     */
    explicit Scene(std::string sceneName_);

    /* Scene initialization pure-virtual function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual void onCreate() = 0;

    /* Scene update pure-virtual function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual void onUpdate() = 0;

    // Class virtual destructor
    virtual ~Scene() = default;

private:
    /* Render scene function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void onRender() const;

    /* Delete scene function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void onDelete();

public:
    /* Get scene's visibility flag function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (bool) - visibility flag.
     */
    bool getVisibility() const;

    /* Set scene's visibility flag function.
     * ARGUMENTS:
     *   - new flag:
     *       bool isVisible_;
     * RETURNS: None.
     */
    void setVisibility(bool isVisible_);

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
    VertexBuffer *createVertexBuffer(
        const std::vector<float> &vertexBufferData,
        const std::string &vertexBufferFormat = "v3"
    );

    /* Create index buffer function.
     * ARGUMENTS:
     *   - buffer's data;
     *       const std::vector<int> &indexBufferData.
     * RETURNS:
     *   (IndexBuffer *) - not-owning pointer of created buffer;
     */
    IndexBuffer *createIndexBuffer(const std::vector<int> &indexBufferData);

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
    VertexArray *createVertexArray(
        const std::vector<float> &vertexBufferData,
        const std::string &vertexBufferFormat,
        const std::vector<int> &indexBufferData
    );

    /* Create shader storage buffer function.
     * ARGUMENTS:
     *   - buffer's data:
     *       const std::vector<T> &bufferData;
     *   - buffer's binding value:
     *       uint bufferBinding.
     * RETURNS:
     *   (ShaderStorageBuffer *) - not-owning pointer of created buffer.
     */
    template <typename T>
    ShaderStorageBuffer *createShaderStorageBuffer(const std::vector<T> &bufferData, uint bufferBinding) {
        shaderStorageBuffersArray.push_back(std::make_unique<ShaderStorageBuffer>(bufferData, bufferBinding));
        return shaderStorageBuffersArray.back().get();
    }  // End of 'createShaderStorageBuffer' function

    /* Create shader function.
     * ARGUMENTS:
     *   - path to shader's realization (read shader class constructor note)
     *       const std::string &shaderPath;
     * RETURNS:
     *   (Shader *) - not-owning pointer of shader program id.
     */
    Shader *createShader(const std::string &shaderPath);

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
    Primitive *createPrimitive(
        const std::string &shaderPath,
        const std::vector<float> &vertexBufferData,
        const std::string &vertexBufferFormat = "v3v3",
        const std::vector<int> &indexBufferData = std::vector<int>()
    );

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
     *   (Primitive *) - not-owning pointer to the created primitive.
     */
    Primitive *createPrimitive(
        uint shaderProgramId,
        const std::vector<float> &vertexBufferData,
        const std::string &vertexBufferFormat,
        const std::vector<int> &indexBufferData
    );

    /* Create model function.
     * ARGUMENTS:
     *   - path to the model's shader:
     *       const std::string &shaderPath;
     *   - models' file name:
     *       const std::string &modelFileName;
     * RETURNS:
     *   (Model *) - not-owning pointer to the created model.
     */
    Model *createModel(const std::string &shaderPath, const std::string &modelFileName);

    /* Create model function.
     * ARGUMENTS:
     *   - model's shader program id:
     *       uint shaderProgramId;
     *   - models' file name:
     *       const std::string &modelFileName;
     * RETURNS:
     *   (Model *) - not-owning pointer to the created model.
     */
    Model *createModel(uint shaderProgramId, const std::string &modelFileName);

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
    Primitive *createSpherePrimitive(
        float radius,
        const math::vec3 &position,
        const math::vec3 &color = math::vec3(0.6),
        int stacks = 20,
        int slices = 20
    );

private:
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
    static void generatePlaneVertexData(
        std::vector<float> &vertexBufferData,
        std::vector<int> &indexBufferData,
        const math::vec3 &width,
        const math::vec3 &height,
        const math::vec3 &normal,
        const math::vec3 &position,
        const math::vec3 &color,
        int indexesOffset = 0
    );

public:
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
    Primitive *createPlanePrimitive(
        float width,
        float height,
        const math::vec3 &position,
        const math::vec3 &color = math::vec3(0.6)
    );

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
    Primitive *createCubePrimitive(
        float length,
        float width,
        float height,
        const math::vec3 &position,
        const math::vec3 &color = math::vec3(0.6)
    );
};  // End of 'Scene' class
}  // namespace hse

#endif  // SCENE_HPP
