#ifndef SHADER_HPP
#define SHADER_HPP

#include "../../../../def.hpp"

// Project namespace
namespace hse {
// Shader class definition
class Shader {
    uint programId;  // Shader program id

    // Shader program structure
    struct ShaderProgram {
        const char *name;    // Shader name
        int type;            // Shader type
        int id;              // Shader id
        std::string source;  // Shader source
    };

    // Shader's programs
    std::vector<ShaderProgram> shaders = {{"vertex", GL_VERTEX_SHADER, 0}, {"fragment", GL_FRAGMENT_SHADER, 0}};

    /* Parse shader program from file function.
     * ARGUMENTS:
     *   - shader's program path:
     *       const std::string &shaderPath;
     * RETURNS: None.
     */
    void parseShaderFromFile(const std::string &shaderPath);

    /* Create shader program with already initialized shaders source function.
     * ARGUMENTS:
     *   - shader program debug name:
     *       const std::string &shaderProgramDebugName;
     * RETURNS: None.
     */
    void createShaderProgram(const std::string &shaderProgramDebugName);

public:
    // Class default constructor
    explicit Shader();

    /* Class constructor.
     * ARGUMENTS:
     *   - shader's program path:
     *       const std::string &shaderPath;
     * NOTE: In out project we have an agreement: shaders (vertex, fragment)
     * realization you should write in one directory in 'bin/shaders' - exactly
     * the name of this directory you have to pass in this constructor
     * (example: see the test unit in SK4 directory).
     */
    explicit Shader(const std::string &shaderPath);

    /* Class constructor.
     * ARGUMENTS:
     *   - vertex shader source:
     *       const std::string &vertexShaderSource;
     *   - fragment shader source:
     *       const std::string &fragmentShaderSource.
     */
    explicit Shader(const std::string &vertexShaderSource, const std::string &fragmentShaderSource);

    // Class destructor
    ~Shader();

    /* Get shader id function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (uint) - shader id;
     */
    uint getShaderProgramId() const;
};  // End of 'Shader' class
}  // namespace hse

#endif  // SHADER_HPP
