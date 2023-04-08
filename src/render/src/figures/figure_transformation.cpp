#include "../../render.hpp"
#include "figure_transformation.hpp"

namespace hse {

int TransformationId::id() const {
    return m_id;
}
TransformationType TransformationId::type() const {
    return m_type;
}

void TransformationMatrixId::set(const math::matr4 &matr) const {
    render::renderInstance.scene.getMatrixById(*this) = matr;
}

void TransformationBendId::set(
    const math::vec3 &pos,
    const math::vec3 &dir,
    const math::vec3 &rad
) const {
    render::renderInstance.scene.getTransformationBendById(*this) = {pos, dir, rad};
}

void TransformationTwistId::set(
    const math::vec3 &pos,
    const math::vec3 &dir,
    float intensity
) const {
    render::renderInstance.scene.getTransformationTwistById(*this) = {pos, dir, intensity};
}

} // namespace "hse"