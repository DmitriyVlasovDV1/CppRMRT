#ifndef RENDER_HPP
#define RENDER_HPP

#include "../def.hpp"
#include "src/units/unit.hpp"

// Project namespace
namespace hse {
// Render class declaration
class Render {
    // Friend render instance factory
    friend void factory();

    ::std::map<::std::string, Unit *> unitsArray;  // Units map
    GLFWwindow *windowInstance;                    // Window instance

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
    void addUnit(const ::std::string &unitName, Unit *unitInstance);

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
