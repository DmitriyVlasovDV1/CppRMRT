#include "model.hpp"

namespace hse {
/* Parse *.obj file function.
 * ARGUMENTS:
 *   - model's file name:
 *       const std::string &fileName;
 * RETURNS: None.
 */
void Model::parseObj(const std::string &fileName) {
    std::ifstream fileContent("../data/models/" + fileName);
    if (!fileContent.is_open()) EXCEPTION(("Do not found model file: " + fileName).c_str());

    std::vector<math::vec3> vertexPositions, vertexNormals;
    std::vector<math::vec2> vertexTextureCoordinates;
    std::vector<float> vertexBufferData;
    std::vector<int> indexBufferData;
    std::map<std::string, Material> materialArray;

    std::string tag, oldTag, materialName;
    int numberOfVertexes = 0;
    while (fileContent >> tag) {
        if (tag == "o") {
            if (oldTag == "f" && !vertexBufferData.empty()) {
                primitivesArray.push_back(
                    std::make_unique<Primitive>(shaderProgramId, vertexBufferData, "v3v3v2v3", indexBufferData)
                );
                primitivesArray.back()->setRenderType(renderType);
                indexBufferData.clear();
                vertexBufferData.clear();
                vertexPositions.clear();
                vertexNormals.clear();
                vertexTextureCoordinates.clear();
            }
            std::string objectName;  // Just skip this
            fileContent >> objectName;
        } else if (tag == "v") {
            math::vec3 vertexPosition;
            fileContent >> vertexPosition.x;
            fileContent >> vertexPosition.y;
            fileContent >> vertexPosition.z;
            vertexPositions.push_back(vertexPosition);
        } else if (tag == "vt") {
            math::vec2 textureCoordinate;
            fileContent >> textureCoordinate.x;
            fileContent >> textureCoordinate.y;
            vertexTextureCoordinates.push_back(textureCoordinate);
        } else if (tag == "vn") {
            math::vec3 vertexNormal;
            fileContent >> vertexNormal.x;
            fileContent >> vertexNormal.y;
            fileContent >> vertexNormal.z;
            vertexNormals.push_back(vertexNormal);
        } else if (tag == "f") {
            std::string line;
            std::getline(fileContent, line);
            std::stringstream fileContentLine(line);

            size_t index;
            char dummy;
            int counter = 0;

            while (fileContentLine >> index) {
                if ((counter + 1) % 3 != 0 || counter == 0) fileContentLine >> dummy;
                index--;
                if (counter % 3 == 0) {
                    if (materialArray.empty()) {
                        vertexBufferData.push_back(0);
                        vertexBufferData.push_back(0);
                        vertexBufferData.push_back(0);
                    } else {
                        vertexBufferData.push_back(materialArray[materialName].color.x);
                        vertexBufferData.push_back(materialArray[materialName].color.y);
                        vertexBufferData.push_back(materialArray[materialName].color.z);
                    }

                    vertexBufferData.push_back(vertexPositions[index].x);
                    vertexBufferData.push_back(vertexPositions[index].y);
                    vertexBufferData.push_back(vertexPositions[index].z);
                } else if (counter % 3 == 1) {
                    vertexBufferData.push_back(vertexTextureCoordinates[index].x);
                    vertexBufferData.push_back(vertexTextureCoordinates[index].y);
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
        } else if (tag == "mtllib") {
            std::string materialFileName, tmp;
            fileContent >> materialFileName;

            for (auto &sign : fileName) {
                if (sign == '/') break;
                tmp += sign;
            }
            materialFileName = tmp + "/" + materialFileName;
            std::ifstream materialFileContent("../data/models/" + materialFileName);
            if (!materialFileContent.is_open())
                EXCEPTION(("Do not found material model file: " + materialFileName).c_str());
            std::string materialTag, name;
            while (materialFileContent >> materialTag) {
                if (materialTag == "newmtl") {
                    materialFileContent >> name;
                } else if (materialTag == "Kd") {
                    materialFileContent >> materialArray[name].color.x;
                    materialFileContent >> materialArray[name].color.y;
                    materialFileContent >> materialArray[name].color.z;
                }
            }
        } else if (tag == "usemtl") {
            fileContent >> materialName;
        } else {
            // Not really sure that it's the best way
            // EXCEPTION(("Unknown tag in file: " + fileName).c_str());

            // Skipping unknown tag
            ::std::string line;
            ::std::getline(fileContent, line);
        }
        oldTag = tag;
    }
    if (!vertexBufferData.empty())
        primitivesArray.push_back(
            std::make_unique<Primitive>(shaderProgramId, vertexBufferData, "v3v3v2v3", indexBufferData)
        );
}  // End of 'Model::parseObj' function

/* Class constructor.
 * ARGUMENTS:
 *   - model's shader program id:
 *       uint shaderProgramId_;
 *   - model's file name:
 *       const std::string &fileName.
 */
Model::Model(uint shaderProgramId_, const std::string &fileName) {
    shaderProgramId = shaderProgramId_;
    isVisible = true;
    renderType = VertexArray::renderType::TRIANGLES;

    std::string fileFormat;
    for (size_t iterator = fileName.size() - 1; iterator > 0 && fileName[iterator] != '.'; iterator--)
        fileFormat.push_back(fileName[iterator]);
    std::reverse(fileFormat.begin(), fileFormat.end());
    if (fileFormat == "obj")
        parseObj(fileName);
    else
        EXCEPTION("\nTrying to load '" + fileName + "':\n" + "This file format is not supported: " + fileFormat);
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
    // Handle all non-constant uniforms
    {
        for (auto &[uniformName, uniformValue] : shaderUniform1i) {
            uniformLocation = glGetUniformLocation(shaderProgramId, uniformName);
            if (uniformLocation != -1) glUniform1i(uniformLocation, *uniformValue);
        }
        for (auto &[uniformName, uniformValue] : shaderUniform1f) {
            uniformLocation = glGetUniformLocation(shaderProgramId, uniformName);
            if (uniformLocation != -1) glUniform1f(uniformLocation, *uniformValue);
        }
        for (auto &[uniformName, uniformValue] : shaderUniform3fv) {
            uniformLocation = glGetUniformLocation(shaderProgramId, uniformName);
            if (uniformLocation != -1) glUniform3fv(uniformLocation, 1, &(*uniformValue).x);
        }
        for (auto &[uniformName, uniformPair] : shaderUniform4fv) {
            uniformLocation = glGetUniformLocation(shaderProgramId, uniformName);
            if (uniformLocation != -1) glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, (float *)(*uniformPair).matrix);
        }
    }

    // Handle all constant uniforms
    {
        for (auto &[uniformName, uniformValue] : shaderUniform1iConstant) {
            uniformLocation = glGetUniformLocation(shaderProgramId, uniformName);
            if (uniformLocation != -1) glUniform1i(uniformLocation, uniformValue);
        }
        for (auto &[uniformName, uniformValue] : shaderUniform1fConstant) {
            uniformLocation = glGetUniformLocation(shaderProgramId, uniformName);
            if (uniformLocation != -1) glUniform1f(uniformLocation, uniformValue);
        }
        for (auto &[uniformName, uniformValue] : shaderUniform3fvConstant) {
            uniformLocation = glGetUniformLocation(shaderProgramId, uniformName);
            if (uniformLocation != -1) glUniform3fv(uniformLocation, 1, &uniformValue.x);
        }
        for (auto &[uniformName, uniformPair] : shaderUniform4fvConstant) {
            uniformLocation = glGetUniformLocation(shaderProgramId, uniformName);
            if (uniformLocation != -1) glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, (float *)uniformPair.matrix);
        }
    }

    // Common uniforms for each model
    uniformLocation = glGetUniformLocation(shaderProgramId, "modelTransformMatrix");
    if (uniformLocation != -1) glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, (float *)transformMatrix.matrix);
    glUseProgram(0);

    // Render model
    for (auto &primitiveInstance : primitivesArray)
        if (primitiveInstance->getVisibility()) primitiveInstance->onRender(camera);
}  // End of 'Model::onRender' function

// Class destructor
Model::~Model() {
    for (auto &primitive : primitivesArray)
        primitive.reset();
}  // End of 'Model::~Model' function

/* Set primitive rendering type function.
 * ARGUMENTS:
 *   - new rendering type:
 *       VertexArray::renderType renderType_:
 * RETURNS: None.
 */
void Model::setRenderType(VertexArray::renderType renderType_) {
    renderType = renderType_;
    for (auto &primitive : primitivesArray)
        primitive->setRenderType(renderType_);
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
    return primitivesArray[index].get();
}  // End of 'Model::getChild' function
}  // namespace hse