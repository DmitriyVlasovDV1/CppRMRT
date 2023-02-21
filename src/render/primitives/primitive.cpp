#include "primitive.hpp"

namespace hse {
/* Class constructor.
 * ARGUMENTS:
 *   - buffer id:
 *       uint id_;
 *   - buffer type:
 *       int type_;
 *   - buffer format:
 *       ::std::string format_;
 *   - buffer size (in elements):
 *       int sizeOfBuffer_.
 */
vertexArray::buffer::buffer(
    uint id_,
    int type_,
    ::std::string format_,
    int sizeOfBuffer_
)
    : id(id_),
      type(type_),
      format(std::move(format_)),
      sizeOfBuffer(sizeOfBuffer_) {
}  // End of 'vertexArray::buffer::buffer' function

/* Buffer assignment operator function.
 * ARGUMENTS:
 *   - buffer to assign:
 *       const buffer &other;
 * RETURNS:
 *   (buffer &) - this;
 * NOTE: This operator is needed for using map.
 */
vertexArray::buffer &vertexArray::buffer::operator=(const buffer &other) {
    id = other.id;
    type = other.type;
    format = other.format;
    sizeOfBuffer = other.sizeOfBuffer;
    data = other.data;
    return *this;
}  // End of 'buffer::operator=' function

/* Class constructor.
 * ARGUMENTS:
 *   - vertex buffer data:
 *       const ::std::vector<float> &vertexBuffer;
 *   - vertex buffer format:
 *       const ::std::string &vertexBufferFormat;
 *   - index buffer data:
 *       const ::std::vector<int> indexBuffer;
 * NOTE: vertexBufferFormat - use default type or "v3v3v3v2" == vertex
 * position, color, normal, texture coordinate.
 */
vertexArray::vertexArray(
    const ::std::vector<float> &vertexBuffer,
    const ::std::string &vertexBufferFormat,
    const ::std::vector<int> &indexBuffer
) {
    Buffers[0].data = (void *)&vertexBuffer[0];
    Buffers[0].sizeOfBuffer =
        static_cast<int>(vertexBuffer.size()) * Buffers[0].sizeOfElement;
    Buffers[1].data = (void *)&indexBuffer[0];
    Buffers[1].sizeOfBuffer =
        static_cast<int>(indexBuffer.size()) * Buffers[1].sizeOfElement;
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);
    for (auto &buffer : Buffers) {
        if (!buffer.sizeOfBuffer) continue;
        glGenBuffers(1, &buffer.id);
        glBindBuffer(buffer.type, buffer.id);
        glBufferData(
            buffer.type, buffer.sizeOfBuffer, buffer.data, GL_STATIC_DRAW
        );
    }
    glBindBuffer(Buffers[0].type, Buffers[0].id);
    ::std::vector<int> vertexBufferOffsets;
    int vertexBufferStride = 0;
    for (auto &sign : vertexBufferFormat)
        if (sign != 'v') {
            vertexBufferOffsets.push_back(sign - '0');
            vertexBufferStride += sign - '0';
        }
    vertexBufferStride *= Buffers[0].sizeOfElement;  // Stride is
                                                     // measured by bytes
    int localOffset = 0;
    for (int offsetNumber = 0; offsetNumber < vertexBufferOffsets.size();
         offsetNumber++) {
        glVertexAttribPointer(
            offsetNumber, vertexBufferOffsets[offsetNumber], GL_FLOAT, GL_FALSE,
            vertexBufferStride, (void *)static_cast<intptr_t>(localOffset)
        );
        glEnableVertexAttribArray(offsetNumber);
        localOffset +=
            vertexBufferOffsets[offsetNumber] * Buffers[0].sizeOfElement;
    }
    glBindVertexArray(0);
}  // End of 'vertexArray::vertexArray' function

/* Draw vertex array function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void vertexArray::drawVertexArray() {
    glBindVertexArray(vertexArrayId);
    glBindBuffer(Buffers[0].type, Buffers[0].id);
    if (Buffers[1].sizeOfBuffer) {
        glBindBuffer(Buffers[1].type, Buffers[1].id);
        glDrawElements(
            GL_TRIANGLE_STRIP,
            Buffers[0].sizeOfBuffer / Buffers[0].sizeOfElement, GL_UNSIGNED_INT,
            nullptr
        );
    } else
        glDrawArrays(
            GL_TRIANGLES, 0, Buffers[0].sizeOfBuffer / Buffers[0].sizeOfElement
        );
    glBindVertexArray(0);
}  // End of 'vertexArray::drawVertexArray' function

// Class destructor
vertexArray::~vertexArray() {
    glBindVertexArray(vertexArrayId);
    for (auto &buffer : Buffers) {
        glBindBuffer(buffer.type, 0);
        glDeleteBuffers(1, &buffer.id);
    }
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vertexArrayId);
}  // End of 'vertexArray::~vertexArray' function

/* Class constructor.
 * ARGUMENTS:
 *   - primitive's shader pointer:
 *       shader *primitiveShader_;
 *   - vertex buffer data:
 *       const ::std::vector<float> &vertexBuffer;
 *   - vertex buffer format:
 *       const ::std::string &vertexBufferFormat;
 *   - index buffer data:
 *       const ::std::vector<int> &indexBuffer;
 * NOTE: vertexBufferFormat - use default type or "v3v3v3v2" == vertex
 * position, color, normal, texture coordinate.
 */
primitive::primitive(
    shader *primitiveShader_,
    const ::std::vector<float> &vertexBuffer,
    const ::std::string &vertexBufferFormat,
    const ::std::vector<int> &indexBuffer
)
    : primitiveShader(primitiveShader_) {
    primitiveVertexArrayInstance = ::std::make_unique<vertexArray>(
        vertexBuffer, vertexBufferFormat, indexBuffer
    );
}  // End of 'primitive::primitive' function

/* Draw primitive function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void primitive::primitiveDraw() {
    glUseProgram(primitiveShader->getShaderProgramId());
    primitiveShader->shaderApply();
    primitiveVertexArrayInstance->drawVertexArray();
    glUseProgram(0);
}  // End of 'primitive::primitiveDraw' function

// Class destructor
primitive::~primitive() {
    ::std::cout << "Clear primitive" << ::std::endl;
}  // End of 'primitive::~primitive' function
}  // namespace hse