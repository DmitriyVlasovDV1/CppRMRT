#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "../../../../def.hpp"

// Project namespace
namespace hse {
// Vertex buffer class declaration
class VertexBuffer {
    uint bufferId;        // Id of each vertex buffer
    int sizeOfVertex;     // Size of each vertex in bytes (given by vertex format)
    size_t sizeOfBuffer;  // Size of full vertex buffer data in bytes

public:
    // Class default constructor
    explicit VertexBuffer();

    /* Class constructor.
     * ARGUMENTS:
     *   - buffer's data:
     *       const std::vector<float> &bufferData;
     *   - buffer's format:
     *       const std::string &bufferFormat;
     * NOTE: vertexBufferFormat - use default type or "v3v3v3v2" == vertex
     * position, color, normal, texture coordinate.
     */
    explicit VertexBuffer(const std::vector<float> &bufferData, const std::string &bufferFormat = "v3");

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

    /* Get vertex buffer id function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (uint) - vertex buffer id.
     */
    uint getBufferId() const;

    // Class destructor
    ~VertexBuffer();
};  // End of 'VertexBuffer' class

// Index buffer class declaration
class IndexBuffer {
    uint bufferId;        // Id of each index buffer
    size_t indexesCount;  // Number of indexes in buffer data

public:
    // Class default constructor
    explicit IndexBuffer();

    /* Class constructor.
     * ARGUMENTS:
     *   - buffer's data;
     *       const std::vector<int> &bufferData.
     */
    explicit IndexBuffer(const std::vector<int> &bufferData);

    /* Get indexes count function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (size_t) - indexes count.
     */
    size_t getIndexesCount() const;

    /* Get index buffer id function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (uint) - index buffer id;
     */
    uint getBufferId() const;

    // Class destructor
    ~IndexBuffer();
};  // End of 'IndexBuffer' class

// Vertex array class declaration
class VertexArray {
    uint vertexArrayId;                // Id of each vertex array
    VertexBuffer *vertexBufferObject;  // Vertex buffer pointer
    IndexBuffer *indexBufferObject;    // Index buffer pointer

public:
    // Enum for rendering type installation
    enum renderType { QUADS, TRIANGLES, TRIANGLES_STRIP, LINES };  // End of 'renderType' enum

    // Class default constructor
    explicit VertexArray();

    /* Class constructor.
     * ARGUMENTS:
     *   - vertex buffer data:
     *       const std::vector<float> &vertexBufferData;
     *   - vertex buffer format:
     *       const std::string &vertexBufferFormat;
     *   - index buffer data:
     *       const std::vector<int> &indexBufferData;
     * NOTE: vertexBufferFormat - use default type or "v3v3v3v2" == vertex
     * position, color, normal, texture coordinate.
     */
    explicit VertexArray(
        const std::vector<float> &vertexBufferData,
        const std::string &vertexBufferFormat,
        const std::vector<int> &indexBufferData
    );

    /* Render vertex array function.
     * ARGUMENTS:
     *   - type of buffer rendering:
     *       renderType type;
     * RETURNS: None;
     * NOTE: Not really recommend to use this method - use models/primitives.
     */
    void onRender(renderType type) const;

    /* Get vertex array id function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (uint) - vertex array id.
     */
    uint getVertexArrayId() const;

    // Class destructor
    ~VertexArray();
};  // End of 'VertexArray' class

// Shader storage buffer class declaration
class ShaderStorageBuffer {
    static std::unordered_set<uint> usedBindings;  // Used bindings set
    // (for not duplicating or lost previous data by some binding)
    uint bufferId;  // Id of each shader storage buffer

public:
    // Class default constructor
    explicit ShaderStorageBuffer() = default;

    /* Class constructor.
     * ARGUMENTS:
     *   - buffer's data:
     *       const std::vector<T> &bufferData;
     *   - buffer's binding value:
     *       uint bufferBinding.
     */
    template <typename T>
    ShaderStorageBuffer(const std::vector<T> &bufferData, uint bufferBinding) : bufferId(0) {
        if (usedBindings.count(bufferBinding))
            EXCEPTION(("SSBO by binding = " + std::to_string(bufferBinding) +
                       "; Try to create ssbo with already used binding")
                          .c_str());
        glGenBuffers(1, &bufferId);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, bufferId);
        glBufferData(GL_SHADER_STORAGE_BUFFER, bufferData.size() * sizeof(T), (void *)&bufferData[0], GL_DYNAMIC_COPY);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bufferBinding, bufferId);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }  // End of 'shaderStorageBuffer' function

    /* Set data to the already created ssbo function.
     * ARGUMENTS:
     *   - buffer's data:
     *       const std::vector<T> &bufferData;
     *   - buffer's binding value:
     *       uint bufferBinding.
     */
    template <typename T>
    void setData(const std::vector<T> &bufferData, uint bufferBinding) {
        if (usedBindings.count(bufferBinding))
            EXCEPTION(("SSBO by binding = " + std::to_string(bufferBinding) +
                       "; Try to create ssbo with already used binding")
                          .c_str());
        glGenBuffers(1, &bufferId);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, bufferId);
        glBufferData(GL_SHADER_STORAGE_BUFFER, bufferData.size() * sizeof(T), (void *)&bufferData[0], GL_DYNAMIC_COPY);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bufferBinding, bufferId);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }  // End of 'setData' function

    // TODO check function!!! -- it's working obviously
    /* Update data in already created ssbo function.
     * ARGUMENTS:
     *   - buffer's data:
     *       const std::vector<T> &bufferData;
     * RETURNS: None.
     */
    template <typename T>
    void updateData(const std::vector<T> &bufferData) {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, bufferId);
        glBufferData(GL_SHADER_STORAGE_BUFFER, bufferData.size() * sizeof(T), (void *)&bufferData[0], GL_DYNAMIC_COPY);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }  // End of 'updateData' function

    /* Check if binding is free (0 ssbo bound by this number) function.
     * ARGUMENTS:
     *   - binding value:
     *       uint binding;
     * RETURNS:
     *   (bool) - true if free, false - otherwise.
     */
    static bool checkBinding(uint binding);

    /* Get shader storage buffer id function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (uint) - shader storage buffer id.
     */
    uint getBufferId() const;

    // Class destructor
    ~ShaderStorageBuffer();
};  // End of 'ShaderStorageBuffer' class
}  // namespace hse

#endif  // BUFFER_HPP
