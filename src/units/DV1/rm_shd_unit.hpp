#ifndef RM_SHD_UNIT_HPP
#define RM_SHD_UNIT_HPP

#include <memory>
#include "../../def.hpp"
#include "../../render/render.hpp"


// Project namespace
namespace hse {
// Test unit class declaration
class rmShdUnit final : public unit {
private:
    int *frameW;
    int *frameH;
public:
    // Class constructor
    explicit rmShdUnit() = default;

    void initUnit() final;

    void responseUnit() final;

    ~rmShdUnit() final;
};  // End of 'rmShdUnit' class
}  // namespace hse

#endif  // RM_SHD_UNIT_HPP
