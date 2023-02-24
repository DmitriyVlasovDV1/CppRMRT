#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include "../../../def.hpp"
#include "../buffers/buffer.hpp"
#include "../shaders/shader.hpp"

namespace hse {
// Primitive class declaration
class primitive {
    // Friend classes
    friend class unit;

    buffer::renderType type;           // Rendering type
    vertexArray *vertexArrayInstance;  // Vertex array instance
    uint shaderProgramId;              // Primitive's shader instance

    ::std::map<const char *, int *> shaderUniform1i;                        // Shader's int uniforms
    ::std::map<const char *, float *> shaderUniform1f;                      // Shader's float uniforms
    ::std::map<const char *, ::std::pair<float **, int>> shaderUniform3fv;  // Shader's float 3-component vector uniforms
                                                                            // + count of these vectors

    // Class default constructor
    explicit primitive();

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
    explicit primitive(
        uint primitiveShaderProgramId_,
        const ::std::vector<float> &vertexBuffer,
        const ::std::string &vertexBufferFormat = "v3v3",
        const ::std::vector<int> &indexBuffer = ::std::vector<int>(),
        buffer::renderType type_ = buffer::renderType::TRIANGLES
    );

    /* Class constructor.
     * ARGUMENTS:
     *   - primitive's shader program id:
     *       uint primitiveShaderProgramId_;
     *   - vertex array:
     *       vertexArray *vertexArrayInstance_;
     *   - primitive's rendering type:
     *       renderType type_.
     */
    explicit primitive(
        uint primitiveShaderProgramId_,
        vertexArray *vertexArrayInstance_,
        buffer::renderType type_ = buffer::renderType::TRIANGLES
    );

    /* Draw primitive function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void render() const;

    // Class destructor
    ~primitive();

public:
    /* Set primitive rendering type function.
     * ARGUMENTS:
     *   - new rendering type:
     *       buffer::renderType type_:
     * RETURNS: None.
     */
    void setRenderType(buffer::renderType type_);

    /* Add uniform to the shader function.
     * ARGUMENTS:
     *   - uniform value:
     *       const int *uniformValue;
     *   - uniform name on the shader:
     *       const char *uniformName;
     * RETURNS: None.
     */
    void addUniform(const int *uniformValue, const char *uniformName);

    /* Add uniform to the shader function.
     * ARGUMENTS:
     *   - uniform value:
     *       const float *uniformValue;
     *   - uniform name on the shader:
     *       const char *uniformName;
     * RETURNS: None.
     */
    void addUniform(const float *uniformValue, const char *uniformName);

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
    void addUniform(
        const float **uniformValue,
        const char *uniformName,
        int uniformCount = 1
    );
};  // End of 'primitive' class
}  // namespace hse

#endif  // PRIMITIVE_HPP
