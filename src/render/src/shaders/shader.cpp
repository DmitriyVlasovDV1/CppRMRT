#include "shader.hpp"

namespace hse {
// Class default constructor
shader::shader() : programId(0) {
}  // End of 'shader::shader' function

/* Class constructor.
 * ARGUMENTS:
 *   - path to shader's realization
 *       const ::std::string &shaderPath_;
 * NOTE: In out project we have an agreement: shaders (vertex, fragment)
 * realization you should write in one directory in 'bin/shaders' - exactly
 * the name of this directory you have to pass in this constructor
 * (example: see test unit in SK4 directory).
 */
shader::shader(const ::std::string &shaderPath) {
    bool isError = false;
    static ::std::string fullShaderPath;
    static std::ifstream shaderFile;
    ::std::string shaderSource;
    for (auto &[name, type, id] : shaders) {
        fullShaderPath = "../data/shaders/" + shaderPath + "/" + name + ".glsl";
        shaderFile.open(fullShaderPath);
        if (!shaderFile.is_open()) {
            assert("Error in shader's path");
            isError = true;
            break;
        }
        id = glCreateShader(type);
        if (id == 0) {
            assert("Error in shader creation");
            isError = true;
            break;
        }
        ::std::string line;
        shaderSource = "";
        while (::std::getline(shaderFile, line))
            shaderSource += line + "\n ";
        shaderFile.close();
        const char *txt = shaderSource.c_str();
        glShaderSource(id, 1, &txt, nullptr);
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
        assert("Error in creating shader program");
    } else {
        for (auto &[name, type, id] : shaders)
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
        for (auto &[name, type, id] : shaders)
            if (id != 0) {
                if (programId != 0) glDetachShader(programId, id);
                glDeleteShader(id);
            }
        if (programId != 0) glDeleteProgram(programId);
        programId = 0;
    }
}  // End of 'shader::shader' function

/* Get shader id function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (uint) - shader id;
 */
[[nodiscard]] uint shader::getShaderProgramId() const {
    return programId;
}  // End of 'shader::getShaderProgramId' function

// Class destructor
shader::~shader() {
    for (auto &[name, type, id] : shaders)
        if (id != 0) {
            if (programId != 0) glDetachShader(programId, id);
            glDeleteShader(id);
        }
    if (programId != 0) glDeleteProgram(programId);
        programId = 0;
    ::std::cout << "Clear shader" << ::std::endl;
}  // End of 'shader::~shader' function
}  // namespace hse