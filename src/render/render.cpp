#include "render.hpp"

// Project namespace
namespace hse {
render render::renderInstance;  // Render class singleton object for receiving
                                // public variables (time, delta time, window
                                // size, ect.)

/* Initialized glew/glfw function.
 * ARGUMENTS:
 *   - window width and height:
 *       uint windowWidth_, windowHeight_;
 * RETURNS: None.
 */
void render::init(uint windowWidth_, uint windowHeight_) {
    windowWidth = windowWidth_;
    windowHeight = windowHeight_;
    windowInstance = glfwCreateWindow(
        static_cast<int>(windowWidth), static_cast<int>(windowHeight),
        "HSE project", nullptr, nullptr
    );
    if (!windowInstance) {
        glfwTerminate();
        assert("Error in glfw window creation");
    }
    glfwMakeContextCurrent(windowInstance);
    glfwSetFramebufferSizeCallback(windowInstance, frameBufferSizeCallback);
    glfwSetKeyCallback(windowInstance, keyboardCallback);
    glewExperimental = true;
    if (!glewInit()) assert("Error in glew initialization");
    ::std::cout << "OpenGL: " << glGetString(GL_VERSION) << "\n";
    ::std::cout << "Shader language: "
                << glGetString(GL_SHADING_LANGUAGE_VERSION) << ::std::endl;

    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(-1);
    glClearColor(0, 0, 0, 1);
    glfwSetTime(time);
}  // End of 'render::init' function

/* Response window function.
 * ARGUMENTS: None.
 * RETURNS: None.
 * NOTE:
 *   If we want to create several windows - let's do it in far-far future.
 */
void render::response() {
    // Initializing units
    for (auto &[unitName, unitInstance] : unitsArray) {
        unitInstance->mainCamera.setProjection(windowWidth, windowHeight);
        unitInstance->init();
    }
    // Render
    while (!glfwWindowShouldClose(windowInstance)) {
        // Our timer
        if (!isPause) {
            deltaTime = static_cast<float>(glfwGetTime()) - time;
            time += deltaTime;
        } else {
            glfwSetTime(time);
        }
        // Calculating current FPS
        glfwSetWindowTitle(
            windowInstance,
            ("FPS: " + ::std::to_string(static_cast<int>(1 / deltaTime)))
                .c_str()
        );

        // Response/Render all units
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        for (auto &[unitName, unitInstance] : unitsArray)
            if (unitInstance->getVisibility()) {
                unitInstance->response();
                unitInstance->render();
            }
        glFinish();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDisable(GL_DEPTH_TEST);

        glfwSwapBuffers(windowInstance);
        glfwPollEvents();
    }
}  // End of 'render::response' function

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
void render::addUnit(const ::std::string &unitName, unit *unitInstance) {
    unitsArray[unitName] = unitInstance;
}  // End of 'render::addUnit' function

/* Get time value function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (float) - time value.
 */
float render::getTime() const {
    return time;
}  // End of 'render::getTime' function

/* Get pause flag function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (bool) - paused flag.
 */
bool render::getPauseFlag() const {
    return isPause;
}  // End of 'render::getPauseFlag' function

/* Set pause flag function.
 * ARGUMENTS:
 *   - new pause flag value:
 *       bool isPause_;
 * RETURNS: None.
 */
void render::setPauseFlag(bool isPause_) {
    isPause = isPause_;
}  // End of 'render::setPauseFlag' function

/* Get delta time between frames function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (float) - delta time.
 */
float render::getDeltaTime() const {
    return deltaTime;
}  // End of 'render::getDeltaTime' function

/* Get window width function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (uint) - window width.
 */
uint render::getWindowWidth() const {
    return windowWidth;
}  // End of 'render::getWindowWidth' function

/* Get window height function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (uint) - window height.
 */
uint render::getWindowHeight() const {
    return windowHeight;
}  // End of 'render::getWindowHeight' function

// Class default constructor
render::render()
    : windowWidth(0),
      windowHeight(0),
      windowInstance(nullptr),
      time(0),
      deltaTime(0),
      isPause(false) {
}  // End of 'render::render' function

/* Class constructor.
 * ARGUMENTS:
 *   - window width and height:
 *       uint windowWidth, windowHeight;
 */
render::render(uint windowWidth_, uint windowHeight_)
    : windowWidth(0),
      windowHeight(0),
      windowInstance(nullptr),
      time(0),
      deltaTime(0),
      isPause(false) {
    init(windowWidth_, windowHeight_);
}  // End of 'render::render' function

/* Resize window callback-function.
 * ARGUMENTS:
 *   - window instance:
 *       GLFWwindow *window;
 *   - window width and height:
 *       int width, height;
 * RETURNS: None.
 */
void render::frameBufferSizeCallback(
    GLFWwindow *window,
    int width,
    int height
) {
    renderInstance.windowWidth = width;
    renderInstance.windowHeight = height;
    for (auto &[unitName, unitInstance] : renderInstance.unitsArray)
        unitInstance->mainCamera.setProjection(width, height);
    glViewport(0, 0, width, height);
}  // End of 'render::frameBufferSizeCallback' function

/* Keyboard response window callback function.
 * ARGUMENTS:
 *   - window instance:
 *       GLFWwindow *window;
 *   - key:
 *       int key;
 *   - key's scaned code:
 *       int scancode;
 *   - key's action:
 *       int action;
 *   - key's mods:
 *       int mods;
 * RETURNS: None.
 */
void render::keyboardCallback(
    GLFWwindow *window,
    int key,
    int scancode,
    int action,
    int mods
) {
    renderInstance.keys[key] = {action, mods};
}  // End of 'render::keyboardCallback' function

// Class destructor
render::~render() {
    for (auto &[unitName, unitInstance] : unitsArray) {
        unitInstance->clear();
        delete unitInstance;
    }
    glfwDestroyWindow(windowInstance);
    glfwTerminate();
    ::std::cout << "Clear render" << ::std::endl;
}  // End of 'render::~render' function
}  // namespace hse