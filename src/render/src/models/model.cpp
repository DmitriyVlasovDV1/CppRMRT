#include "model.hpp"

namespace hse {
/* Parse *.obj file function.
 * ARGUMENTS:
 *   - model's file name:
 *       const ::std::string &fileName;
 * RETURNS: None.
 */
void Model::parseObj(const ::std::string &fileName) {
    ::std::ifstream fileContent("../data/models/" + fileName);
    if (!fileContent.is_open())
        EXCEPTION(("Do not found model file: " + fileName).c_str());

    ::std::vector<math::vec3> vertexPositions, vertexNormals;
    ::std::vector<math::vec2> vertexTextureCoordinates;
    ::std::vector<float> vertexBufferData;
    ::std::vector<int> indexBufferData;

    ::std::string type, oldType;
    int numberOfVertexes = 0;
    while (fileContent >> type) {
        if (type == "o") {
            if (oldType == "f" && !vertexBufferData.empty()) {
                primitivesArray.push_back(new Primitive(
                    shaderProgramId, vertexBufferData, "v3v2v3", indexBufferData
                ));
                primitivesArray.back()->setRenderType(renderType);
                indexBufferData.clear();
                vertexBufferData.clear();
                vertexPositions.clear();
                vertexNormals.clear();
                vertexTextureCoordinates.clear();
            }
            ::std::string objectName;  // Just skip this
            fileContent >> objectName;
        } else if (type == "v") {
            math::vec3 vertexPosition;
            fileContent >> vertexPosition.x;
            fileContent >> vertexPosition.y;
            fileContent >> vertexPosition.z;
            vertexPositions.push_back(vertexPosition);
        } else if (type == "vt") {
            math::vec2 textureCoordinate;
            fileContent >> textureCoordinate.x;
            fileContent >> textureCoordinate.y;
            vertexTextureCoordinates.push_back(textureCoordinate);
        } else if (type == "vn") {
            math::vec3 vertexNormal;
            fileContent >> vertexNormal.x;
            fileContent >> vertexNormal.y;
            fileContent >> vertexNormal.z;
            vertexNormals.push_back(vertexNormal);
        } else if (type == "f") {
            ::std::string line;
            ::std::getline(fileContent, line);
            ::std::stringstream fileContentLine(line);

            size_t index;
            char dummy;
            int counter = 0;

            while (fileContentLine >> index) {
                if ((counter + 1) % 3 != 0 || counter == 0)
                    fileContentLine >> dummy;
                index--;
                if (counter % 3 == 0) {
                    vertexBufferData.push_back(vertexPositions[index].x);
                    vertexBufferData.push_back(vertexPositions[index].y);
                    vertexBufferData.push_back(vertexPositions[index].z);
                } else if (counter % 3 == 1) {
                    vertexBufferData.push_back(vertexTextureCoordinates[index].x
                    );
                    vertexBufferData.push_back(vertexTextureCoordinates[index].y
                    );
                } else if (counter % 3 == 2) {
                    vertexBufferData.push_back(vertexNormals[index].x);
                    vertexBufferData.push_back(vertexNormals[index].y);
                    vertexBufferData.push_back(vertexNormals[index].z);
                }
                counter++;
            }
            if (counter / 3 % 4 == 0) {
                indexBufferData.push_back(numberOfVertexes + 0);
                indexBufferData.push_back(numberOfVertexes + 1);
                indexBufferData.push_back(numberOfVertexes + 2);
                indexBufferData.push_back(numberOfVertexes + 2);
                indexBufferData.push_back(numberOfVertexes + 0);
                indexBufferData.push_back(numberOfVertexes + 3);
                numberOfVertexes += 4;
            } else if (counter / 3 % 3 == 0) {
                indexBufferData.push_back(numberOfVertexes + 0);
                indexBufferData.push_back(numberOfVertexes + 1);
                indexBufferData.push_back(numberOfVertexes + 2);
                numberOfVertexes += 3;
            }
        } else {
            // Not really sure that it's the best way
            EXCEPTION(("Unknown tag in file: " + fileName).c_str());

            // Skipping unknown tag
            ::std::string line;
            ::std::getline(fileContent, line);
        }
        oldType = type;
    }
    if (!vertexBufferData.empty())
        primitivesArray.push_back(new Primitive(
            shaderProgramId, vertexBufferData, "v3v2v3", indexBufferData
        ));
}  // End of 'Model::parseObj' function

/* Class constructor.
 * ARGUMENTS:
 *   - model's shader program id:
 *       uint shaderProgramId_;
 *   - model's file name:
 *       const ::std::string &fileName.
 */
Model::Model(uint shaderProgramId_, const ::std::string &fileName)
    : shaderProgramId(shaderProgramId_),
      isVisible(true),
      renderType(VertexArray::renderType::TRIANGLES) {
    ::std::string fileFormat;
    for (size_t iterator = fileName.size() - 1;
         iterator > 0 && fileName[iterator] != '.'; iterator--)
        fileFormat.push_back(fileName[iterator]);
    ::std::reverse(fileFormat.begin(), fileFormat.end());
    if (fileFormat == "obj")
        parseObj(fileName);
    else
        assert("Wrong file format");
}  // End of 'Model::Model' function

/* Draw model function.
 * ARGUMENTS:
 *   - camera for rendering model:
 *      const Camera &camera;
 * RETURNS: None.
 */
void Model::onRender(const Camera &camera) const {
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
            glUniform3fv(uniformLocation, 1, &uniformValue.x);
    }
    for (auto &[uniformName, uniformPair] : shaderUniform4fv) {
        uniformLocation = glGetUniformLocation(shaderProgramId, uniformName);
        if (uniformLocation != -1)
            glUniformMatrix4fv(
                uniformLocation, 1, GL_FALSE, (float *)uniformPair.matrix
            );
    }
    uniformLocation =
        glGetUniformLocation(shaderProgramId, "modelTransformMatrix");
    if (uniformLocation != -1)
        glUniformMatrix4fv(
            uniformLocation, 1, GL_FALSE, (float *)transformMatrix.matrix
        );
    glUseProgram(0);
    for (auto &primitiveInstance : primitivesArray)
        primitiveInstance->onRender(camera);
}  // End of 'Model::onRender' function

