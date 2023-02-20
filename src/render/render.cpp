#include "render.hpp"

// Project namespace
namespace hse {
/* Add shader function.
 * ARGUMENTS:
 *   - path to shader's realization (read shader class constructor note)
 *       const ::std::string &shaderPath;
 * RETURNS:
 *   (uint) - added shader id.
 */
uint sourceManager::shaderAdd(const ::std::string &shaderPath) {
    if (shadersArray[shaderPath])
        return shadersArray[shaderPath]->getShaderProgramId();
    shadersArray[shaderPath] = new shader(shaderPath);
    return shadersArray[shaderPath]->getShaderProgramId();
}  // End of 'sourceManager::shaderAdd' function

/* Get source manager instance function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (sourceManager &) - source manager instance.
 */
sourceManager &sourceManager::getSourceManagerInstance() {
    static sourceManager sourceManagerInstance;
    return sourceManagerInstance;
}  // End of 'sourceManager::getSourceManagerInstance' function

// Class constructor
unit::unit() : isInitialized(false) {
}  // End of 'unit::unit' function

// Class default constructor
scene::scene() : enableScene(true) {
}  // End of 'scene' function

/* Initialize scene function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void scene::initScene() {
    for (auto &[unitName, unitInstance] : unitsArray)
        if (!unitInstance->isInitialized)
            unitInstance->initUnit(), unitInstance->isInitialized = true;
}

/* Add unit to the scene function.
 * ARGUMENTS:
 *   - unit name:
 *       ::std::string &&unitName;
 *   - unit instance:
 *       unit &&unitInstance;
 * RETURNS: None.
 */
void scene::addUnit(const ::std::string &unitName, const unit *unitInstance) {
    unitsArray[unitName] = const_cast<unit *>(unitInstance);
    initScene();
}  // End of 'scene::addUnit' function

/* Delete unit from the scene function.
 * ARGUMENTS:
 *   - unit name:
 *       const ::std::string &unitName;
 * RETURNS:
 *   (unit *) - not-owning pointer to deleted unit.
 */
unit *scene::deleteUnitFromScene(const ::std::string &unitName) {
    unit *tmp = unitsArray[unitName];
    unitsArray.erase(unitName);
    return tmp;
}  // End of 'scene::deleteUnitFromScene' function

/* Get scene status function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (bool) - true if scene is enabled, false if not.
 */
bool scene::getSceneStatus() const {
    return enableScene;
}  // End of 'scene::getSceneStatus' function

/* Set scene status function.
 * ARGUMENTS:
 *   - flag indicating that the scene is enabled:
 *       bool enableScene_;
 * RETURNS: None.
 */
void scene::setSceneStatus(bool enableScene_) {
    enableScene = enableScene_;
}  // End of 'scene::setSceneStatus' function

/* Response scene function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void scene::sceneResponse() {
    for (auto &[unitName, unitInstance] : unitsArray)
        unitInstance->responseUnit();
}  // End of 'scene::sceneResponse' function

// Class destructor
scene::~scene() {
    for (auto &[unitName, unitInstance] : unitsArray)
        delete unitInstance;
}  // End of 'scene::~scene' function

/* Class constructor.
 * ARGUMENTS:
 *   - window width and height:
 *       uint windowWidth, windowHeight;
 */
[[maybe_unused]] render::render(uint windowWidth_, uint windowHeight_) {
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
void render::frameBufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}  // End of 'framebuffer_size_callback' function

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
}  // End of 'render::init' function

/* Response single window function.
 * ARGUMENTS: None.
 * RETURNS: None.
 * NOTE:
 *   If we want to create several windows - let's do it in far-far future.
 */
void render::response() const {
    while (!glfwWindowShouldClose(windowInstance)) {
        glClear(GL_COLOR_BUFFER_BIT);

        for (auto &[sceneName, sceneInstance] : scenesArray)
            if (sceneInstance->getSceneStatus()) sceneInstance->sceneResponse();

        glfwSwapBuffers(windowInstance);
        glfwPollEvents();
    }
}  // End of 'render::response' function

// Class destructor
render::~render() {
    glfwDestroyWindow(windowInstance);
    glfwTerminate();
}  // End of 'render::~render' function

/* Get render instance function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (render &) - render instance;
 * NOTE:
 *   I am not sure that render class should be singleton object - may be
 * changed in not near future.
 */
render &render::getRenderInstance() {
    static render renderInstance;
    return renderInstance;
}  // End of 'render:getRenderInstance' function

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

/* Resize window function.
 * ARGUMENTS:
 *   - window width and height:
 *       uint windowWidth, windowHeight;
 * RETURNS: None.
 */
void render::resizeWindow(uint windowWidth_, uint windowHeight_) {
    windowWidth = windowWidth_;
    windowHeight = windowHeight_;
    glfwSetWindowSize(
        windowInstance, static_cast<int>(windowWidth),
        static_cast<int>(windowHeight)
    );
}  // End of 'render::resizeWindow' function

/* Add scene instance to scene's array function.
 * ARGUMENTS:
 *   - scene name:
 *       const ::std::string &sceneName;
 *   - scene instance:
 *       ::std::unique_ptr<scene> sceneInstance;
 * RETURNS: None.
 * NOTE:
 *   Returns value may be changed to (scene *) - not-owning pointer to the
 * scene, if we want to be able to copy scenes.
 */
void render::addScene(
    const ::std::string &sceneName,
    ::std::unique_ptr<scene> &sceneInstance
) {
    scenesArray[sceneName] = ::std::move(sceneInstance);
}  // End of 'render::addScene' function
}  // namespace hse