#ifndef SHADER_HPP
#define SHADER_HPP

#include "../../def.hpp"

// Project namespace
namespace hse {
// Shader class definition
class shader {
    // Friend classes
    friend class sourceManager;

private:
    uint shaderProgramId;  // Shader program id

    struct {
        const char *name;  // Shader name
        int type;          // Shader type
        int id;            // Shader id
    } shaders[2] = {
        {"vertex", GL_VERTEX_SHADER},
        {"fragment", GL_FRAGMENT_SHADER}};

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
