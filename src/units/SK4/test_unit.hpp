#ifndef TEST_UNIT_HPP
#define TEST_UNIT_HPP

#include "../../render/src/units/unit.hpp"

// Project namespace
namespace hse {
// Test unit class declaration
class TestUnit final : public Unit {
    Model *unitModel;

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
    explicit TestUnit() = default;

    /* Unit initialization override function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void onCreate() final;

    /* Unit response override function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void onUpdate() final;

    // Class override destructor
    ~TestUnit() final;
};  // End of 'TestUnit' class
}  // namespace hse

#endif  // TEST_UNIT_HPP
