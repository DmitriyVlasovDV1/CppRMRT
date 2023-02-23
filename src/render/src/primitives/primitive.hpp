#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include "../../../def.hpp"
#include "../shaders/shader.hpp"
#include "vertex_array.hpp"

namespace hse {
// Primitive class declaration
class primitive {
    // Friend classes
    friend class render;

    vertexArray *primitiveVertexArrayInstance{};  // Vertex array instance
    uint primitiveShaderProgramId{};              // Primitive's shader instance

    ::std::map<const char *, int *> shaderUniform1i;    // Shader's int uniforms
    ::std::map<const char *, float *> shaderUniform1f;  // Shader's float
                                                        // uniforms
    ::std::map<const char *, ::std::pair<float **, int>>
        shaderUniform3fv;  // Shader's float 3-component vector uniforms + count
                           // of these vectors

    // Class default constructor
    explicit primitive() = default;

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
    explicit primitive(
        uint primitiveShaderProgramId_,
        const ::std::vector<float> &vertexBuffer,
        const ::std::string &vertexBufferFormat = "v3v3",
        const ::std::vector<int> &indexBuffer = ::std::vector<int>()
    );

    /* Draw primitive function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void primitiveDraw();

    // Class destructor
    ~primitive();

public:
    /* Add uniform to the shader function.
     * ARGUMENTS:
     *   - uniform value:
     *       int *uniformValue;
     *   - uniform name on the shader:
     *       const char *uniformName;
     * RETURNS: None.
     */
    void shaderAddUniform1i(int *uniformValue, const char *uniformName);

    /* Add uniform to the shader function.
     * ARGUMENTS:
     *   - uniform value:
     *       float *uniformValue;
     *   - uniform name on the shader:
     *       const char *uniformName;
     * RETURNS: None.
     */
    void shaderAddUniform1f(float *uniformValue, const char *uniformName);

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
    void shaderAddUniform3fv(
        float **uniformValue,
        const char *uniformName,
        int uniformCount
    );
};  // End fo 'primitive' class
}  // namespace hse

#endif  // PRIMITIVE_HPP
