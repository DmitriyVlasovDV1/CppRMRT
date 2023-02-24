#ifndef SCENE_HPP
#define SCENE_HPP

#include "../../../def.hpp"
#include "../buffers/buffer.hpp"
#include "../primitives/primitive.hpp"
#include "../shaders/shader.hpp"

namespace hse {
// Unit class declaration
class unit {
    // Friend classes
    friend class scene;

    bool isInitialized;                                // We can have a lot of copies of one unit,
                                                       // but we want to initialize it only once
    ::std::map<::std::string, shader *> shadersArray;  // Unit's shader programs array
    ::std::vector<primitive *> primitivesArray;        // Unit's primitives array
    ::std::vector<buffer *> buffersArray;              // Unit's buffers array

public:
    /* Create vertex buffer function.
     * ARGUMENTS:
     *   - buffer's data:
     *       const ::std::vector<float> &vertexBufferData;
     *   - buffer's format:
     *       const ::std::string &vertexBufferFormat;
     * NOTE: vertexBufferFormat - use default type or "v3v3v3v2" == vertex
     * position, color, normal, texture coordinate.
     */
    uint createVertexBuffer(const ::std::vector<float> &vertexBufferData, const ::std::string &vertexBufferFormat = "v3");

    /* Create index buffer function.
     * ARGUMENTS:
     *   - buffer's data;
     *       const ::std::vector<int> &indexBufferData.
     */
    uint createIndexBuffer(const ::std::vector<int> &indexBufferData);

    /* Create vertex array function.
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
    uint createVertexArray(const ::std::vector<float> &vertexBufferData,
                           const ::std::string &vertexBufferFormat,
                           const ::std::vector<int> &indexBufferData);

    /* Create shader storage buffer function.
     * ARGUMENTS:
     *   - buffer's data:
     *       const ::std::vector<T> &bufferData;
     *   - buffer's binding value:
     *       uint bufferBinding.
     */
    template <typename T>
    uint createShaderStorageBuffer(const ::std::vector<T> &bufferData, uint bufferBinding);

    /* Create shader function.
     * ARGUMENTS:
     *   - path to shader's realization (read shader class constructor note)
     *       const ::std::string &shaderPath;
     * RETURNS:
     *   (uint) - shader program id.
     */
    uint createShader(const ::std::string &shaderPath);

    /* Create primitive function.
     * ARGUMENTS:
     *   - path to primitive's shader:
     *       const ::std::string &shaderPath;
     *   - vertex buffer data:
     *       const ::std::vector<float> &vertexBuffer;
     *   - vertex buffer format:
     *       const ::std::string &vertexBufferFormat;
     *   - index buffer data:
     *       const ::std::vector<int> &indexBuffer;
     * RETURNS:
     *   (primitive *) - not-owning pointer to the created primitive;
     * NOTE: vertexBufferFormat - use default type or "v3v3v3v2" == vertex
     * position, color, normal, texture coordinate.
     */
    primitive *createPrimitive(
        const ::std::string &shaderPath,
        const ::std::vector<float> &vertexBuffer,
        const ::std::string &vertexBufferFormat = "v3v3",
        const ::std::vector<int> &indexBuffer = ::std::vector<int>()
    );

protected:
    // Class constructor
    explicit unit();

    /* Unit initialization pure-virtual function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual void init() = 0;

    /* Unit response pure-virtual function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual void response() = 0;

    /* Render unit function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void render() const;

    /* Clear unit function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void clear();

    // Class virtual destructor
    virtual ~unit() = default;
};  // End of 'unit' class

// Scene class declaration
class scene {
private:
    ::std::map<::std::string, unit *> unitsArray;  // Array of all scene's units
    bool sceneVisibility;                          // Flag indicating that the scene is visible
                                                   // default value is 'true'

public:
    // Class default constructor
    explicit scene();

    /* Initialize scene function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void init();


    /* Response scene function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void response();

    /* Add unit to the scene function.
     * ARGUMENTS:
     *   - unit name:
     *       const ::std::string &unitName;
     *   - unit instance:
     *       const unit *unitInstance;
     * RETURNS: None.
     */
    void addUnit(const ::std::string &unitName, const unit *unitInstance);

    /* Delete unit from the scene function.
     * ARGUMENTS:
     *   - unit name:
     *       const ::std::string &unitName;
     * RETURNS:
     *   (unit *) - not-owning pointer to deleted unit.
     */
    unit *deleteUnit(const ::std::string &unitName);

    /* Get scene status function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (bool) - true if scene is enabled, false if not.
     */
    [[nodiscard]] bool getSceneStatus() const;

    /* Set scene status function.
     * ARGUMENTS:
     *   - flag indicating that the scene is enabled:
     *       bool enableScene_;
     * RETURNS: None.
     */
    void setSceneStatus(bool enableScene_);

    // Class destructor
    ~scene();
};  // End of 'scene' class
}  // namespace hse

#endif  // SCENE_HPP
