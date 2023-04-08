#include "render.hpp"

// Project namespace
namespace hse {
/* Resize window callback-function.
 * ARGUMENTS:
 *   - window instance:
 *       GLFWwindow *window;
 *   - window width and height:
 *       int width, height;
 * RETURNS: None.
 */
void frameBufferSizeCallback(GLFWwindow *window, int width, int height) {
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, width, height);
}  // End of 'frameBufferSizeCallback' function

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
void keyboardCallback(
    GLFWwindow *window,
    int key,
    int scancode,
    int action,
    int mods
) {
    keys[key] = {action, mods};
}  // End of 'keyboardCallback' function

/* Initializing glew/glfw function.
 * ARGUMENTS:
 *   - window width and height:
 *       uint windowWidth_, windowHeight_;
 * RETURNS: None.
 */
void Render::onCreate(uint windowWidth_, uint windowHeight_) {
    time = 0;
    deltaTime = 0;
    isPause = false;
    windowWidth = windowWidth_;
    windowHeight = windowHeight_;
    windowInstance = glfwCreateWindow(
        static_cast<int>(windowWidth), static_cast<int>(windowHeight),
        "HSE project", nullptr, nullptr
    );
    if (!windowInstance) {
        glfwTerminate();
        EXCEPTION("Error in glfw window creation");
    }
    glfwMakeContextCurrent(windowInstance);
    glfwSetFramebufferSizeCallback(windowInstance, frameBufferSizeCallback);
    glfwSetKeyCallback(windowInstance, keyboardCallback);
    glewExperimental = true;
    if (GLenum glewStatus = glewInit(); glewStatus != GLEW_OK) {
        ::std::cout << glewGetErrorString(glewStatus) << ::std::endl;
        // EXCEPTION("Error in glew initialization");
    }
    ::std::cout << "OpenGL: " << glGetString(GL_VERSION) << "\n";
    ::std::cout << "Shader language: "
                << glGetString(GL_SHADING_LANGUAGE_VERSION) << ::std::endl;

    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(-1);
    glClearColor(0, 0, 0, 1);
    glfwSetTime(time);
}  // End of 'Render::onCreate' function

/* Update window frame function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void Render::onUpdate() {
    // Initializing units
    for (auto &[unitName, unitInstance] : scenesArray) {
        unitInstance->mainCamera.setProjection(windowWidth, windowHeight);
        unitInstance->onCreate();
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

        // Update/Render all units
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        for (auto &[sceneName, sceneInstance] : scenesArray)
            if (sceneInstance->getVisibility()) {
                sceneInstance->mainCamera.setProjection(
                    windowWidth, windowHeight
                );
                sceneInstance->onUpdate();
                sceneInstance->onRender();
            }
        glFinish();
        glDisable(GL_DEPTH_TEST);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glfwSwapBuffers(windowInstance);
        glfwPollEvents();
    }
}  // End of 'Render::onUpdate' function

/* Add scene's instance to the scenes array function.
 * ARGUMENTS:
 *   - scene name:
 *       const ::std::string &sceneName;
 *   - scene instance:
 *       Scene *sceneInstance;
 * RETURNS: None.
 * NOTE:
 *   Returns value may be changed to (unit *) - not-owning pointer to
 * the scene, if we want to be able to copy scenes.
 */
void Render::addScene(const ::std::string &sceneName, Scene *sceneInstance) {
    scenesArray[sceneName] = sceneInstance;
}  // End of 'Render::addScene' function

// Class default constructor
Render::Render() : windowInstance(nullptr) {
    time = 0;
    deltaTime = 0;
    isPause = false;
}  // End of 'Render::Render' function

/* Class constructor.
 * ARGUMENTS:
 *   - window width and height:
 *       uint windowWidth, windowHeight;
 */
Render::Render(uint windowWidth_, uint windowHeight_)
    : windowInstance(nullptr) {
    onCreate(windowWidth_, windowHeight_);
}  // End of 'Render::Render' function

// Class destructor
Render::~Render() {
    for (auto &[sceneName, sceneInstance] : scenesArray) {
        sceneInstance->onDelete();
        delete sceneInstance;
    }
    glfwDestroyWindow(windowInstance);
    glfwTerminate();
    ::std::cout << "Clear render" << ::std::endl;
}  // End of 'Render::~Render' function
}  // namespace hse