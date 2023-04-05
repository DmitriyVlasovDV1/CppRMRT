#include "figure_scene.hpp"
#include "render.hpp"
#include "figure_render.hpp"

namespace hse::drawable_figures {

bool FigureIdHasher::operator() (const FigureId &a, const FigureId &b) const {
    return a.m_id < b.m_id;
}

// intersection
FigureId FigureId::operator&(const FigureId &other) {
    return render::renderInstance.scene.createIntersection(*this, other);
}

FigureId FigureId::operator|(const FigureId &other) {
    return render::renderInstance.scene.createUnion(*this, other);
}

FigureId FigureId::operator/(const FigureId &other) {
    return render::renderInstance.scene.createSubtraction(*this, other);
}

FigureId FigureId::operator&=(const FigureId &other) {
    *this = render::renderInstance.scene.createIntersection(*this, other);
    return *this;
}

FigureId FigureId::operator|=(const FigureId &other) {
    *this = render::renderInstance.scene.createUnion(*this, other);
    return *this;
}

FigureId FigureId::operator/=(const FigureId &other) {
    *this = render::renderInstance.scene.createSubtraction(*this, other);
    return *this;
}

FigureId & FigureId::operator<<(const TransformationId &trId) {
    render::renderInstance.scene.m_figures[m_id].m_transforms.push_back(trId);
    return *this;
}

FigureId & FigureId::operator<<(const math::matr4 &matr) {
    TransformationId trId = render::renderInstance.scene.createTransformation(matr);
    render::renderInstance.scene.m_figures[m_id].m_transforms.push_back(trId);
    return *this;
}


// add instance for main scene
void FigureId::draw() const {
    render::renderInstance.scene.draw(*this);
}

void FigureScene::setRenderType(RenderType renderType) {
    m_curRenderType = renderType;
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

TransformationId FigureScene::createTransformation(const math::matr4 &matr) {
    TransformationId res(static_cast<int>(m_matrices.size()));
    m_matrices.push_back(matr);
    return res;
}

TransformationId FigureScene::createTranslation(const math::vec3 &vec) {
    TransformationId res(static_cast<int>(m_matrices.size()));
    m_matrices.push_back(math::matr4::translate(vec));
    return res;
}

TransformationId FigureScene::createRotation(
    const math::vec3 &vec,
    const float deg
) {
    TransformationId res(static_cast<int>(m_matrices.size()));
    m_matrices.push_back(math::matr4::rotate(deg, vec));
    return res;
}

TransformationId FigureScene::createScale(const math::vec3 &vec) {
    TransformationId res(static_cast<int>(m_matrices.size()));
    m_matrices.push_back(math::matr4::scale(vec));
    return res;
}

TransformationId & TransformationId::operator=(const math::matr4 &matr) {
    render::renderInstance.scene.m_matrices[m_id] = matr;
    return *this;
}

TransformationId & TransformationId::operator*=(const math::matr4 &matr) {
    render::renderInstance.scene.m_matrices[m_id] *= matr;
    return *this;
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

void FigureScene::draw(const FigureId &id) {
    m_scene.insert(id);
}

void FigureScene::hide(const FigureId &id) {
    m_scene.erase(id);
}

/*
void FigureScene::initRM() {
    GLuint ssbo, ssbo2, ssbo3, ssbo4, ssbo5, ssbo6;
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(SphereBrick) * factory->spheres.size(), factory->spheres.data(), GL_STATIC_DRAW); // ??? GL_DYNAMIC_COPY mb GL_STATIC_DRAW
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    glGenBuffers(1, &ssbo2);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo2);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(BoxBrick) * factory->boxes.size(), factory->boxes.data(), GL_STATIC_DRAW); // ??? GL_DYNAMIC_COPY mb GL_STATIC_DRAW
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo2);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    glGenBuffers(1, &ssbo3);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo3);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(::math::matr4) * factory->matricies.size(), factory->matricies.data(), GL_DYNAMIC_COPY); // ??? GL_DYNAMIC_COPY mb GL_STATIC_DRAW
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, ssbo3);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    glGenBuffers(1, &ssbo4);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo4);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * serialized.size(), serialized.data(), GL_STATIC_DRAW); // ??? GL_DYNAMIC_COPY mb GL_STATIC_DRAW
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, ssbo4);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    // tmp
    BoxBrick data[1024];
    glGenBuffers(1, &ssbo5);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo5);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(BoxBrick) * 1024, data, GL_STATIC_DRAW); // ??? GL_DYNAMIC_COPY mb GL_STATIC_DRAW
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, ssbo5);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}
*/

FigureScene::FigureScene() : m_curRenderType(RenderType::RM) {
    m_renders[RenderType::COMMON] = std::make_shared<CommonRender>(*this);
    m_renders[RenderType::RM] = std::make_shared<RMRender>(*this);
}

// prepare renders
void FigureScene::init() {
    // initialization of renders
    for (auto &render : m_renders) {
        render.second->init();
    }
}

// draw scene
void FigureScene::response() {
    for (auto &render : m_renders) {
        render.second->clear();
    }
    m_renders[m_curRenderType]->render();
}

}
