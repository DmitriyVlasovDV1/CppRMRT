#ifndef RENDER_HPP
#define RENDER_HPP

#include "../def.hpp"
#include "src/scenes/scene.hpp"

// Project namespace
namespace hse {
// Render class declaration
class render {
public:
    static render renderInstance;  // Render instance

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

    /* Create scene instance to scene's array function.
     * ARGUMENTS:
     *   - scene name:
     *       const ::std::string &sceneName;
     *   - scene instance:
     *       scene *sceneInstance;
     * RETURNS: None.
     * NOTE:
     *   Returns value may be changed to (scene *) - not-owning pointer to
     * the scene, if we want to be able to copy scenes.
     */
    void createScene(const ::std::string &sceneName, scene *sceneInstance);

    /* Get time function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (const float &) - time.
     */
    [[nodiscard]] const float &getTime() const;

    /* Get delta time function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (const float &) - delta time.
     */
    [[nodiscard]] const float &getDeltaTime() const;

    /* Get window width function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (uint) - window width.
     */
    [[nodiscard]] const uint &getWindowWidth() const;

    /* Get window height function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (const uint &) - window height.
     */
    [[nodiscard]] const uint &getWindowHeight() const;

private:
    // Source containers
    ::std::map<::std::string, scene *> scenesArray;    // Scenes map

    // Render public variables (can be received by methods)
    uint windowWidth,            // Window width
        windowHeight;            // Window height
    GLFWwindow *windowInstance;  // Window instance (for now we can have only one window)
    float time, deltaTime;       // Time and delta time per frame

    // Class default constructors
    explicit render();

    /* Class constructor.
     * ARGUMENTS:
     *   - window width and height:
     *       uint windowWidth, windowHeight.
     */
    [[maybe_unused]] explicit render(uint windowWidth_, uint windowHeight_);

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

    // Class destructor
    ~render();
};  // End of 'render' class
}  // namespace hse

#endif  // RENDER_HPP
