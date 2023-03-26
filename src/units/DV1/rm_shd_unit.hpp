#ifndef RM_SHD_UNIT_HPP
#define RM_SHD_UNIT_HPP

#include <memory>
#include "../../def.hpp"
#include "../../render/render.hpp"
//#include "rm_figures.hpp"

// Project namespace
namespace hse {
class Builder;
// Test unit class declaration
class rmShdUnit final : public unit {
private:
    int *frameW;
    int *frameH;
    uint shdId;
    primitive *unitPrimitive;
    //std::unique_ptr<FigureFactory> factory;
public:
    // Class constructor
    explicit rmShdUnit() = default;

    void init() final;

    void response() final;

    ~rmShdUnit() final;
};  // End of 'rmShdUnit' class
}  // namespace hse

#endif  // RM_SHD_UNIT_HPP
