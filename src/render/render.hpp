#ifndef RENDER_HPP
#define RENDER_HPP

#include "../def.hpp"
#include "src/resources/scenes/scene.hpp"
#include "src/figures/figure_scene.hpp"

// Project namespace
namespace hse {
// Render class declaration
class Render {
    // Friend render instance factory
    friend void factory();

    std::vector<Scene *> scenesArray;  // Scenes array
    GLFWwindow *windowInstance;        // Window instance

public:
    static FigureScene scene;  // УНИВЕРСАЛЬНАЯ СЦЕНА

private:
    /* Initializing glew/glfw function.
     * ARGUMENTS:
     *   - window width and height:
     *       uint windowWidth_, windowHeight_;
     * RETURNS: None.
     */
    void onCreate(uint windowWidth_ = 400, uint windowHeight_ = 400);

    /* Start window response/render infinity cycle function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void startRenderLoop();

public:
    /* Add scene's instance to the scenes array function.
     * ARGUMENTS:
     *   - scene instance:
     *       Scene *sceneInstance;
     * RETURNS: None.
     * NOTE:
     *   Returns value may be changed to (Scene *) - not-owning pointer to
     * the scene, if we want to be able to copy scenes.
     */
    void addScene(Scene *sceneInstance);

private:
    // Class default constructor
    explicit Render();

    /* Class constructor.
     * ARGUMENTS:
     *   - window width and height:
     *       uint windowWidth_, windowHeight_.
     */
    explicit Render(uint windowWidth_, uint windowHeight_);

    // Class destructor
    ~Render();
};  // End of 'Render' class
}  // namespace hse

#endif  // RENDER_HPP
