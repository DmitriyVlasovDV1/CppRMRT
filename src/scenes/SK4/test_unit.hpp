#ifndef TEST_SCENE_HPP
#define TEST_SCENE_HPP

#include "../../render/src/resources/scenes/scene.hpp"

// Project namespace
namespace hse {
// Test scene class declaration
class TestScene final : public Scene {
    int numberOfDemo;
    Model *cup = nullptr;
    Model *tree = nullptr;
    Primitive *cube = nullptr;
    Primitive *sphere1 = nullptr;
    Primitive *sphere2 = nullptr;
    Primitive *plane = nullptr;

    /* Spheres generation function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void spheresGeneration();

    /* Spheres update function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void spheresUpdate();

    /* Cubes generation function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void cubesGeneration();

    /* Cubes update function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void cubesUpdate();

    /* Tree generation function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void treeGeneration();

    /* Tree update function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void treeUpdate();

    /* Cup generation function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void cupGeneration();

    /* Cup update function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void cupUpdate();

    /* Objects (primitives/models) generation function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void objectsGeneration();

    /* Objects (primitives/models) update function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void objectsUpdate();

    /* Camera update function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void cameraUpdate();

    /* Input (keyboard/mouse) update function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void inputUpdate();

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

#endif  // TEST_SCENE_HPP
