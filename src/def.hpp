#ifndef DEF_HPP
#define DEF_HPP

// Include header files of using libraries
#include <glew.h>
#include <glfw3.h>

// All useful includes
#include <algorithm>
#include <cassert>
#include <cmath>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "utilities/math/math.hpp"

#define EXCEPTION(message) throw ::std::runtime_error(message)

typedef unsigned int uint;

// Project namespace
namespace hse {
// Key struct for callback
struct Key {
    int action;
    int mode;
};  // End of 'Key' struct

extern float time;                 // Global time variable
extern float deltaTime;            // Global delta time variable
extern bool isPause;               // Pause flag
extern unsigned int windowWidth,   // Window width
    windowHeight;                  // Window height
extern std::map<int, Key> keys;  // Keys map for input response

/* Resize window callback function.
 * ARGUMENTS:
 *   - window instance:
 *       GLFWwindow *window;
 *   - window width and height:
 *       int width, height;
 * RETURNS: None.
 */
void frameBufferSizeCallback(GLFWwindow *window, int width, int height);

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
void keyboardCallback(
    GLFWwindow *window,
    int key,
    int scancode,
    int action,
    int mods
);

// Function for generate render instance variable
void factory();
}  // namespace hse

#endif  // DEF_HPP
