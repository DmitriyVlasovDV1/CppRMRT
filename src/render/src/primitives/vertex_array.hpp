#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP

#include "../../../def.hpp"

// Project namespace
namespace hse {
// Vertex buffer class declaration
class vertexArray {
    // Friend classes
    friend class primitive;

    uint vertexArrayId{};  // Vertex array id

    struct buffer {
        uint id{};                    // Id of buffer
        int type{};                   // Type of buffer
        ::std::string format;         // Buffer format
        int sizeOfBuffer{};           // Buffer size in bytes
        int sizeOfVertex{};           // One vertex size in bytes
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
    };  // End of 'buffer' class

    ::std::vector<buffer> Buffers = {
        buffer(0, GL_ARRAY_BUFFER, "v3v3", 0),
        buffer(0, GL_ELEMENT_ARRAY_BUFFER, "v1", 0)};  // All buffers in vertex
                                                       // array

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
}  // namespace hse

#endif  // VERTEX_ARRAY_HPP
