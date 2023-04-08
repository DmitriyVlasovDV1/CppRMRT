#ifndef RENDER_HPP
#define RENDER_HPP

#include "../def.hpp"
#include "src/units/unit.hpp"
#include "src/figures/figure_scene.hpp"

// Project namespace
namespace hse {
// Render class declaration
class render {
public:
    // Key struct for callback
    struct key {
        int action;
        int mode;
    };  // end of 'key' struct

    ::std::map<int, key> keys;     // Keys map for input response
    static render renderInstance;  // Render instance
    FigureScene scene;             // Universal scene

    /* Initialized glew/glfw function.
     * ARGUMENTS:
     *   - window width and height:
     *       uint windowWidth_, windowHeight_;
     * RETURNS: None.
     */
    void init(uint windowWidth_ = 400, uint windowHeight_ = 400);

    /* Response window function.
     * ARGUMENTS: None.
     * RETURNS: None.
     * NOTE:
     *   If we want to create several windows - let's do it in far-far future.
     */
    void response();

    /* Add unit instance to the unit's array function.
     * ARGUMENTS:
     *   - unit name:
     *       const ::std::string &unitName;
     *   - unit instance:
     *       unit *unitInstance;
     * RETURNS: None.
     * NOTE:
     *   Returns value may be changed to (unit *) - not-owning pointer to
     * the scene, if we want to be able to copy scenes.
     */
    void addUnit(const ::std::string &unitName, unit *unitInstance);

    /* Get time value function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (float) - time value.
     */
    float getTime() const;

    /* Get pause flag function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (bool) - paused flag.
     */
    bool getPauseFlag() const;

    /* Set pause flag function.
     * ARGUMENTS:
     *   - new pause flag value:
     *       bool isPause_;
     * RETURNS: None.
     */
    void setPauseFlag(bool isPause_);

    /* Get delta time between frames function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (float) - delta time.
     */
    float getDeltaTime() const;

    /* Get window width function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (uint) - window width.
     */
    int getWindowWidth() const;

    /* Get window height function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (uint) - window height.
     */
    int getWindowHeight() const;

private:
    ::std::map<::std::string, unit *> unitsArray;  // Units map
    GLFWwindow *windowInstance;                    // Window instance

    // Render public variables (can be received by methods)
    uint windowWidth,       // Window width
        windowHeight;       // Window height
    float time, deltaTime;  // Time and delta time per frame
    bool isPause;           // Pause flag

    // Class default constructor
    explicit render();

    /* Class constructor.
     * ARGUMENTS:
     *   - window width and height:
     *       uint windowWidth_, windowHeight_.
     */
    explicit render(uint windowWidth_, uint windowHeight_);

    /* Resize window callback function.
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

    /* Keyboard response window callback function.
     * ARGUMENTS:
     *   - window instance:
     *       GLFWwindow *window;
     *   - key:
     *       int key;
     *   - key's scanned code:
     *       int scancode;
     *   - key's action:
     *       int action;
     *   - key's mods:
     *       int mods;
     * RETURNS: None.
     */
    static void keyboardCallback(
        GLFWwindow *window,
        int key,
        int scancode,
        int action,
        int mods
    );

    // Class destructor
    ~render();
};  // End of 'render' class
}  // namespace hse

#endif  // RENDER_HPP
