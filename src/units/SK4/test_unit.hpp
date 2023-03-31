#ifndef TEST_UNIT_HPP
#define TEST_UNIT_HPP

#include "../../def.hpp"
#include "../../render/render.hpp"

// Project namespace
namespace hse {
// Test unit class declaration
class testUnit final : public unit {
    primitive *unitPrimitive;

    /* Sphere generation function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void spheresGeneration();

    /* Camera response function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void cameraResponse();

    /* Input (keyboard/mouse) response function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void inputResponse();

public:
    // Class constructor
    explicit testUnit() = default;

    /* Unit initialization override function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void init() final;

    /* Unit response override function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void response() final;

    // Class override destructor
    ~testUnit() final;
};  // End of 'testUnit' class
}  // namespace hse

#endif  // TEST_UNIT_HPP
