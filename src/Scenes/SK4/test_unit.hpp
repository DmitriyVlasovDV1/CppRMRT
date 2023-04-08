#ifndef TEST_UNIT_HPP
#define TEST_UNIT_HPP

#include "../../render/src/scenes/scene.hpp"

// Project namespace
namespace hse {
// Test scene class declaration
class TestScene final : public Scene {
    Model *unitModel = nullptr;

    /* Spheres generation function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void spheresGeneration();

    /* Cubes generation function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void cubesGeneration();

    /* Camera update function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void cameraUpdate();

    /* Input (keyboard/mouse) update function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    static void inputUpdate();

public:
    // Class constructor
    explicit TestScene() = default;

    /* Scene creation override function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void onCreate() final;

    /* Scene update override function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void onUpdate() final;

    // Class override destructor
    ~TestScene() final;
};  // End of 'TestScene' class
}  // namespace hse

#endif  // TEST_UNIT_HPP
