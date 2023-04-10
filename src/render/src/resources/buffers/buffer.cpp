#include "buffer.hpp"

// Project namespace
namespace hse {
// Class default constructor
VertexBuffer::VertexBuffer() : bufferId(0), sizeOfVertex(0), sizeOfBuffer(0) {
}  // End of 'VertexBuffer::vertexBuffer' function

/* Class constructor.
 * ARGUMENTS:
 *   - buffer's data:
 *       const std::vector<float> &bufferData;
 *   - buffer's format:
 *       const std::string &bufferFormat;
 * NOTE: vertexBufferFormat - use default type or "v3v3v3v2" == vertex
 * position, color, normal, texture coordinate.
 */
VertexBuffer::VertexBuffer(const std::vector<float> &bufferData, const std::string &bufferFormat)
    : bufferId(0), sizeOfBuffer(bufferData.size() * sizeof(float)) {
    glGenBuffers(1, &bufferId);
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeOfBuffer), &bufferData[0], GL_STATIC_DRAW);

    std::vector<int> vertexBufferOffsets;
    int vertexBufferStride = 0;
    for (auto &sign : bufferFormat)
        if (sign != 'v') {
            vertexBufferOffsets.push_back(sign - '0');
            vertexBufferStride += sign - '0';
        }
    sizeOfVertex = vertexBufferStride * static_cast<int>(sizeof(float));  // Stride is measured by bytes

    int localOffset = 0;
    for (int offsetNumber = 0; offsetNumber < vertexBufferOffsets.size(); offsetNumber++) {
        glVertexAttribPointer(
            offsetNumber, vertexBufferOffsets[offsetNumber], GL_FLOAT, GL_FALSE, sizeOfVertex,
            (void *)static_cast<intptr_t>(localOffset)
        );
        glEnableVertexAttribArray(offsetNumber);
        localOffset += vertexBufferOffsets[offsetNumber] * static_cast<int>(sizeof(float));
    }
}  // End of 'VertexBuffer::vertexBuffer' function

/* Get size of one vertex function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (int) - vertex size.
 */
int VertexBuffer::getVertexSize() const {
    return sizeOfVertex;
}  // End of 'VertexBuffer::getVertexSize' function

/* Get size of buffer data function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (size_t) - size of buffer data.
 */
size_t VertexBuffer::getBufferSize() const {
    return sizeOfBuffer;
}  // End of 'VertexBuffer::getBufferSize' function

/* Get vertex buffer id function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (uint) - vertex buffer id.
 */
uint VertexBuffer::getBufferId() const {
    return bufferId;
}  // End of 'VertexBuffer::getBufferId' function

// Class destructor
VertexBuffer::~VertexBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &bufferId);
}  // End of 'VertexBuffer::~vertexBuffer' function

// Class default constructor
IndexBuffer::IndexBuffer() : bufferId(0), indexesCount(0) {
}  // End of 'IndexBuffer::IndexBuffer' function

/* Class constructor.
 * ARGUMENTS:
 *   - buffer's data;
 *       const std::vector<int> &bufferData.
 */
IndexBuffer::IndexBuffer(const std::vector<int> &bufferData) : bufferId(0) {
    indexesCount = bufferData.size();
    glGenBuffers(1, &bufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indexesCount * sizeof(int)), &bufferData[0], GL_STATIC_DRAW
    );
}  // End of 'IndexBuffer::IndexBuffer' function

/* Get indexes count function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (size_t) - indexes count.
 */
size_t IndexBuffer::getIndexesCount() const {
    return indexesCount;
}  // End of 'IndexBuffer::getIndexesCount' function

/* Get index buffer id function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (uint) - index buffer id;
 */
uint IndexBuffer::getBufferId() const {
    return bufferId;
}  // End of 'IndexBuffer::getBufferId' function

// Class destructor
IndexBuffer::~IndexBuffer() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &bufferId);
}  // End of 'IndexBuffer::~IndexBuffer' function

// Class default constructor
VertexArray::VertexArray() : vertexArrayId(0), vertexBufferObject(nullptr), indexBufferObject(nullptr) {
}  // End of 'VertexArray::VertexArray' function

/* Class constructor.
 * ARGUMENTS:
 *   - vertex buffer data:
 *       const std::vector<float> &vertexBuffer;
 *   - vertex buffer format:
 *       const std::string &vertexBufferFormat;
 *   - index buffer data:
 *       const std::vector<int> &indexBuffer;
 * NOTE: vertexBufferFormat - use default type or "v3v3v3v2" == vertex
 * position, color, normal, texture coordinate.
 */
VertexArray::VertexArray(
    const std::vector<float> &vertexBufferData,
    const std::string &vertexBufferFormat,
    const std::vector<int> &indexBufferData
)
    : vertexArrayId(0) {
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);
    vertexBufferObject = new VertexBuffer(vertexBufferData, vertexBufferFormat);
    if (!indexBufferData.empty()) indexBufferObject = new IndexBuffer(indexBufferData);
    glBindVertexArray(0);
}  // End of 'VertexArray::VertexArray' function

/* Render vertex array function.
 * ARGUMENTS:
 *   - type of buffer rendering:
 *       renderType type;
 * RETURNS: None.
 */
void VertexArray::onRender(renderType type) const {
    glBindVertexArray(vertexArrayId);
    if (indexBufferObject) {
        glDrawElements(
            type == TRIANGLES         ? GL_TRIANGLES
            : type == TRIANGLES_STRIP ? GL_TRIANGLE_STRIP
            : type == QUADS           ? GL_QUADS
                                      : GL_LINES,
            static_cast<int>(indexBufferObject->getIndexesCount()), GL_UNSIGNED_INT, nullptr
        );
    } else
        glDrawArrays(
            type == TRIANGLES         ? GL_TRIANGLES
            : type == TRIANGLES_STRIP ? GL_TRIANGLE_STRIP
            : type == QUADS           ? GL_QUADS
                                      : GL_LINES,
            0, static_cast<int>(vertexBufferObject->getBufferSize()) / vertexBufferObject->getVertexSize()
        );
    glBindVertexArray(0);
}  // End of 'VertexArray::onRender' function

/* Get vertex array id function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (uint) - vertex array id.
 */
uint VertexArray::getVertexArrayId() const {
    return vertexArrayId;
}  // End of 'VertexArray::getVertexArrayId' function

// Class destructor
VertexArray::~VertexArray() {
    glBindVertexArray(vertexArrayId);
    delete vertexBufferObject;
    delete indexBufferObject;
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vertexArrayId);
}  // End of 'VertexArray::~VertexArray' function

std::unordered_set<uint> ShaderStorageBuffer::usedBindings;

/* Check if binding is free (0 ssbo bound by this number) function.
 * ARGUMENTS:
 *   - binding value:
 *       uint binding;
 * RETURNS:
 *   (bool) - true if free, false - otherwise.
 */
bool ShaderStorageBuffer::checkBinding(uint binding) {
    return usedBindings.count(binding);
}  // End of 'ShaderStorageBuffer::checkBinding' function

/* Get shader storage buffer id function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (uint) - shader storage buffer id.
 */
uint ShaderStorageBuffer::getBufferId() const {
    return bufferId;
}  // End of 'ShaderStorageBuffer::getBufferId' function

// Class destructor
ShaderStorageBuffer::~ShaderStorageBuffer() {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    glDeleteBuffers(1, &bufferId);
}  // End of 'ShaderStorageBuffer::~ShaderStorageBuffer' function
}  // namespace hse