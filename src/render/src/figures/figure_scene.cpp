#include "figure_scene.hpp"
#include "../../render.hpp"
#include "figure_render.hpp"

namespace hse {

bool FigureIdHasher::operator() (const FigureId &a, const FigureId &b) const {
    return a.id() < b.id();
}

FigureScene::FigureScene() : m_curRenderType(RenderType::RM) {
    m_renders[RenderType::COMMON] = std::make_shared<CommonRender>();
    m_renders[RenderType::RM] = std::make_shared<RMRender>();
}

void FigureScene::init() {
    for (auto &render : m_renders) {
        render.second->init();
    }
}

void FigureScene::response() {
    for (auto &render : m_renders) {
        render.second->hide();
    }
    m_renders[m_curRenderType]->render();
}

void FigureScene::setRenderType(RenderType renderType) {
    m_curRenderType = renderType;
}

RenderType FigureScene::getRenderType() const {
    return m_curRenderType;
}

void FigureScene::draw(const FigureId &id) {
    m_scene.insert(id);
}

void FigureScene::hide(const FigureId &id) {
    m_scene.erase(id);
}


SpherePrimitive & FigureScene::getSpherePrimitiveById(const PrimitiveId &id) {
    assert(id.type() == PrimitiveType::SPHERE);
    return m_spheres[id.id()];
}

BoxPrimitive & FigureScene::getBoxPrimitiveById(const PrimitiveId &id) {
    assert(id.type() == PrimitiveType::BOX);
    return m_boxes[id.id()];
}

Figure & FigureScene::getFigureById(const FigureId &id) {
    return m_figures[id.id()];
}

math::matr4 & FigureScene::getMatrixById(const TransformationId &id) {
   return m_matrices[id.id()];
}

TransformationTwist & FigureScene::getTransformationTwistById(const TransformationId &id) {
    return m_twistings[id.id()];
}

TransformationBend & FigureScene::getTransformationBendById(const TransformationId &id) {
    return m_bendings[id.id()];
}

const std::vector<SpherePrimitive> & FigureScene::getSpheres() const {
    return m_spheres;
}

const std::vector<BoxPrimitive> & FigureScene::getBoxes() const {
    return m_boxes;
}

const std::vector<math::matr4> & FigureScene::getMatrices() const {
    return m_matrices;
}

const std::vector<TransformationTwist> & FigureScene::getTwistings() const {
    return m_twistings;
}

const std::vector<TransformationBend> & FigureScene::getBendings() const {
    return m_bendings;
}

std::set<FigureId, FigureIdHasher> & FigureScene::getScene() {
    return m_scene;
}

FigureId FigureScene::createCopy(const FigureId &id) {
    FigureId res(static_cast<int>(m_figures.size()));
    m_figures.push_back(m_figures[id.id()]);
    return res;
}

FigureId FigureScene::createBox(float size) {
    int ind(static_cast<int>(m_boxes.size()));
    m_boxes.emplace_back(size);
    FigureId res(static_cast<int>(m_figures.size()));
    m_figures.push_back(Figure(PrimitiveType::BOX, ind));
    return res;
}

FigureId FigureScene::createSphere(float radius) {
    int ind(static_cast<int>(m_spheres.size()));
    m_spheres.emplace_back(radius);
    FigureId res(static_cast<int>(m_figures.size()));
    m_figures.push_back(Figure(PrimitiveType::SPHERE, ind));
    return res;
}

TransformationMatrixId FigureScene::createTransformation(const math::matr4 &matr) {
    TransformationMatrixId res(static_cast<int>(m_matrices.size()));
    m_matrices.push_back(matr);
    return res;
}

TransformationMatrixId FigureScene::createTranslation(const math::vec3 &vec) {
    TransformationMatrixId res(static_cast<int>(m_matrices.size()));
    m_matrices.push_back(math::matr4::translate(vec));
    return res;
}

TransformationMatrixId FigureScene::createRotation(
    const math::vec3 &vec,
    const float deg
) {
    TransformationMatrixId res(static_cast<int>(m_matrices.size()));
    m_matrices.push_back(math::matr4::rotate(deg, vec));
    return res;
}

TransformationMatrixId FigureScene::createScale(const math::vec3 &vec) {
    TransformationMatrixId res(static_cast<int>(m_matrices.size()));
    m_matrices.push_back(math::matr4::scale(vec));
    return res;
}

TransformationTwistId FigureScene::createTwist(const math::vec3 &pos, const math::vec3 &dir, float intensity) {
    TransformationTwistId res(static_cast<int>(m_twistings.size()));
    m_twistings.push_back({pos, dir, intensity});
    return res;
}

TransformationBendId FigureScene::createBend(const math::vec3 &pos, const math::vec3 &dir, const math::vec3 &rad) {
    TransformationBendId res(static_cast<int>(m_bendings.size()));
    m_bendings.push_back({pos, dir, rad});
    return res;
}

FigureId FigureScene::createUnion(const FigureId &a, const FigureId &b) {
    FigureId res(static_cast<int>(m_figures.size()));
    m_figures.push_back(Figure(CreationType::UNION, {a, b}));
    return res;
}

FigureId FigureScene::createIntersection(const FigureId &a, const FigureId &b) {
    FigureId res(static_cast<int>(m_figures.size()));
    m_figures.push_back(Figure(CreationType::INTERSECTION, {a, b}));
    return res;
}
FigureId FigureScene::createSubtraction(const FigureId &a, const FigureId &b) {
    FigureId res(static_cast<int>(m_figures.size()));
    m_figures.push_back(Figure(CreationType::SUBTRACTION, {a, b}));
    return res;
}

}
