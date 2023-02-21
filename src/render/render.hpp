#ifndef RENDER_HPP
#define RENDER_HPP

#include "../def.hpp"
#include "src/primitives/primitive.hpp"
#include "src/scenes/scene.hpp"
#include "src/shaders/shader.hpp"

// Project namespace
namespace hse {
// Render class declaration
class render {
public:
    static render renderInstance;  // Render instance

private:
    // Source containers
    ::std::map<::std::string, ::std::unique_ptr<scene>> scenesArray;  // Scenes
                                                                      // map
    ::std::map<::std::string, shader *> shadersArray;  // All shader's programs
                                                       // array
    ::std::vector<primitive *> primitivesArray;        // All primitives array

    // Render public variables (can be received by methods)
    uint windowWidth{},            // Window width
        windowHeight{};            // Window height
    GLFWwindow *windowInstance{};  // Window instance (for now we can have only
                                   // one window)
    float time{}, deltaTime{};     // Time and delta time per frame

    // Class default constructors
    explicit render() = default;

    /* Resize window callback-function.
     * ARGUMENTS:
     *   - window instance:
     *       GLFWwindow *window;
     *   - window width and height:
     *       int width, height;
     * RETURNS: None.
     */
    static void frameBufferSizeCallback(
        GLFWwindow *window,
        int width,
        int height
    );

    /* Class constructor.
     * ARGUMENTS:
     *   - window width and height:
     *       uint windowWidth, windowHeight.
     */
    [[maybe_unused]] explicit render(uint windowWidth_, uint windowHeight_);

    // Class destructor
    ~render();

public:
    /* Initialized glew/glfw function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void init(uint windowWidth_ = 400, uint windowHeight_ = 400);

    /* Response single window function.
     * ARGUMENTS: None.
     * RETURNS: None.
     * NOTE:
     *   If we want to create several windows - let's do it in far-far future.
     */
    void response();

    /* Add scene instance to scene's array function.
     * ARGUMENTS:
     *   - scene name:
     *       const ::std::string &sceneName;
     *   - scene instance:
     *       ::std::unique_ptr<scene> &sceneInstance;
     * RETURNS: None.
     * NOTE:
     *   Returns value may be changed to (scene *) - not-owning pointer to
     * the scene, if we want to be able to copy scenes.
     */
    void addScene(
        const ::std::string &sceneName,
        ::std::unique_ptr<scene> &sceneInstance
    );

    /* Add shader function.
     * ARGUMENTS:
     *   - path to shader's realization (read shader class constructor note)
     *       const ::std::string &shaderPath;
     * RETURNS:
     *   (shader *) - not-owning pointer to the created shader.
     */
    shader *addShader(const ::std::string &shaderPath);

    /* Add primitive function.
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
    primitive *addPrimitive(
        const ::std::string &shaderPath,
        const ::std::vector<float> &vertexBuffer,
        const ::std::string &vertexBufferFormat = "v3v3",
        const ::std::vector<int> &indexBuffer = ::std::vector<int>()
    );

    /* Get time function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (float) - time.
     */
    [[nodiscard]] float &getTime();

    /* Get delta time function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (float) - delta time.
     */
    [[nodiscard]] float &getDeltaTime();

    /* Get window width function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (uint) - window width.
     */
    [[nodiscard]] uint getWindowWidth() const;

    /* Get window height function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (uint) - window height.
     */
    [[nodiscard]] uint getWindowHeight() const;
};  // End of 'render' class
}  // namespace hse

#endif  // RENDER_HPP
