#ifndef SHADER_HPP
#define SHADER_HPP

#include "../../../def.hpp"

// Project namespace
namespace hse {
// Shader class definition
class shader {
    // Friend classes
    friend class unit;

    uint programId;  // Shader program id

    struct {
        const char *name;  // Shader name
        int type;          // Shader type
        int id;            // Shader id
    } shaders[2] = {
        {"vertex", GL_VERTEX_SHADER},       // Struct data for vertex shader
        {"fragment", GL_FRAGMENT_SHADER}};  // Struct data for fragment shader

    /* Parse shader program from file function.
     * ARGUMENTS:
     *   - shader's program path:
     *       const ::std::string &shaderPath;
     * RETURNS:
     *   (::std::pair<::std::string, ::std::string>) - pair of shaders
     * realization.
     */
    ::std::pair<::std::string, ::std::string> parseShaderFromFile(
        const ::std::string &shaderPath
    ) const;

    /* Compile shader program function.
     * ARGUMENTS:
     *   - shader's program path:
     *       const ::std::string &shaderPath;
     *   - vertex shader realization:
     *       const char *vertexShaderSource;
     *   - fragment shader realization:
     *       const char *fragmentShaderSource;
     */
    void compileShaderProgram(
        const ::std::string &shaderPath,
        const char *vertexShaderSource,
        const char *fragmentShaderSource
    );

    // Class default constructor
    explicit shader();

    /* Class constructor.
     * ARGUMENTS:
     *   - shader's program path:
     *       const ::std::string &shaderPath;
     * NOTE: In out project we have an agreement: shaders (vertex, fragment)
     * realization you should write in one directory in 'bin/shaders' - exactly
     * the name of this directory you have to pass in this constructor
     * (example: see the test unit in SK4 directory).
     */
    explicit shader(const ::std::string &shaderPath);

    /* Class constructor.
     * ARGUMENTS:
     *   - vertex shader source in string:
     *       const ::std::string &vertexShaderSource;
     *   - fragment shader source in string:
     *       const ::std::string &fragmentShaderSource;
     *   - shader's program path:
     *       const ::std::string &shaderPath;
     */
    explicit shader(
        const ::std::string &vertexShaderSource,
        const ::std::string &fragmentShaderSource,
        const ::std::string &shaderPath = ""
    );

    // Class destructor
    ~shader();

public:
    /* Get shader id function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (uint) - shader id;
     */
    uint getShaderProgramId() const;
};  // End of 'shader' class
}  // namespace hse

#endif  // SHADER_HPP
