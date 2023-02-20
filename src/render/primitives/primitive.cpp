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
    Buffers["vertex"].data = (void *)vertexBuffer.data();
    Buffers["vertex"].sizeOfBuffer = static_cast<int>(vertexBuffer.size());
    Buffers["index"].data = (void *)indexBuffer.data();
    Buffers["index"].sizeOfBuffer = static_cast<int>(indexBuffer.size());
    glGenBuffers(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);
    for (auto &[bufferName, buffer] : Buffers) {
        if (!buffer.sizeOfBuffer) continue;
        glGenBuffers(1, &buffer.id);
        glBindBuffer(buffer.type, buffer.id);
        glBufferData(
            buffer.type, buffer.sizeOfBuffer, buffer.data, GL_STATIC_DRAW
        );
    }
    glBindBuffer(Buffers["vertex"].type, Buffers["vertex"].id);
    ::std::vector<int> vertexBufferOffsets;
    int vertexBufferStride = 0;
    for (auto &sign : vertexBufferFormat)
        if (sign != 'v') {
            vertexBufferOffsets.push_back(sign - '0');
            vertexBufferStride += sign - '0';
        }
    vertexBufferStride *= Buffers["vertex"].sizeOfElement;  // Stride is
                                                            // measured by bytes
    // type: "v3v3"
    // VB:     |{ppp|ccc}|{ppp|ccc}|{ppp|ccc}|{ppp|ccc}|
    // stride:  {.......}
    // offset:  {...}
    int localOffset = 0;
    for (int offsetNumber = 0; offsetNumber < vertexBufferOffsets.size();
         offsetNumber++) {
        glVertexAttribPointer(
            offsetNumber, vertexBufferOffsets[offsetNumber], GL_FLOAT, GL_FALSE,
            vertexBufferStride, (void *)static_cast<intptr_t>(localOffset)
        );
        glEnableVertexAttribArray(offsetNumber);
    }
    glBindVertexArray(0);
}  // End of 'vertexArray::vertexArray' function

/* Draw vertex array function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void vertexArray::drawVertexArray() {
    glBindVertexArray(vertexArrayId);
    if (Buffers["index"].sizeOfBuffer) {
        glBindBuffer(Buffers["index"].type, Buffers["index"].id);
        glDrawElements(GL_TRIANGLE_STRIP, 3, GL_UNSIGNED_INT, nullptr);
    } else {
        glBindBuffer(Buffers["vertex"].type, Buffers["vertex"].id);
        glDrawArrays(GL_TRIANGLES, 0, Buffers["vertex"].sizeOfBuffer);
    }
    glBindVertexArray(0);
    glUseProgram(0);
}  // End of 'vertexArray::drawVertexArray' function

// Class destructor
vertexArray::~vertexArray() {
    glBindVertexArray(vertexArrayId);
    for (auto &[bufferName, buffer] : Buffers) {
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
    primitiveVertexArrayInstance =
        vertexArray(vertexBuffer, vertexBufferFormat, indexBuffer);
}  // End of 'primitive::primitive' function

/* Draw primitive function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void primitive::primitiveDraw() {
    primitiveShader->shaderApply();
    primitiveVertexArrayInstance.drawVertexArray();
}  // End of 'primitive::primitiveDraw' function
}  // namespace hse