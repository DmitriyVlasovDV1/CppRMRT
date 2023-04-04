#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "../../../def.hpp"

// Project namespace
namespace hse {
// Buffer class declaration
class buffer {
    // Friend classes
    friend class unit;

public:
    // Enum for rendering type installation
    enum renderType { TRIANGLES, LINES };  // End of 'renderType' enum

protected:
    uint bufferId;  // Buffer id

    // Class default constructor
    explicit buffer();

    // Class destructor
    virtual ~buffer() = default;

public:
    /* Get buffer id function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (uint) - buffer id;
     */
    [[nodiscard]] uint getBufferId() const;
};  // End of 'buffer' class

// Vertex buffer class declaration
class vertexBuffer final : public buffer {
    // Friend classes
    friend class unit;
    friend class primitive;
    friend class vertexArray;

    int sizeOfVertex;
    size_t sizeOfBuffer;

    // Class default constructor
    explicit vertexBuffer();

    /* Class constructor.
     * ARGUMENTS:
     *   - buffer's data:
     *       const ::std::vector<float> &bufferData;
     *   - buffer's format:
     *       const ::std::string &bufferFormat;
     * NOTE: vertexBufferFormat - use default type or "v3v3v3v2" == vertex
     * position, color, normal, texture coordinate.
     */
    explicit vertexBuffer(
        const ::std::vector<float> &bufferData,
        const ::std::string &bufferFormat = "v3"
    );

public:
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
    ~vertexBuffer() final;
};  // End of 'vertexBuffer' class

// Index buffer class declaration
class indexBuffer final : public buffer {
    // Friend classes
    friend class unit;
    friend class primitive;
    friend class vertexArray;

    size_t indexesCount;  // Number of indexes in buffer data

    // Class default constructor
    explicit indexBuffer() = default;

    /* Class constructor.
     * ARGUMENTS:
     *   - buffer's data;
     *       const ::std::vector<int> &bufferData.
     */
    explicit indexBuffer(const ::std::vector<int> &bufferData);

public:
    // Class destructor
    ~indexBuffer() final;
};  // End of 'indexBuffer' class

// Vertex array class declaration
class vertexArray final : public buffer {
    // Friend classes
    friend class unit;
    friend class primitive;

    vertexBuffer *vertexBufferObject;
    indexBuffer *indexBufferObject;

    // Class default constructor
    explicit vertexArray();

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
    explicit vertexArray(
        const ::std::vector<float> &vertexBufferData,
        const ::std::string &vertexBufferFormat,
        const ::std::vector<int> &indexBufferData
    );

public:
    /* Draw vertex array function.
     * ARGUMENTS:
     *   - type of buffer rendering:
     *       renderType type;
     * RETURNS: None.
     */
    void render(renderType type) const;

    // Class destructor
    ~vertexArray() final;
};  // End of 'vertexArray' class

// Shader storage buffer class declaration
class shaderStorageBuffer final : public buffer {
    // Friend classes
    friend class unit;

    ::std::unordered_set<uint> usedBindings;  // Used bindings set

public: // TODO
    // Class default constructor
    explicit shaderStorageBuffer() = default;

    /* Class constructor.
     * ARGUMENTS:
     *   - buffer's data:
     *       const ::std::vector<T> &bufferData;
     *   - buffer's binding value:
     *       uint bufferBinding.
     */
    template <typename T>
    explicit shaderStorageBuffer(
        const ::std::vector<T> &bufferData,
        uint bufferBinding
    );

    // Class destructor
    ~shaderStorageBuffer() final;
};  // End of 'vertexArray' class
}  // namespace hse

#endif  // BUFFER_HPP
