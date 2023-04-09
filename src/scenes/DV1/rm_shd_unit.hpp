#ifndef RM_SHD_UNIT_HPP
#define RM_SHD_UNIT_HPP

#include "../../def.hpp"
#include "../../render/render.hpp"
#include "../../render/src/resources/scenes/scene.hpp"

// #include "rm_figures.hpp"

// Project namespace
namespace hse {
class rmShdScene final : public Scene {
public:
    TransformationMatrixId translateId;
    TransformationMatrixId rotationId;

    // Class constructor
    explicit rmShdScene() = default;

    void onCreate() final;

    void onUpdate() final;

    ~rmShdScene() final;
};  // End of 'rmShdUnit' class
}  // namespace hse

#endif  // RM_SHD_UNIT_HPP
