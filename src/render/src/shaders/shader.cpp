#include "shader.hpp"

namespace hse {
/* Parse shader program from file function.
 * ARGUMENTS:
 *   - shader's program path:
 *       const ::std::string &shaderPath;
 * RETURNS: None.
 */
void Shader::parseShaderFromFile(const ::std::string &shaderPath) {
    ::std::string fullShaderPath;
    std::ifstream shaderFile;
    ::std::string shaderSource;

    for (auto &[name, type, id, source] : shaders) {
        id = 0;
        fullShaderPath = "../data/shaders/" + shaderPath + "/" + name + ".glsl";
        shaderFile.open(fullShaderPath);
        if (!shaderFile.is_open()) EXCEPTION("Error in shader's path");
        ::std::string line;
        shaderSource = "";
        while (::std::getline(shaderFile, line))
            shaderSource += line + "\n ";
        shaderFile.close();
        source = shaderSource.c_str();
    }
}  // End of 'Shader::parseShaderFromFile' function

/* Compile shader program function.
 * ARGUMENTS:
 *   - shader's program path:
 *       const ::std::string &shaderPath;
 * RETURNS: None.
 */
void Shader::compileShaderProgram(const ::std::string &shaderPath) {
    parseShaderFromFile(shaderPath);

    bool isError = false;
    ::std::string fullShaderPath;

    for (auto &[name, type, id, source] : shaders) {
        fullShaderPath = "../data/shaders/" + shaderPath + "/" + name + ".glsl";
        id = glCreateShader(type);
        if (id == 0) {
            assert("Error in shader creation");
            isError = true;
            break;
        }
        glShaderSource(id, 1, &source, nullptr);
        glCompileShader(id);
        int compileResult;
        glGetShaderiv(id, GL_COMPILE_STATUS, &compileResult);
        if (!compileResult) {
            char logBuffer[999];
            glGetShaderInfoLog(
                id, sizeof(logBuffer), &compileResult, logBuffer
            );
            ::std::cout << "Error in compiling: " << fullShaderPath
                        << " logs:\n"
                        << logBuffer << ::std::endl;
            isError = true;
            break;
        }
    }
    if (isError) return;
    programId = glCreateProgram();
    if (programId == 0) {
        isError = true;
        EXCEPTION("Error in creating shader program");
    } else {
        for (auto &[name, type, id, source] : shaders)
            if (id != 0) glAttachShader(programId, id);
        glLinkProgram(programId);
        int linkStatus;
        glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus);
        if (!linkStatus) {
            char logBuffer[999];
            glGetProgramInfoLog(
                programId, sizeof(logBuffer), &linkStatus, logBuffer
            );
            ::std::cout << "Error in linking: " << fullShaderPath
                        << ::std::endl;
            isError = true;
        }
    }
    if (isError) {
        for (auto &[name, type, id, source] : shaders)
            if (id != 0) {
                if (programId != 0) glDetachShader(programId, id);
                glDeleteShader(id);
            }
        if (programId != 0) glDeleteProgram(programId);
        programId = 0;
    }
}  // End of 'Shader::compileShaderProgram' function

// Class default constructor
Shader::Shader() : programId(0) {
}  // End of 'Shader::Shader' function

/* Class constructor.
 * ARGUMENTS:
 *   - path to shader's realization
 *       const ::std::string &shaderPath;
 * NOTE: In out project we have an agreement: shaders (vertex, fragment)
 * realization you should write in one directory in 'bin/shaders' - exactly
 * the name of this directory you have to pass in this constructor
 * (example: see test unit in SK4 directory).
 */
Shader::Shader(const ::std::string &shaderPath) : programId(0) {
    compileShaderProgram(shaderPath);
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
    ::std::cout << "Clear shader" << ::std::endl;
}  // End of 'Shader::~shader' function
}  // namespace hse