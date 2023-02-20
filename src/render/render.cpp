#include "render.hpp"

// Project namespace
namespace hse {
render render::renderInstance;

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
void render::frameBufferSizeCallback(
    GLFWwindow *window,
    int width,
    int height
) {
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
    time = 0;
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

        // Response all units
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto &[sceneName, sceneInstance] : scenesArray)
            if (sceneInstance->getSceneStatus()) sceneInstance->sceneResponse();

        // Render

        glfwSwapBuffers(windowInstance);
        glfwPollEvents();
    }
}  // End of 'render::response' function

// Class destructor
render::~render() {
    glfwDestroyWindow(windowInstance);
    glfwTerminate();
}  // End of 'render::~render' function

/* Add shader function.
 * ARGUMENTS:
 *   - path to shader's realization (read shader class constructor note)
 *       const ::std::string &shaderPath;
 * RETURNS:
 *   (shader *) - not-owning pointer to the created shader.
 */
shader *render::shaderAdd(const ::std::string &shaderPath) {
    if (shadersArray[shaderPath]) return shadersArray[shaderPath];
    shadersArray[shaderPath] = new shader(shaderPath);
    return shadersArray[shaderPath];
}  // End of 'render::shaderAdd' function

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
primitive *render::primitiveAdd(
    const ::std::string &shaderPath,
    const ::std::vector<float> &vertexBuffer,
    const ::std::string &vertexBufferFormat,
    const ::std::vector<int> &indexBuffer
) {
    primitivesArray.emplace_back(new primitive(
        shaderAdd(shaderPath), vertexBuffer, vertexBufferFormat, indexBuffer
    ));
    return primitivesArray.back();
}  // End of 'render::primitiveAdd' function

/* Get time function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (float) - time.
 */
[[nodiscard]] float render::getTime() const {
    return time;
}  // End of 'getTime' function

/* Get delta time function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (float) - delta time.
 */
[[nodiscard]] float render::getDeltaTime() const {
    return deltaTime;
}  // End of 'getDeltaTime' function

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