#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include "../../def.hpp"
#include "../shaders/shader.hpp"

namespace hse {
// Vertex buffer class declaration
class vertexArray {
    uint vertexArrayId{};  // Vertex array id

    struct buffer {
        uint id{};                    // Id of buffer
        int type{};                   // Type of buffer
        ::std::string format;         // Buffer format
        int sizeOfBuffer{};           // Buffer size in bytes
        void *data{};                 // Buffer data pointer
        const int sizeOfElement = 4;  // Size of 1 element of buffer

        // Class default constructor
        explicit buffer() = default;

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
        explicit buffer(
            uint id_,
            int type_,
            ::std::string format_,
            int sizeOfBuffer_
        );
    };

    ::std::map<::std::string, buffer> Buffers = {
        {"vertex", buffer(0, GL_ARRAY_BUFFER, "v3v3", 0)},
        {"index", buffer(0, GL_ELEMENT_ARRAY_BUFFER, "v1", 0)}};  // All buffers
                                                                  // in vertex
                                                                  // array

public:
    // Class default constructor
    explicit vertexArray() = default;

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
    explicit vertexArray(
        const ::std::vector<float> &vertexBuffer,
        const ::std::string &vertexBufferFormat,
        const ::std::vector<int> &indexBuffer
    );

    /* Draw vertex array function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void drawVertexArray();

    // Class destructor
    ~vertexArray();
};  // End of 'vertexArray' class

// Primitive class declaration
class primitive {
    // Friend classes
    friend class render;

    vertexArray primitiveVertexArrayInstance;  // Vertex array instance
    shader *primitiveShader{};                 // Primitive's shader instance

    // Class default constructor
    explicit primitive() = default;

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
    explicit primitive(
        shader *primitiveShader_,
        const ::std::vector<float> &vertexBuffer,
        const ::std::string &vertexBufferFormat = "v3v3",
        const ::std::vector<int> &indexBuffer = ::std::vector<int>()
    );

    /* Draw primitive function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void primitiveDraw();
};  // End fo 'primitive' class
}  // namespace hse

#endif  // PRIMITIVE_HPP
