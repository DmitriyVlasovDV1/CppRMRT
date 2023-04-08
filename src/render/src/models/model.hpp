#ifndef MODEL_HPP
#define MODEL_HPP

#include "../../../def.hpp"
#include "../primitives/primitive.hpp"

// Project namespace
namespace hse {
// Model class declaration
class Model {
    // Friend classes
    friend class Scene;

    uint shaderProgramId;                        // Model's shader program id
    bool isVisible;                              // Model's visibility flag
    VertexArray::renderType renderType;          // Model rendering type
    ::std::vector<Primitive *> primitivesArray;  // Array of all model's
                                                 // primitives

    ::std::map<const char *, int> shaderUniform1i;           // Integer uniforms
    ::std::map<const char *, float> shaderUniform1f;         // Floats uniforms
    ::std::map<const char *, math::vec3> shaderUniform3fv;   // 3d-vector
                                                             // uniforms
    ::std::map<const char *, math::matr4> shaderUniform4fv;  // Matrix4x4
                                                             // uniforms

public:
    math::matr4 transformMatrix;  // Transform matrix for model

private:
    /* Parse *.obj file function.
     * ARGUMENTS:
     *   - model's file name:
     *       const ::std::string &fileName;
     * RETURNS: None.
     */
    void parseObj(const ::std::string &fileName);

    /* Class constructor.
     * ARGUMENTS:
     *   - model's shader program id:
     *       uint shaderProgramId_;
     *   - model's file name:
     *       const ::std::string &fileName.
     */
    explicit Model(uint shaderProgramId_, const ::std::string &fileName);

    /* Draw model function.
     * ARGUMENTS:
     *   - camera for rendering model:
     *      const Camera &camera;
     * RETURNS: None.
     */
    void onRender(const Camera &camera) const;

    // Class destructor
    ~Model();

public:
    /* Attach shader program id to the model function.
     * ARGUMENTS:
     *   - shader program id:
     *       uint shaderProgramId_;
     * RETURNS: None.
     */
    void setShaderProgram(uint shaderProgramId_);

    /* Get model's visibility flag function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (bool) - visibility flag.
     */
    bool getVisibility() const;

    /* Set model's visibility flag function.
     * ARGUMENTS:
     *   - new flag:
     *       bool isVisible_;
     * RETURNS: None.
     */
    void setVisibility(bool isVisible_);

    /* Get model rendering type function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (VertexArray::renderType) - rendering type.
     */
    VertexArray::renderType getRenderType() const;

    /* Set model rendering type function.
     * ARGUMENTS:
     *   - new rendering type:
     *       VertexArray::renderType renderType_:
     * RETURNS: None.
     */
    void setRenderType(VertexArray::renderType renderType_);

    /* Get number of primitives in model function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (int) - number of primitives in model.
     */
    int getNumberOfChildren() const;

    /* Get primitive's pointer by index in model function.
     * ARGUMENTS:
     *   - index of the primitive:
     *       int index;
     * RETURNS:
     *   (Primitive *) - not-owning pointer to the primitive.
     */
    Primitive *getChild(int index) const;

    /* Add uniform of one integer variable to the shader function.
     * ARGUMENTS:
     *   - uniform value:
     *       int &uniformValue;
     *   - uniform name on the shader:
     *       const char *uniformName;
     * RETURNS: None.
     */
    void addUniform(int &uniformValue, const char *uniformName);

    /* Add uniform of one float variable to the shader function.
     * ARGUMENTS:
     *   - uniform value:
     *       float &uniformValue;
     *   - uniform name on the shader:
     *       const char *uniformName;
     * RETURNS: None.
     */
    void addUniform(float &uniformValue, const char *uniformName);

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
};  // End of 'Model' class
}  // namespace hse

#endif  // MODEL_HPP
