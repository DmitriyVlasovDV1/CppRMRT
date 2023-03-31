#include "figure_render.hpp"

namespace hse::drawable_figures {
FigureRender FigureRender::figureRender;

// intersection
FigureId FigureId::operator&(const FigureId &other) {
    return other;
}

// add instance for main scene
void FigureId::draw(RenderType renderType) {
}

FigureId FigureRender::createBox(float size, MaterialId mtl) {
    int ind(static_cast<int>(m_primitives.size()));
    m_primitives.push_back(std::make_unique<BoxPrimitive>(size));
    FigureId res(static_cast<int>(m_figures.size()));
    m_figures.push_back(Figure(ind));
    return res;
}

FigureId FigureRender::createSphere(float radius) {
    int ind(static_cast<int>(m_primitives.size()));
    m_primitives.push_back(std::make_unique<SpherePrimitive>(radius));
    FigureId res(static_cast<int>(m_figures.size()));
    m_figures.push_back(Figure(ind));
    return res;
}

TransformationId FigureRender::createTranslation(const math::vec3 &vec) {
    TransformationId res(static_cast<int>(m_transformations.size()));
    m_transformations.push_back(std::make_unique<Translation>(vec));
    return res;
}

TransformationId FigureRender::createRotation(
    const math::vec3 &vec,
    const float deg
) {
    TransformationId res(static_cast<int>(m_transformations.size()));
    m_transformations.push_back(std::make_unique<Rotation>(vec, deg));
    return res;
}

TransformationId FigureRender::createScale(const math::vec3 &vec) {
    TransformationId res(static_cast<int>(m_transformations.size()));
    m_transformations.push_back(std::make_unique<Scale>(vec));
    return res;
}

FigureId FigureRender::createIntersection(const FigureId &a, const FigureId &b) {
    FigureId res(static_cast<int>(m_figures.size()));
    m_figures.push_back(Figure(CreationType::INTERSECTION, {a, b}));
    return res;
}

}
