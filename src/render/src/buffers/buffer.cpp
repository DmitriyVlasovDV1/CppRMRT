#include "buffer.hpp"

// Project namespace
namespace hse {
// Class default constructor
Buffer::Buffer() : bufferId(0) {
}  // End of 'Buffer::Buffer' function

/* Get buffer id function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (uint) - buffer id;
 */
uint Buffer::getBufferId() const {
    return bufferId;
}  // End of 'Buffer::getBufferId' function

// Class default constructor
VertexBuffer::VertexBuffer() : sizeOfVertex(0), sizeOfBuffer(0) {
}  // End of 'VertexBuffer::vertexBuffer' function

/* Class constructor.
 * ARGUMENTS:
 *   - buffer's data:
 *       const ::std::vector<float> &bufferData;
 *   - buffer's format:
 *       const ::std::string &bufferFormat;
 * NOTE: vertexBufferFormat - use default type or "v3v3v3v2" == vertex
 * position, color, normal, texture coordinate.
 */
VertexBuffer::VertexBuffer(
    const ::std::vector<float> &bufferData,
    const ::std::string &bufferFormat
)
    : sizeOfBuffer(bufferData.size() * sizeof(float)) {
    glGenBuffers(1, &bufferId);
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    glBufferData(
        GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeOfBuffer), &bufferData[0],
        GL_STATIC_DRAW
    );

    ::std::vector<int> vertexBufferOffsets;
    int vertexBufferStride = 0;
    for (auto &sign : bufferFormat)
        if (sign != 'v') {
            vertexBufferOffsets.push_back(sign - '0');
            vertexBufferStride += sign - '0';
        }
    sizeOfVertex =
        vertexBufferStride * static_cast<int>(sizeof(float));  // Stride is
                                                               // measured by
                                                               // bytes

    int localOffset = 0;
    for (int offsetNumber = 0; offsetNumber < vertexBufferOffsets.size();
         offsetNumber++) {
        glVertexAttribPointer(
            offsetNumber, vertexBufferOffsets[offsetNumber], GL_FLOAT, GL_FALSE,
            sizeOfVertex, (void *)static_cast<intptr_t>(localOffset)
        );
        glEnableVertexAttribArray(offsetNumber);
        localOffset +=
            vertexBufferOffsets[offsetNumber] * static_cast<int>(sizeof(float));
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

// Class destructor
VertexBuffer::~VertexBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &bufferId);
    ::std::cout << "Clear vertex buffer" << ::std::endl;
}  // End of 'VertexBuffer::~vertexBuffer' function

// Class default constructor
IndexBuffer::IndexBuffer() : indexesCount(0) {
}  // End of 'IndexBuffer::IndexBuffer' function

/* Class constructor.
 * ARGUMENTS:
 *   - buffer's data;
 *       const ::std::vector<int> &bufferData.
 */
IndexBuffer::IndexBuffer(const ::std::vector<int> &bufferData) {
    indexesCount = bufferData.size();
    glGenBuffers(1, &bufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(indexesCount * sizeof(int)), &bufferData[0],
        GL_STATIC_DRAW
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

// Class destructor
IndexBuffer::~IndexBuffer() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &bufferId);
    ::std::cout << "Clear index buffer" << ::std::endl;
}  // End of 'IndexBuffer::~IndexBuffer' function

// Class default constructor
VertexArray::VertexArray()
    : vertexBufferObject(nullptr), indexBufferObject(nullptr) {
}  // End of 'VertexArray::VertexArray' function

/* Class constructor.
 * ARGUMENTS:
 *   - vertex buffer data:
 *       const ::std::vector<float> &vertexBuffer;
 *   - vertex buffer format:
 *       const ::std::string &vertexBufferFormat;
 *   - index buffer data:
 *       const ::std::vector<int> &indexBuffer;
 * NOTE: vertexBufferFormat - use default type or "v3v3v3v2" == vertex
 * position, color, normal, texture coordinate.
 */
VertexArray::VertexArray(
    const ::std::vector<float> &vertexBufferData,
    const ::std::string &vertexBufferFormat,
    const ::std::vector<int> &indexBufferData
) {
    glGenVertexArrays(1, &bufferId);
    glBindVertexArray(bufferId);
    vertexBufferObject = new VertexBuffer(vertexBufferData, vertexBufferFormat);
    if (!indexBufferData.empty())
        indexBufferObject = new IndexBuffer(indexBufferData);
    glBindVertexArray(0);
}  // End of 'VertexArray::VertexArray' function

/* Render vertex array function.
 * ARGUMENTS:
 *   - type of buffer rendering:
 *       renderType type;
 * RETURNS: None.
 */
void VertexArray::onRender(renderType type) const {
    glBindVertexArray(bufferId);
    if (indexBufferObject) {
        glDrawElements(
            type == TRIANGLES         ? GL_TRIANGLES
            : type == TRIANGLES_STRIP ? GL_TRIANGLE_STRIP
            : type == QUADS           ? GL_QUADS
                                      : GL_LINES,
            static_cast<int>(indexBufferObject->getIndexesCount()),
            GL_UNSIGNED_INT, nullptr
        );
    } else
        glDrawArrays(
            type == TRIANGLES         ? GL_TRIANGLES
            : type == TRIANGLES_STRIP ? GL_TRIANGLE_STRIP
            : type == QUADS           ? GL_QUADS
                                      : GL_LINES,
            0,
            static_cast<int>(vertexBufferObject->getBufferSize()) /
                vertexBufferObject->getVertexSize()
        );
    glBindVertexArray(0);
}  // End of 'VertexArray::render' function

// Class destructor
VertexArray::~VertexArray() {
    glBindVertexArray(bufferId);
    delete vertexBufferObject;
    delete indexBufferObject;
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &bufferId);
    ::std::cout << "Clear vertex array" << ::std::endl;
}  // End of 'VertexArray::~VertexArray' function

::std::unordered_set<uint> ShaderStorageBuffer::usedBindings;


// Class destructor
ShaderStorageBuffer::~ShaderStorageBuffer() {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    glDeleteBuffers(1, &bufferId);
    ::std::cout << "Clear shader storage buffer" << ::std::endl;
}  // End of 'ShaderStorageBuffer::~ShaderStorageBuffer' function
}  // namespace hse