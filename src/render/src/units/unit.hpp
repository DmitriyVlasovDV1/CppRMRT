#ifndef UNIT_HPP
#define UNIT_HPP

#include "../../../def.hpp"
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
    primitive *createPrimitive(
        const ::std::string &shaderPath,
        const ::std::vector<float> &vertexBuffer,
        const ::std::string &vertexBufferFormat = "v3v3",
        const ::std::vector<int> &indexBuffer = ::std::vector<int>()
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