// Class destructor
Model::~Model() {
    for (auto &primitive : primitivesArray)
        delete primitive;
    ::std::cout << "Clear model" << ::std::endl;
}  // End of 'Model::~Model' function

/* Attach shader program id to the model function.
 * ARGUMENTS:
 *   - shader program id:
 *       uint shaderProgramId_;
 * RETURNS: None.
 */
void Model::setShaderProgram(uint shaderProgramId_) {
    shaderProgramId = shaderProgramId_;
}  // End of 'Model::setShaderProgram' function

/* Get model's visibility flag function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (bool) - visibility flag.
 */
bool Model::getVisibility() const {
    return isVisible;
}  // End of 'Model::getVisibility' function

/* Set model's visibility flag function.
 * ARGUMENTS:
 *   - new flag:
 *       bool isVisible_;
 * RETURNS: None.
 */
void Model::setVisibility(bool isVisible_) {
    isVisible = isVisible_;
}  // End of 'Model::setVisibility' function

/* Get model rendering type function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (VertexArray::renderType) - rendering type.
 */
VertexArray::renderType Model::getRenderType() const {
    return renderType;
}  // End of 'Model::getRenderType' function

/* Set model rendering type function.
 * ARGUMENTS:
 *   - new rendering type:
 *       VertexArray::renderType renderType_:
 * RETURNS: None.
 */
void Model::setRenderType(VertexArray::renderType renderType_) {
    renderType = renderType_;
    for (auto &primitiveInstance : primitivesArray)
        primitiveInstance->renderType = renderType_;
}  // End of 'Model::setRenderType' function

/* Get number of primitives in model function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (int) - number of primitives in model.
 */
int Model::getNumberOfChildren() const {
    return static_cast<int>(primitivesArray.size());
}  // End of 'Model::getNumberOfChildren' function

/* Get primitive's pointer by index in model function.
 * ARGUMENTS:
 *   - index of the primitive:
 *       int index;
 * RETURNS:
 *   (Primitive *) - not-owning pointer to the primitive.
 */
Primitive *Model::getChild(int index) const {
    return primitivesArray[index];
}  // End of 'Model::getChild' function

/* Add uniform of one integer variable to the shader function.
 * ARGUMENTS:
 *   - uniform value:
 *       int uniformValue;
 *   - uniform name on the shader:
 *       const char *uniformName;
 * RETURNS: None.
 */
void Model::addUniform(int uniformValue, const char *uniformName) {
    shaderUniform1i[uniformName] = uniformValue;
}  // End of 'Model::addUniform' function

/* Add uniform of one float variable to the shader function.
 * ARGUMENTS:
 *   - uniform value:
 *       float uniformValue;
 *   - uniform name on the shader:
 *       const char *uniformName;
 * RETURNS: None.
 */
void Model::addUniform(float uniformValue, const char *uniformName) {
    shaderUniform1f[uniformName] = uniformValue;
}  // End of 'Model::addUniform' function

/* Add uniform of 3-component geom vector to the shader function.
 * ARGUMENTS:
 *   - uniform value:
 *       const math::vec3 &vector;
 *   - uniform name on the shader:
 *       const char *uniformName;
 * RETURNS: None.
 */
void Model::addUniform(const math::vec3 &vector, const char *uniformName) {
    shaderUniform3fv[uniformName] = vector;
}  // End of 'Model::addUniform' function

/* Add uniform of matrix4x4 variable to the shader function.
 * ARGUMENTS:
 *   - uniform value:
 *       const math::matr4 &uniformValue;
 *   - uniform name on the shader:
 *       const char *uniformName;
 *   - uniforms number:
 *       int uniformCount;
 * RETURNS: None.
 */
void Model::addUniform(const math::matr4 &matrix, const char *uniformName) {
    shaderUniform4fv[uniformName] = matrix;
}  // End of 'Model::addUniform' function
}  // namespace hse