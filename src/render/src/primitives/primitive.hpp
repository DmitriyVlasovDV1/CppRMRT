#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include "../../../def.hpp"
#include "../../../utilities/cameras/camera.hpp"
#include "../buffers/buffer.hpp"
#include "../shaders/shader.hpp"

namespace hse {
// Primitive class declaration
class primitive {
    // Friend classes
    friend class unit;
    friend class model;

    buffer::renderType renderType;     // Rendering type
    vertexArray *vertexArrayInstance;  // Vertex array instance
    uint shaderProgramId;              // Primitive's shader instance
    bool isVisible;                    // Visibility flag

public:
    math::matr4 transformMatrix;  // Transform matrix

private:
    ::std::map<const char *, int> shaderUniform1i;           // Integer uniforms
    ::std::map<const char *, float> shaderUniform1f;         // Floats uniforms
    ::std::map<const char *, math::vec3> shaderUniform3fv;   // 3d-vector
                                                             // uniforms
    ::std::map<const char *, math::matr4> shaderUniform4fv;  // Matrix4x4
                                                             // uniforms

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
     * ARGUMENTS:
     *   - camera for rendering primitive:
     *      const camera &camera;
     * RETURNS: None.
     */
    void render(const camera &camera) const;

    // Class destructor
    ~primitive();

public:
    /* Attach shader program id to the primitive function.
     * ARGUMENTS:
     *   - shader program id:
     *       uint shaderProgramId_;
     * RETURNS: None.
     */
    void setShaderProgram(uint shaderProgramId_);

    /* Get primitive rendering type function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (buffer::renderType) - rendering type.
     */
    buffer::renderType getRenderType() const;

    /* Set primitive rendering type function.
     * ARGUMENTS:
     *   - new rendering type:
     *       buffer::renderType renderType_:
     * RETURNS: None.
     */
    void setRenderType(buffer::renderType renderType_);

    /* Get primitive's visibility flag function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (bool) - visibility flag.
     */
    bool getVisibility() const;

    /* Set primitive's visibility flag function.
     * ARGUMENTS:
     *   - new flag:
     *       bool isVisible_;
     * RETURNS: None.
     */
    void setVisibility(bool isVisible_);

    /* Add uniform to the shader function.
     * ARGUMENTS:
     *   - uniform value:
     *       int uniformValue;
     *   - uniform name on the shader:
     *       const char *uniformName;
     * RETURNS: None.
     */
    void addUniform(int uniformValue, const char *uniformName);

    /* Add uniform of one float variable to the shader function.
     * ARGUMENTS:
     *   - uniform value:
     *       float uniformValue;
     *   - uniform name on the shader:
     *       const char *uniformName;
     * RETURNS: None.
     */
    void addUniform(float uniformValue, const char *uniformName);

    /* Add uniform of 3-component geom vector to the shader function.
     * ARGUMENTS:
     *   - uniform value:
     *       const math::vec3 &vector;
     *   - uniform name on the shader:
     *       const char *uniformName;
     * RETURNS: None.
     */
    void addUniform(const math::vec3 &vector, const char *uniformName);

    /* Add uniform of matrix4x4 variable to the shader function.
     * ARGUMENTS:
     *   - uniform value:
     *       const math::matr4 &uniformValue;
     *   - uniform name on the shader:
     *       const char *uniformName;
     *   - uniforms number:
     *       int uniformCount;
     * RETURNS: None.
     */
    void addUniform(const math::matr4 &matrix, const char *uniformName);
};  // End of 'primitive' class
}  // namespace hse

#endif  // PRIMITIVE_HPP
