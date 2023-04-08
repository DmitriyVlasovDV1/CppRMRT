#ifndef RM_SHD_UNIT_HPP
#define RM_SHD_UNIT_HPP

#include <memory>
#include "../../def.hpp"
#include "../../render/render.hpp"
#include "../../render/src/scenes/scene.hpp"
//#include "rm_figures.hpp"

// Project namespace
namespace hse {
// Test unit class declaration
class rmShdUnit final : public Scene {
public:
    TransformationMatrixId translateId;
    TransformationMatrixId rotationId;

    // Class constructor
    explicit rmShdUnit() = default;

    void onCreate() final;

    void onUpdate() final;

    ~rmShdUnit() final;
};  // End of 'rmShdUnit' class
}  // namespace hse

#endif  // RM_SHD_UNIT_HPP
