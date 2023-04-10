#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include "../../../../def.hpp"
#include "../../../../utilities/camera/camera.hpp"
#include "../buffers/buffer.hpp"
#include "../shaders/shader.hpp"

namespace hse {
// Primitive class declaration
class Primitive {
    // Friend classes
    friend class Scene;

protected:
    VertexArray *vertexArrayInstance;    // Vertex array instance
    VertexArray::renderType renderType;  // Rendering type
    uint shaderProgramId;                // Primitive's shader instance
    bool isVisible;                      // Visibility flag
    math::matr4 transformMatrix;         // Transform matrix

    std::map<const char *, const int *> shaderUniform1i;           // Integer uniforms
    std::map<const char *, const float *> shaderUniform1f;         // Floats uniforms
    std::map<const char *, const math::vec3 *> shaderUniform3fv;   // 3d-vector uniforms
    std::map<const char *, const math::matr4 *> shaderUniform4fv;  // Matrix4x4 uniforms

    std::map<const char *, int> shaderUniform1iConstant;           // Integer constant uniforms
    std::map<const char *, float> shaderUniform1fConstant;         // Floats constant uniforms
    std::map<const char *, math::vec3> shaderUniform3fvConstant;   // 3d-vector constant uniforms
    std::map<const char *, math::matr4> shaderUniform4fvConstant;  // Matrix4x4 constant uniforms

public:
    // Class default constructor
    explicit Primitive();

    /* Class constructor.
     * ARGUMENTS:
     *   - primitive's shader program id:
     *       uint primitiveShaderProgramId_;
     *   - vertex buffer data:
     *       const std::vector<float> &vertexBuffer;
     *   - vertex buffer format:
     *       const std::string &vertexBufferFormat;
     *   - index buffer data:
     *       const std::vector<int> &indexBuffer;
     *   - primitive's rendering type:
     *       VertexArray::renderType type_;
     * NOTE: vertexBufferFormat - use default type or "v3v3v3v2" == vertex
     * position, color, normal, texture coordinate.
     */
    explicit Primitive(
        uint primitiveShaderProgramId_,
        const std::vector<float> &vertexBuffer,
        const std::string &vertexBufferFormat = "v3v3",
        const std::vector<int> &indexBuffer = ::std::vector<int>(),
        VertexArray::renderType type_ = VertexArray::renderType::TRIANGLES
    );

    /* Class constructor.
     * ARGUMENTS:
     *   - primitive's shader program id:
     *       uint primitiveShaderProgramId_;
     *   - vertex array:
     *       vertexArray *vertexArrayInstance_;
     *   - primitive's rendering type:
     *       VertexArray::renderType type_.
     */
    explicit Primitive(
        uint primitiveShaderProgramId_,
        VertexArray *vertexArrayInstance_,
        VertexArray::renderType type_ = VertexArray::renderType::TRIANGLES
    );

    /* Render primitive function.
     * ARGUMENTS:
     *   - camera for rendering primitive:
     *      const camera &camera;
     * RETURNS: None;
     * NOTE: Don't use it - use interface in scene, primitives will render automatically.
     */
    virtual void onRender(const Camera &camera) const;

    // Class destructor
    ~Primitive();

    /* Attach shader program id to the primitive function.
     * ARGUMENTS:
     *   - shader program id:
     *       uint shaderProgramId_;
     * RETURNS: None.
     */
    void setShaderProgram(uint shaderProgramId_);

    /* Get primitive's transform matrix function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (math::matr4) - primitive's transform matrix.
     */
    math::matr4 getTransformMatrix() const;

    /* Set primitive's transform matrix function.
     * ARGUMENTS:
     *   - new primitive transform matrix:
     *       const math::matr4 &transformMatrix_;
     * RETURNS: None.
     */
    void setTransformMatrix(const math::matr4 &transformMatrix_);

    /* Get primitive rendering type function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (VertexArray::renderType) - rendering type.
     */
    VertexArray::renderType getRenderType() const;

    /* Set primitive rendering type function.
     * ARGUMENTS:
     *   - new rendering type:
     *       VertexArray::renderType renderType_:
     * RETURNS: None.
     */
    virtual void setRenderType(VertexArray::renderType renderType_);

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

    /* Add dynamic uniform to the shader function.
     * ARGUMENTS:
     *   - uniform value:
     *       const int *uniformValue;
     *   - uniform name on the shader:
     *       const char *uniformName;
     * RETURNS: None.
     */
    void addUniform(const int *uniformValue, const char *uniformName);

    /* Add dynamic uniform of one float variable to the shader function.
     * ARGUMENTS:
     *   - uniform value:
     *       const float *uniformValue;
     *   - uniform name on the shader:
     *       const char *uniformName;
     * RETURNS: None.
     */
    void addUniform(const float *uniformValue, const char *uniformName);

    /* Add dynamic uniform of 3-component geom vector to the shader function.
     * ARGUMENTS:
     *   - uniform value:
     *       const math::vec3 *vector;
     *   - uniform name on the shader:
     *       const char *uniformName;
     * RETURNS: None.
     */
    void addUniform(const math::vec3 *vector, const char *uniformName);

    /* Add dynamic uniform of matrix4x4 variable to the shader function.
     * ARGUMENTS:
     *   - uniform value:
     *       const math::matr4 *matrix;
     *   - uniform name on the shader:
     *       const char *uniformName;
     * RETURNS: None.
     */
    void addUniform(const math::matr4 *matrix, const char *uniformName);

    /* Add constant uniform of one integer variable to the shader function.
     * ARGUMENTS:
     *   - uniform value:
     *       int uniformValue;
     *   - uniform name on the shader:
     *       const char *uniformName;
     * RETURNS: None.
     */
    void addConstantUniform(int uniformValue, const char *uniformName);

    /* Add constant uniform of one float variable to the shader function.
     * ARGUMENTS:
     *   - uniform value:
     *       float uniformValue;
     *   - uniform name on the shader:
     *       const char *uniformName;
     * RETURNS: None.
     */
    void addConstantUniform(float uniformValue, const char *uniformName);

    /* Add constant uniform of 3-component geom vector to the shader function.
     * ARGUMENTS:
     *   - uniform value:
     *       const math::vec3 &vector;
     *   - uniform name on the shader:
     *       const char *uniformName;
     * RETURNS: None.
     */
    void addConstantUniform(const math::vec3 &vector, const char *uniformName);

    /* Add constant uniform of matrix4x4 variable to the shader function.
     * ARGUMENTS:
     *   - uniform value:
     *       const math::matr4 &matrix;
     *   - uniform name on the shader:
     *       const char *uniformName;
     * RETURNS: None.
     */
    void addConstantUniform(const math::matr4 &matrix, const char *uniformName);
};  // End of 'Primitive' class
}  // namespace hse

#endif  // PRIMITIVE_HPP
