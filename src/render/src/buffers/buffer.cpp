#include "buffer.hpp"

// Project namespace
namespace hse {
// Class default constructor
buffer::buffer() : bufferId(0) {
}  // End of 'buffer::buffer' function

/* Get buffer id function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (uint) - buffer id;
 */
uint buffer::getBufferId() const {
    return bufferId;
}  // End of 'buffer::getBufferId' function

// Class default constructor
vertexBuffer::vertexBuffer() : sizeOfVertex(0), sizeOfBuffer(0) {
}  // End of 'vertexBuffer::vertexBuffer' function

/* Class constructor.
 * ARGUMENTS:
 *   - buffer's data:
 *       const ::std::vector<float> &bufferData;
 *   - buffer's format:
 *       const ::std::string &bufferFormat;
 * NOTE: vertexBufferFormat - use default type or "v3v3v3v2" == vertex
 * position, color, normal, texture coordinate.
 */
vertexBuffer::vertexBuffer(
    const ::std::vector<float> &bufferData,
    const ::std::string &bufferFormat
)
    : sizeOfBuffer(bufferData.size() * sizeof(float)) {
    glGenBuffers(1, &bufferId);
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    glBufferData(
        GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeOfBuffer * sizeof(float)),
        (void *)&bufferData[0], GL_STATIC_DRAW
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
}  // End of 'vertexBuffer::vertexBuffer' function

/* Get size of one vertex function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (int) - vertex size.
 */
[[nodiscard]] int vertexBuffer::getVertexSize() const {
    return sizeOfVertex;
}  // End of 'vertexBuffer::getVertexSize' function

/* Get size of buffer data function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (size_t) - size of buffer data.
 */
[[nodiscard]] size_t vertexBuffer::getBufferSize() const {
    return sizeOfBuffer;
}  // End of 'vertexBuffer::getBufferSize' function

// Class destructor
vertexBuffer::~vertexBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &bufferId);
    ::std::cout << "Clear vertex buffer" << ::std::endl;
}  // End of 'vertexBuffer::~vertexBuffer' function

/* Class constructor.
 * ARGUMENTS:
 *   - buffer's data;
 *       const ::std::vector<int> &bufferData.
 */
indexBuffer::indexBuffer(const ::std::vector<int> &bufferData) {
    glGenBuffers(1, &bufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(bufferData.size() * sizeof(int)),
        (void *)&bufferData[0], GL_STATIC_DRAW
    );
}  // End of 'indexBuffer::indexBuffer' function

// Class destructor
indexBuffer::~indexBuffer() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &bufferId);
    ::std::cout << "Clear index buffer" << ::std::endl;
}  // End of 'indexBuffer::~indexBuffer' function

// Class default constructor
vertexArray::vertexArray()
    : vertexBufferObject(nullptr), indexBufferObject(nullptr) {
}  // End of 'vertexArray::vertexArray' function

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
vertexArray::vertexArray(
    const ::std::vector<float> &vertexBufferData,
    const ::std::string &vertexBufferFormat,
    const ::std::vector<int> &indexBufferData
) {
    glGenVertexArrays(1, &bufferId);
    glBindVertexArray(bufferId);
    vertexBufferObject = new vertexBuffer(vertexBufferData, vertexBufferFormat);
    if (!indexBufferData.empty())
        indexBufferObject = new indexBuffer(indexBufferData);
    glBindVertexArray(0);
}

/* Draw vertex array function.
 * ARGUMENTS:
 *   - type of buffer rendering:
 *       renderType type;
 * RETURNS: None.
 */
void vertexArray::render(renderType type) const {
    glBindVertexArray(bufferId);
    if (indexBufferObject) {
        glDrawElements(
            type == TRIANGLES ? GL_TRIANGLE_STRIP : GL_LINE_LOOP,
            static_cast<int>(vertexBufferObject->getBufferSize()) /
                vertexBufferObject->getVertexSize(),
            GL_UNSIGNED_INT, nullptr
        );
    } else
        glDrawArrays(
            type == TRIANGLES ? GL_TRIANGLES : GL_LINES, 0,
            static_cast<int>(vertexBufferObject->getBufferSize()) /
                vertexBufferObject->getVertexSize()
        );
    glBindVertexArray(0);
}

// Class destructor
vertexArray::~vertexArray() {
    glBindVertexArray(bufferId);
    delete vertexBufferObject;
    delete indexBufferObject;
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &bufferId);
    ::std::cout << "Clear vertex array" << ::std::endl;
}  // End of 'vertexArray::~vertexArray' function

/* Class constructor.
 * ARGUMENTS:
 *   - buffer's data:
 *       const ::std::vector<T> &bufferData;
 *   - buffer's binding value:
 *       uint bufferBinding.
 */
template <typename T>
shaderStorageBuffer::shaderStorageBuffer(
    const ::std::vector<T> &bufferData,
    uint bufferBinding
) {
    if (usedBindings.count(bufferBinding))
        assert(("SSBO by binding = " + ::std::to_string(bufferBinding) +
                "; Try to create ssbo with already used binding")
                   .c_str());
    glGenBuffers(1, &bufferId);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, bufferId);
    glBufferData(
        GL_SHADER_STORAGE_BUFFER, bufferData.size() * sizeof(T),
        (void *)&bufferData[0], GL_DYNAMIC_COPY
    );
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bufferBinding, bufferId);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}  // End of 'shaderStorageBuffer::shaderStorageBuffer' function

// Class destructor
shaderStorageBuffer::~shaderStorageBuffer() {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    glDeleteBuffers(1, &bufferId);
    ::std::cout << "Clear shader storage buffer" << ::std::endl;
}  // End of 'shaderStorageBuffer::~shaderStorageBuffer' function
}  // namespace hse