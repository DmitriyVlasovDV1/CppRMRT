#include "render.hpp"

// Project namespace
namespace hse {
render render::renderInstance;

/* Initialized glew/glfw function.
 * ARGUMENTS: None.
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
    glewExperimental = true;
    if (!glewInit()) assert("Error in glew initialization");
    ::std::cout << "OpenGL: " << glGetString(GL_VERSION) << "\n";
    ::std::cout << "Shader language: "
                << glGetString(GL_SHADING_LANGUAGE_VERSION) << ::std::endl;
    glClearColor(0.3f, 0.5f, 0.7f, 1.0f);
    glfwSetTime(time);
}  // End of 'render::init' function

/* Response single window function.
 * ARGUMENTS: None.
 * RETURNS: None.
 * NOTE:
 *   If we want to create several windows - let's do it in far-far future.
 */
void render::response() {
    while (!glfwWindowShouldClose(windowInstance)) {
        // Our timer
        deltaTime = static_cast<float>(glfwGetTime()) - time;
        time += deltaTime;
        glfwSetWindowTitle(
            windowInstance,
            ("FPS: " + ::std::to_string(static_cast<int>(1 / deltaTime)))
                .c_str()
        );

        for (auto &[sceneName, sceneInstance] : scenesArray)
            if (sceneInstance->getSceneStatus()) sceneInstance->response();

        glfwSwapBuffers(windowInstance);
        glfwPollEvents();
    }
}  // End of 'render::response' function

// Class destructor
render::~render() {
    for (auto &[sceneName, sceneInstance] : scenesArray)
        delete sceneInstance;
    glfwDestroyWindow(windowInstance);
    glfwTerminate();
    ::std::cout << "Clear render" << ::std::endl;
}  // End of 'render::~render' function

/* Get time function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (float &) - time.
 */
[[nodiscard]] const float &render::getTime() const {
    return time;
}  // End of 'getTime' function

/* Get delta time function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (const float &) - delta time.
 */
[[nodiscard]] const float &render::getDeltaTime() const {
    return deltaTime;
}  // End of 'getDeltaTime' function

/* Get window width function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (const uint &) - window width.
 */
[[nodiscard]] const uint &render::getWindowWidth() const {
    return windowWidth;
}  // End of 'render::getWindowWidth' function

/* Get window height function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (const uint &) - window height.
 */
[[nodiscard]] const uint &render::getWindowHeight() const {
    return windowHeight;
}  // End of 'render::getWindowHeight' function

/* Add scene instance to scene's array function.
 * ARGUMENTS:
 *   - scene name:
 *       const ::std::string &sceneName;
 *   - scene instance:
 *       scene *sceneInstance;
 * RETURNS: None.
 * NOTE:
 *   Returns value may be changed to (scene *) - not-owning pointer to the
 * scene, if we want to be able to copy scenes.
 */
void render::createScene(
    const ::std::string &sceneName,
    scene *sceneInstance
) {
    scenesArray[sceneName] = sceneInstance;
}  // End of 'render::addScene' function

// Class default constructors
render::render()
    : windowWidth(0),
      windowHeight(0),
      windowInstance(nullptr),
      time(0),
      deltaTime(0) {
}  // End of 'render::render' function

/* Class constructor.
 * ARGUMENTS:
 *   - window width and height:
 *       uint windowWidth, windowHeight;
 */
[[maybe_unused]] render::render(uint windowWidth_, uint windowHeight_)
    : windowWidth(0),
      windowHeight(0),
      windowInstance(nullptr),
      time(0),
      deltaTime(0) {
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
    glViewport(0, 0, width, height);
}  // End of 'framebuffer_size_callback' function
}  // namespace hse