#ifndef RENDER_HPP
#define RENDER_HPP

#include "../def.hpp"
#include "shaders/shader.hpp"

// Project namespace
namespace hse {
// Source manager class definition
class sourceManager {
    ::std::map<::std::string, shader *> shadersArray;  // All shader's programs
                                                       // array

    // Class default constructor
    explicit sourceManager() = default;

public:
    // Deleted copy-constructors
    sourceManager(const sourceManager &other) = delete;
    sourceManager(sourceManager &&other) = delete;
    sourceManager &operator=(const sourceManager &other) = delete;
    sourceManager &operator=(sourceManager &&other) = delete;

    /* Add shader function.
     * ARGUMENTS:
     *   - path to shader's realization (read shader class constructor note)
     *       const ::std::string &shaderPath;
     * RETURNS:
     *   (uint) - added shader id.
     */
    uint shaderAdd(const ::std::string &shaderPath);

    /* Get source manager instance function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (sourceManager &) - source manager instance.
     */
    static sourceManager &getSourceManagerInstance();
};  // End of 'sourceManager' class

// Unit class declaration
class unit {
    // Friend classes
    friend class scene;

private:
    bool isInitialized = false;

protected:
    // Class constructor
    explicit unit();

public:
    // Deleted copy-constructors
    unit(const unit &other) = delete;
    unit(unit &&other) = delete;
    unit &operator=(const unit &other) = delete;
    unit &operator=(unit &&other) = delete;

protected:
    /* Unit initialization pure-virtual function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual void initUnit() = 0;

    /* Unit response pure-virtual function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual void responseUnit() = 0;

    // Class pure-virtual destructor
    virtual ~unit() = default;
};  // End of 'unit' class

class scene {
private:
    ::std::map<::std::string, unit *> unitsArray;  // Array of
                                                   // all
                                                   // scene's
                                                   // units
    bool enableScene;  // Flag indicating that the scene is enabled
                       // default value is 'true'

public:
    // Class default constructor
    explicit scene();

    /* Initialize scene function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void initScene();

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
    unit *deleteUnitFromScene(const ::std::string &unitName);

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

    /* Response scene function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void sceneResponse();

    // Class destructor
    ~scene();
};  // End of 'scene' class

// Render class declaration
class render {
private:
    ::std::map<::std::string, ::std::unique_ptr<scene>> scenesArray;  // Scenes
                                                                      // map
    uint windowWidth{},            // window width
        windowHeight{};            // window height
    GLFWwindow *windowInstance{};  // Window instance (for now we can have only
                                   // one window)

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
    static void frameBufferSizeCallback(GLFWwindow *window, int width, int height);

    /* Class constructor.
     * ARGUMENTS:
     *   - window width and height:
     *       uint windowWidth, windowHeight.
     */
    [[maybe_unused]] explicit render(uint windowWidth_, uint windowHeight_);

public:
    // Deleted copy-constructors
    render(const render &other) = delete;
    render(render &&other) = delete;
    render &operator=(const render &other) = delete;
    render &operator=(render &&other) = delete;

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
    void response() const;

private:
    // Class destructor
    ~render();

public:
    /* Get render instance function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (render &) - render instance;
     * NOTE:
     *   I am not sure that render class should be singleton object - may be
     * changed in not near future.
     */
    static render &getRenderInstance();

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

    /* Resize window function.
     * ARGUMENTS:
     *   - window width and height:
     *       uint windowWidth, windowHeight;
     * RETURNS: None.
     */
    void resizeWindow(uint windowWidth_, uint windowHeight_);

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
};  // End of 'render' class
}  // namespace hse

#endif  // RENDER_HPP
