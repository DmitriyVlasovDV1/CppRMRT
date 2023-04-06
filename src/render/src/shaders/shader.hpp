#ifndef SHADER_HPP
#define SHADER_HPP

#include "../../../def.hpp"

// Project namespace
namespace hse {
// Shader class definition
class Shader {
    // Friend classes
    friend class Unit;

    uint programId;  // Shader program id

    // Shader program structure
    struct ShaderProgram {
        const char *name;    // Shader name
        int type;            // Shader type
        int id;              // Shader id
        const char *source;  // Shader source
    };

    // Shader's programs
    ::std::vector<ShaderProgram> shaders = {
        {"vertex", GL_VERTEX_SHADER, 0, nullptr},
        {"fragment", GL_FRAGMENT_SHADER, 0, nullptr}};

    /* Parse shader program from file function.
     * ARGUMENTS:
     *   - shader's program path:
     *       const ::std::string &shaderPath;
     * RETURNS: None.
     */
    void parseShaderFromFile(const ::std::string &shaderPath);

    /* Compile shader program function.
     * ARGUMENTS:
     *   - shader's program path:
     *       const ::std::string &shaderPath;
     * RETURNS: None.
     */
    void compileShaderProgram(const ::std::string &shaderPath);

    // Class default constructor
    explicit Shader();

    /* Class constructor.
     * ARGUMENTS:
     *   - shader's program path:
     *       const ::std::string &shaderPath;
     * NOTE: In out project we have an agreement: shaders (vertex, fragment)
     * realization you should write in one directory in 'bin/shaders' - exactly
     * the name of this directory you have to pass in this constructor
     * (example: see the test unit in SK4 directory).
     */
    explicit Shader(const ::std::string &shaderPath);

    // Class destructor
    ~Shader();

public:
    /* Get shader id function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (uint) - shader id;
     */
    uint getShaderProgramId() const;
};  // End of 'Shader' class
}  // namespace hse

#endif  // SHADER_HPP
