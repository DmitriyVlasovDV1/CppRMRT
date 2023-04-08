#ifndef FIGURE_SCENE_HPP_
#define FIGURE_SCENE_HPP_

#include <memory>
#include "../../../def.hpp"
#include "../buffers/buffer.hpp"
#include "../units/unit.hpp"
#include "figure_transformation.hpp"
#include "figure_render.hpp"
#include "figure.hpp"
#include "figure_material.hpp"

namespace hse {

struct FigureIdHasher {
    bool operator() (const FigureId &a, const FigureId &b) const;
};

enum class RenderType {
    COMMON,
    RM,
    RT
};

class FigureScene : public unit {
public:
    FigureScene();

    void init() override;

    void response() override;

    void setRenderType(RenderType renderType);

    RenderType getRenderType() const;

    void draw(const FigureId &id);

    void hide(const FigureId &id);

    SpherePrimitive & getSpherePrimitiveById(const PrimitiveId &id);

    BoxPrimitive & getBoxPrimitiveById(const PrimitiveId &id);

    Figure & getFigureById(const FigureId &id);

    math::matr4 & getMatrixById(const TransformationId &id);

    TransformationTwist & getTransformationTwistById(const TransformationId &id);

    TransformationBend & getTransformationBendById(const TransformationId &id);

    const std::vector<SpherePrimitive> & getSpheres() const;

    const std::vector<BoxPrimitive> & getBoxes() const;

    const std::vector<math::matr4> & getMatrices() const;

    std::set<FigureId, FigureIdHasher> & getScene();

    FigureId createBox(float size);

    FigureId createSphere(float radius);

    TransformationMatrixId createTransformation(const math::matr4 &matr);

    TransformationMatrixId createTranslation(const math::vec3 &vec);

    TransformationMatrixId createRotation(const math::vec3 &vec, float deg);

    TransformationMatrixId createScale(const math::vec3 &vec);

    TransformationTwistId createTwist(const math::vec3 &pos, const math::vec3 &dir, float intensity);

    TransformationBendId createBend(const math::vec3 &pos, const math::vec3 &dir, const math::vec3 &rad);

    FigureId createUnion(const FigureId &a, const FigureId &b);

    FigureId createIntersection(const FigureId &a, const FigureId &b);

    FigureId createSubtraction(const FigureId &a, const FigureId &b);

private:
    std::vector<BoxPrimitive> m_boxes;
    std::vector<SpherePrimitive> m_spheres;
    std::vector<Figure> m_figures;

    std::vector<math::matr4> m_matrices;
    std::vector<TransformationBend> m_bendings;
    std::vector<TransformationTwist> m_twistings;

    std::vector<Material> m_materials;

    std::set<FigureId, FigureIdHasher> m_scene;
    RenderType m_curRenderType;
    std::map<RenderType, std::shared_ptr<FigureRender>> m_renders;

}; // FigureRender

}
#endif  // RM_FIGURES_HPP
