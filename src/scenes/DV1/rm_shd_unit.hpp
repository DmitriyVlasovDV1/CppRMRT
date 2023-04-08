#ifndef RM_SHD_UNIT_HPP
#define RM_SHD_UNIT_HPP

#include "rm_figures.hpp"

// Project namespace
namespace hse {
class Builder;
// Test unit class declaration
class rmShdUnit final : public Scene {
private:
    int *frameW;
    int *frameH;
    uint shdId;
    Primitive *unitPrimitive;
    std::unique_ptr<FigureFactory> factory;
public:
    // Class constructor
    explicit rmShdUnit() = default;

    void onCreate() final;

    void onUpdate() final;

    ~rmShdUnit() final;
};  // End of 'rmShdUnit' class
}  // namespace hse

#endif  // RM_SHD_UNIT_HPP
