#include "shader.hpp"

namespace hse {
/* Parse shader program from file function.
 * ARGUMENTS:
 *   - shader's program path:
 *       const std::string &shaderPath;
 * RETURNS: None.
 */
void Shader::parseShaderFromFile(const std::string &shaderPath) {
    std::string fullShaderPath;
    std::ifstream shaderFile;

    for (auto &shader : shaders) {
        shader.id = 0;
        fullShaderPath = "../data/shaders/" + shaderPath + "/" + shader.name + ".glsl";
        shaderFile.open(fullShaderPath);
        if (!shaderFile.is_open()) EXCEPTION("Error in shader's path: " + shaderPath);
        std::string line, shaderSource;
        shaderSource = "";
        while (std::getline(shaderFile, line))
            shaderSource += line + "\n ";
        shader.source = shaderSource;
        shaderFile.close();
    }
}  // End of 'Shader::parseShaderFromFile' function

/* Create shader program with already initialized shaders source function.
 * ARGUMENTS:
 *   - shader program debug name:
 *       const std::string &shaderProgramDebugName;
 * RETURNS: None.
 */
void Shader::createShaderProgram(const std::string &shaderProgramDebugName) {
    for (auto &shader : shaders) {
        shader.id = glCreateShader(shader.type);
        if (shader.id == 0) EXCEPTION("Error in shader creation");

        const char *tmp = shader.source.c_str();
        glShaderSource(shader.id, 1, &tmp, nullptr);
        glCompileShader(shader.id);

        int compileResult;
        glGetShaderiv(shader.id, GL_COMPILE_STATUS, &compileResult);
        if (!compileResult) {
            char logBuffer[999];
            glGetShaderInfoLog(shader.id, sizeof(logBuffer), &compileResult, logBuffer);
            EXCEPTION("Shader log:\n" + shaderProgramDebugName + ": \n" + logBuffer);
        }
    }
    programId = glCreateProgram();
    if (programId == 0) {
        EXCEPTION("Error in creating shader program: " + shaderProgramDebugName);
    } else {
        for (auto &[name, type, id, source] : shaders)
            if (id != 0) glAttachShader(programId, id);
        glLinkProgram(programId);
        int linkStatus;
        glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus);
        if (!linkStatus) {
            char logBuffer[999];
            glGetProgramInfoLog(programId, sizeof(logBuffer), &linkStatus, logBuffer);
            EXCEPTION("Shader log:\n" + shaderProgramDebugName + ": \n" + logBuffer);
        }
    }
}  // End of 'Shader::createShaderProgram' function

// Class default constructor
Shader::Shader() : programId(0) {
}  // End of 'Shader::Shader' function

/* Class constructor.
 * ARGUMENTS:
 *   - path to shader's realization
 *       const std::string &shaderPath;
 * NOTE: In out project we have an agreement: shaders (vertex, fragment)
 * realization you should write in one directory in 'bin/shaders' - exactly
 * the name of this directory you have to pass in this constructor
 * (example: see test unit in SK4 directory).
 */
Shader::Shader(const std::string &shaderPath) : programId(0) {
    parseShaderFromFile(shaderPath);
    createShaderProgram(shaderPath);
}  // End of 'Shader::Shader' function

/* Class constructor.
 * ARGUMENTS:
 *   - vertex shader source:
 *       const std::string &vertexShaderSource;
 *   - fragment shader source:
 *       const std::string &fragmentShaderSource.
 */
Shader::Shader(const std::string &vertexShaderSource, const std::string &fragmentShaderSource) : programId(0) {
    for (auto &[name, type, id, source] : shaders) {
        if (name == "vertex")
            source = vertexShaderSource;
        else if (name == "fragment")
            source = fragmentShaderSource;
    }
    createShaderProgram("Shader created by strings");
}  // End of 'Shader::Shader' function

/* Get shader id function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (uint) - shader id;
 */
uint Shader::getShaderProgramId() const {
    return programId;
}  // End of 'Shader::getShaderProgramId' function

// Class destructor
Shader::~Shader() {
    for (auto &[name, type, id, source] : shaders)
        if (id != 0) {
            if (programId != 0) glDetachShader(programId, id);
            glDeleteShader(id);
        }
    if (programId != 0) glDeleteProgram(programId);
    programId = 0;
}  // End of 'Shader::~shader' function
}  // namespace hse