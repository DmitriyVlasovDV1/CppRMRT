#include <iostream>
#include "def.hpp"
#include "render/render.hpp"
#include "scenes/DV1/rm_shd_unit.hpp"
#include "scenes/SK4/test_unit.hpp"

float hse::time;                    // Global time variable
float hse::deltaTime;               // Global delta time variable
bool hse::isPause;                  // Pause flag
unsigned int hse::windowWidth,      // Window width
    hse::windowHeight;              // Window height
std::map<int, hse::Key> hse::keys;  // Keys map for input response

// Function for generate render instance variable
void hse::factory() {
    hse::Render renderInstance;

    renderInstance.onCreate(500, 500);
    renderInstance.addScene(new hse::rmShdScene());
    //renderInstance.addScene(new hse::TestScene());
    renderInstance.startRenderLoop();
}  // End of 'hse::factory' function

// Main program function
int main() {
    hse::factory();
    return 0;
}  // End of 'main' function
