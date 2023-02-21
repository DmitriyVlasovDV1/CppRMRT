#ifndef SHADER_HPP
#define SHADER_HPP

#include "../../def.hpp"

// Project namespace
namespace hse {
// Shader class definition
class shader {
    // Friend classes
    friend class render;

private:
    uint shaderProgramId;  // Shader program id

    struct {
        const char *name;  // Shader name
        int type;          // Shader type
        int id;            // Shader id
    } shaders[2] = {
        {"vertex", GL_VERTEX_SHADER},
        {"fragment", GL_FRAGMENT_SHADER}};

    ::std::map<const char *, int> shaderUniform1i;    // Shader's int uniforms
    ::std::map<const char *, float> shaderUniform1f;  // Shader's float
                                                      // uniforms
    ::std::map<const char *, const float *> shaderUniform3fv;  // Shader's
                                                               // float
                                                               // 3-component
                                                               // vector
                                                               // uniforms

private:
    // Class default constructor
    explicit shader();

    /* Class constructor.
     * ARGUMENTS:
     *   - path to shader's realization
     *       const ::std::string &shaderPath_;
     * NOTE: In out project we have an agreement: shaders (vertex, fragment)
     * realization you should write in one directory in 'bin/shaders' - exactly
     * the name of this directory you have to pass in this constructor
     * (example: see test unit in SK4 directory).
     */
    explicit shader(const ::std::string &shaderPath);

public:
    /* Apply shader function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void shaderApply() const;

    /* Add uniform to the shader function.
     * ARGUMENTS:
     *   - uniform value:
     *       int uniformValue;
     *   - uniform name on the shader:
     *       const char *uniformName;
     * RETURNS: None.
     */
    void shaderAddUniform1i(int uniformValue, const char *uniformName);

    /* Add uniform to the shader function.
     * ARGUMENTS:
     *   - uniform value:
     *       float uniformValue;
     *   - uniform name on the shader:
     *       const char *uniformName;
     * RETURNS: None.
     */
    void shaderAddUniform1f(float uniformValue, const char *uniformName);

    /* Add uniform to the shader function.
     * ARGUMENTS:
     *   - uniform value:
     *       const float *uniformValue;
     *   - uniform name on the shader:
     *       const char *uniformName;
     * RETURNS: None.
     */
    void shaderAddUniform3fv(
        const float *uniformValue,
        const char *uniformName
    );

private:
    // Class destructor
    ~shader();

public:
    /* Get shader id function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (uint) - shader id;
     */
    [[nodiscard]] uint getShaderProgramId() const;

};  // End of 'shader' class
}  // namespace hse

#endif  // SHADER_HPP
