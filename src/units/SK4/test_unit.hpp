#ifndef TEST_UNIT_HPP
#define TEST_UNIT_HPP

#include "../../def.hpp"
#include "../../render/render.hpp"

// Project namespace
namespace hse {
// Test unit class declaration
class testUnit final : public unit {
public:
    // Class constructor
    explicit testUnit() = default;

    /* Unit initialization override function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void initUnit() final;

    /* Unit response override function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void responseUnit() final;

    // Class override destructor
    ~testUnit() final;
};  // End of 'testUnit' class
}  // namespace hse

#endif  // TEST_UNIT_HPP
