#include "shader.hpp"

namespace hse {
/* Load shader program from file function.
 * ARGUMENTS:
 *   - shader's program path:
 *       const ::std::string &shaderPath;
 * RETURNS:
 *   (::std::pair<::std::string, ::std::string>) - pair of shaders realization.
 */
::std::pair<::std::string, ::std::string> shader::loadShaderFromFile(
    const ::std::string &shaderPath
) const {
    int counter = 0;
    ::std::pair<::std::string, ::std::string> result;
    ::std::string fullShaderPath;
    std::ifstream shaderFile;
    ::std::string shaderSource;

    for (auto &[name, type, id] : shaders) {
        fullShaderPath = "../data/shaders/" + shaderPath + "/" + name + ".glsl";
        shaderFile.open(fullShaderPath);
        if (!shaderFile.is_open()) {
            assert("Error in shader's path");
            break;
        }
        ::std::string line;
        shaderSource = "";
        while (::std::getline(shaderFile, line))
            shaderSource += line + "\n ";
        shaderFile.close();
        if (counter++ == 0)
            result.first = shaderSource;
        else
            result.second = shaderSource;
    }
    return result;
}  // End of 'shader::loadShaderFromFile' function

/* Compile shader program function.
 * ARGUMENTS:
 *   - shader's program path:
 *       const ::std::string &shaderPath;
 *   - vertex shader realization:
 *       const char *vertexShaderSource;
 *   - fragment shader realization:
 *       const char *fragmentShaderSource;
 */
void shader::compileShaderProgram(
    const ::std::string &shaderPath,
    const char *vertexShaderSource,
    const char *fragmentShaderSource
) {
    bool isError = false;
    ::std::string fullShaderPath;

    for (auto &[name, type, id] : shaders) {
        fullShaderPath = "../data/shaders/" + shaderPath + "/" + name + ".glsl";
        id = glCreateShader(type);
        if (id == 0) {
            assert("Error in shader creation");
            isError = true;
            break;
        }
        if (type == GL_VERTEX_SHADER)
            glShaderSource(id, 1, &vertexShaderSource, nullptr);
        else
            glShaderSource(id, 1, &fragmentShaderSource, nullptr);
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
}  // End of 'shader::compileShaderProgram' function

// Class default constructor
shader::shader() : programId(0) {
}  // End of 'shader::shader' function

/* Class constructor.
 * ARGUMENTS:
 *   - path to shader's realization
 *       const ::std::string &shaderPath;
 * NOTE: In out project we have an agreement: shaders (vertex, fragment)
 * realization you should write in one directory in 'bin/shaders' - exactly
 * the name of this directory you have to pass in this constructor
 * (example: see test unit in SK4 directory).
 */
shader::shader(const ::std::string &shaderPath) : programId(0) {
    const ::std::pair<::std::string, ::std::string> shaderSource =
        loadShaderFromFile(shaderPath);
    compileShaderProgram(
        shaderPath, shaderSource.first.c_str(), shaderSource.second.c_str()
    );
}  // End of 'shader::shader' function

/* Class constructor.
 * ARGUMENTS:
 *   - vertex shader source in string:
 *       const ::std::string &vertexShaderSource;
 *   - fragment shader source in string:
 *       const ::std::string &fragmentShaderSource;
 *   - shader's program path:
 *       const ::std::string &shaderPath;
 */
shader::shader(
    const ::std::string &vertexShaderSource,
    const ::std::string &fragmentShaderSource,
    const ::std::string &shaderPath
)
    : programId(0) {
    compileShaderProgram(
        shaderPath, vertexShaderSource.c_str(), vertexShaderSource.c_str()
    );
}  // End of 'shader::shader' function

/* Get shader id function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (uint) - shader id;
 */
uint shader::getShaderProgramId() const {
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
