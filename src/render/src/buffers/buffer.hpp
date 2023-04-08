#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "../../../def.hpp"

// Project namespace
namespace hse {
// Buffer class declaration
class Buffer {
protected:
    uint bufferId;  // Buffer id

public:
    // Class default constructor
    explicit Buffer();

    // Class destructor
    virtual ~Buffer() = default;

    /* Get buffer id function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (uint) - buffer id;
     */
    uint getBufferId() const;
};  // End of 'Buffer' class

// Vertex buffer class declaration
class VertexBuffer final : public Buffer {
    int sizeOfVertex;  // Size of each vertex in bytes (given by vertex format)
    size_t sizeOfBuffer;  // Size of full vertex buffer data in bytes

public:
    // Class default constructor
    explicit VertexBuffer();

    /* Class constructor.
     * ARGUMENTS:
     *   - buffer's data:
     *       const ::std::vector<float> &bufferData;
     *   - buffer's format:
     *       const ::std::string &bufferFormat;
     * NOTE: vertexBufferFormat - use default type or "v3v3v3v2" == vertex
     * position, color, normal, texture coordinate.
     */
    explicit VertexBuffer(
        const ::std::vector<float> &bufferData,
        const ::std::string &bufferFormat = "v3"
    );

    /* Get size of one vertex function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (int) - vertex size.
     */
    int getVertexSize() const;

    /* Get size of buffer data function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (size_t) - size of buffer data.
     */
    size_t getBufferSize() const;

    // Class destructor
    ~VertexBuffer() final;
};  // End of 'VertexBuffer' class

// Index buffer class declaration
class IndexBuffer final : public Buffer {
    size_t indexesCount;  // Number of indexes in buffer data

public:
    // Class default constructor
    explicit IndexBuffer();

    /* Class constructor.
     * ARGUMENTS:
     *   - buffer's data;
     *       const ::std::vector<int> &bufferData.
     */
    explicit IndexBuffer(const ::std::vector<int> &bufferData);

    /* Get indexes count function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (size_t) - indexes count.
     */
    size_t getIndexesCount() const;

    // Class destructor
    ~IndexBuffer() final;
};  // End of 'IndexBuffer' class

// Vertex array class declaration
class VertexArray final : public Buffer {
    VertexBuffer *vertexBufferObject;  // Vertex buffer pointer
    IndexBuffer *indexBufferObject;    // Index buffer pointer

public:
    // Class default constructor
    explicit VertexArray();

    /* Class constructor.
     * ARGUMENTS:
     *   - vertex buffer data:
     *       const ::std::vector<float> &vertexBufferData;
     *   - vertex buffer format:
     *       const ::std::string &vertexBufferFormat;
     *   - index buffer data:
     *       const ::std::vector<int> &indexBufferData;
     * NOTE: vertexBufferFormat - use default type or "v3v3v3v2" == vertex
     * position, color, normal, texture coordinate.
     */
    explicit VertexArray(
        const ::std::vector<float> &vertexBufferData,
        const ::std::string &vertexBufferFormat,
        const ::std::vector<int> &indexBufferData
    );

    // Enum for rendering type installation
    enum renderType {
        QUADS,
        TRIANGLES,
        TRIANGLES_STRIP,
        LINES
    };  // End of 'renderType' enum

    /* Render vertex array function.
     * ARGUMENTS:
     *   - type of buffer rendering:
     *       renderType type;
     * RETURNS: None.
     */
    void onRender(renderType type) const;

    // Class destructor
    ~VertexArray() final;
};  // End of 'VertexArray' class

// Shader storage buffer class declaration
class ShaderStorageBuffer final : public Buffer {
    static ::std::unordered_set<uint> usedBindings;  // Used bindings set (for
                                                     // not duplicating or lost
                                                     // previous data by some
                                                     // binding)

public:
    // Class default constructor
    explicit ShaderStorageBuffer() = default;

    /* Class constructor.
     * ARGUMENTS:
     *   - buffer's data:
     *       const ::std::vector<T> &bufferData;
     *   - buffer's binding value:
     *       uint bufferBinding.
     */
    template <typename T>
    explicit ShaderStorageBuffer(
        const ::std::vector<T> &bufferData,
        uint bufferBinding
    );

    /* Check if binding is free (0 ssbo bound by this number) function.
     * ARGUMENTS:
     *   - binding value:
     *       uint binding;
     * RETURNS:
     *   (bool) - true if free, false - otherwise.
     */
    static bool checkBinding(uint binding);

    // Class destructor
    ~ShaderStorageBuffer() final;
};  // End of 'ShaderStorageBuffer' class
}  // namespace hse

#endif  // BUFFER_HPP
