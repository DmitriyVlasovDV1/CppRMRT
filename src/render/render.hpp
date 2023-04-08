#ifndef RENDER_HPP
#define RENDER_HPP

#include "../def.hpp"
#include "src/scenes/scene.hpp"
#include "src/figures/figure_scene.hpp"

// Project namespace
namespace hse {
// Render class declaration
class Render {
public: // TODO
    // Friend render instance factory
    friend void factory();

    ::std::map<::std::string, Scene *> scenesArray;  // Scenes map
    GLFWwindow *windowInstance;                      // Window instance
    static FigureScene scene;

    /* Initializing glew/glfw function.
     * ARGUMENTS:
     *   - window width and height:
     *       uint windowWidth_, windowHeight_;
     * RETURNS: None.
     */
    void onCreate(uint windowWidth_ = 400, uint windowHeight_ = 400);

    /* Update window frame function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void onUpdate();

    /* Add scene's instance to the scenes array function.
     * ARGUMENTS:
     *   - scene name:
     *       const ::std::string &sceneName;
     *   - scene instance:
     *       Scene *sceneInstance;
     * RETURNS: None.
     * NOTE:
     *   Returns value may be changed to (scene *) - not-owning pointer to
     * the scene, if we want to be able to copy scenes.
     */
    void addScene(const ::std::string &sceneName, Scene *sceneInstance);

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
