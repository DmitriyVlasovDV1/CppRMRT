#include "shader.hpp"

namespace hse {
// Class default constructor
shader::shader() : shaderProgramId(0) {
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
        fullShaderPath = "../bin/shaders/" + shaderPath + "/" + name + ".glsl";
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
    shaderProgramId = glCreateProgram();
    if (shaderProgramId == 0) {
        isError = true;
        assert("Error in creating shader program");
    } else {
        for (auto &[name, type, id] : shaders)
            if (id != 0) glAttachShader(shaderProgramId, id);
        glLinkProgram(shaderProgramId);
        int linkStatus;
        glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &linkStatus);
        if (!linkStatus) {
            char logBuffer[999];
            glGetProgramInfoLog(
                shaderProgramId, sizeof(logBuffer), &linkStatus, logBuffer
            );
            ::std::cout << "Error in linking: " << fullShaderPath
                        << ::std::endl;
            isError = true;
        }
    }
    if (isError) {
        for (auto &[name, type, id] : shaders)
            if (id != 0) {
                if (shaderProgramId != 0) glDetachShader(shaderProgramId, id);
                glDeleteShader(id);
            }
        if (shaderProgramId != 0) glDeleteProgram(shaderProgramId);
        shaderProgramId = 0;
    }
}  // End of 'shader::shader' function

/* Apply shader function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void shader::shaderApply() const {
    glUseProgram(shaderProgramId);
    int uniformLocation;
    for (auto &[uniformName, uniformValue] : shaderUniform1i) {
        uniformLocation = glGetUniformLocation(shaderProgramId, uniformName);
        if (uniformLocation != -1) glUniform1i(uniformLocation, uniformValue);
    }
    for (auto &[uniformName, uniformValue] : shaderUniform1f) {
        uniformLocation = glGetUniformLocation(shaderProgramId, uniformName);
        if (uniformLocation != -1) glUniform1f(uniformLocation, uniformValue);
    }
    for (auto &[uniformName, uniformValue] : shaderUniform3fv) {
        uniformLocation = glGetUniformLocation(shaderProgramId, uniformName);
        if (uniformLocation != -1)
            glUniform3fv(uniformLocation, 1, uniformValue);
    }
}  // End of 'shader::shaderApply' function

/* Add uniform to the shader function.
 * ARGUMENTS:
 *   - uniform value:
 *       int uniformValue;
 *   - uniform name on the shader:
 *       const char *uniformName;
 * RETURNS: None.
 */
void shader::shaderAddUniform1i(int uniformValue, const char *uniformName) {
    shaderUniform1i[uniformName] = uniformValue;
}  // End of 'shader::shaderAddUniform1i' function

/* Add uniform to the shader function.
 * ARGUMENTS:
 *   - uniform value:
 *       float uniformValue;
 *   - uniform name on the shader:
 *       const char *uniformName;
 * RETURNS: None.
 */
void shader::shaderAddUniform1f(float uniformValue, const char *uniformName) {
    shaderUniform1f[uniformName] = uniformValue;
}  // End of 'shader::shaderAddUniform1f' function

/* Add uniform to the shader function.
 * ARGUMENTS:
 *   - uniform value:
 *       float *uniformValue;
 *   - uniform name on the shader:
 *       const char *uniformName;
 * RETURNS: None.
 */
void shader::shaderAddUniform3fv(float *uniformValue, const char *uniformName) {
    shaderUniform3fv[uniformName] = uniformValue;
}  // End of 'shader::shaderAddUniform3fv' function

/* Get shader id function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (uint) - shader id;
 */
[[nodiscard]] uint shader::getShaderProgramId() const {
    return shaderProgramId;
}  // End of 'shader::getShaderProgramId' function

// Class destructor
shader::~shader() {
    for (auto &[name, type, id] : shaders)
        if (id != 0) {
            if (shaderProgramId != 0) glDetachShader(shaderProgramId, id);
            glDeleteShader(id);
        }
    if (shaderProgramId != 0) glDeleteProgram(shaderProgramId);
    shaderProgramId = 0;
}  // End of 'shader::~shader' function
}  // namespace hse