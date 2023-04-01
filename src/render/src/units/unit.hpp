#ifndef UNIT_HPP
#define UNIT_HPP

#include "../../../def.hpp"
#include "../../../utilities/cameras/camera.hpp"
#include "../buffers/buffer.hpp"
#include "../primitives/primitive.hpp"
#include "../shaders/shader.hpp"

namespace hse {
// Unit class declaration
class unit {
    // Friend classes
    friend class render;

    bool isVisible;      // Unit's visibility flag
    bool isInitialized;  // We can have a lot of copies of one unit,
                         // but we want to initialize it only once
    ::std::map<::std::string, shader *> shadersArray;  // Unit's shader programs
                                                       // array
    ::std::vector<primitive *> primitivesArray;  // Unit's primitives array
    ::std::vector<buffer *> buffersArray;        // Unit's buffers array

protected:
    camera mainCamera;  // Unit's main camera (can be changed, but all render
                        // works from this camera in each unit)

public:
    /* Get unit's visibility flag function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (bool) - visibility flag.
     */
    bool getVisibility() const;

    /* Set unit's visibility flag function.
     * ARGUMENTS:
     *   - new flag:
     *       bool isVisible_;
     * RETURNS: None.
     */
    void setVisibility(bool isVisible_);

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
    uint createVertexBuffer(
        const ::std::vector<float> &vertexBufferData,
        const ::std::string &vertexBufferFormat = "v3"
    );

    /* Create index buffer function.
     * ARGUMENTS:
     *   - buffer's data;
     *       const ::std::vector<int> &indexBufferData.
     * RETURNS:
     *   (uint) - created buffer id;
     */
    uint createIndexBuffer(const ::std::vector<int> &indexBufferData);

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
    uint createVertexArray(
        const ::std::vector<float> &vertexBufferData,
        const ::std::string &vertexBufferFormat,
        const ::std::vector<int> &indexBufferData
    );

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
    uint createShaderStorageBuffer(
        const ::std::vector<T> &bufferData,
        uint bufferBinding
    );

    /* Create shader function.
     * ARGUMENTS:
     *   - path to shader's realization (read shader class constructor note)
     *       const ::std::string &shaderPath;
     * RETURNS:
     *   (uint) - shader program id.
     */
    uint createShader(const ::std::string &shaderPath);

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
    uint createShader(
        const ::std::string &vertexShaderSource,
        const ::std::string &fragmentShaderSource,
        const ::std::string &shaderPath = ""
    );

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
    primitive *createPrimitive(
        const ::std::string &shaderPath,
        const ::std::vector<float> &vertexBufferData,
        const ::std::string &vertexBufferFormat = "v3v3",
        const ::std::vector<int> &indexBufferData = ::std::vector<int>()
    );

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
     *   (primitive *) - not-owning pointer to the created primitive.
     */
    primitive *createPrimitive(
        uint shaderProgramId,
        const ::std::vector<float> &vertexBufferData,
        const ::std::string &vertexBufferFormat,
        const ::std::vector<int> &indexBufferData
    );

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
     *   (primitive *) - not-owning pointer to the created sphere primitive.
     */
    primitive *createSpherePrimitive(
        float radius,
        const math::vec3 &position,
        int stacks = 20,
        int slices = 20
    );

private:
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
    static void generatePlaneVertexData(
        ::std::vector<float> &vertexBufferData,
        ::std::vector<int> &indexBufferData,
        const math::vec3 &width,
        const math::vec3 &height,
        const math::vec3 &normal,
        const math::vec3 &position,
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
     * RETURNS:
     *   (primitive *) - not-owning pointer to the created plane primitive.
     */
    primitive *createPlanePrimitive(
        float width,
        float height,
        const math::vec3 &position
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
     * RETURNS:
     *   (primitive *) - not-owning pointer to the created cube primitive.
     */
    primitive *createCubePrimitive(
        float length,
        float width,
        float height,
        const math::vec3 &position
    );

protected:
    // Class constructor
    explicit unit();

    /* Unit initialization pure-virtual function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual void init() = 0;

    /* Unit response pure-virtual function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual void response() = 0;

    /* Render unit function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void render() const;

    /* Clear unit function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void clear();

    // Class virtual destructor
    virtual ~unit() = default;
};  // End of 'unit' class
}  // namespace hse

#endif  // UNIT_HPP
