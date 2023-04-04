#include "figure_scene.hpp"
#include "render.hpp"
#include "figure_render.hpp"

namespace hse::drawable_figures {

bool FigureIdHasher::operator() (const FigureId &a, const FigureId &b) const {
    return a.m_id < b.m_id;
}

// intersection
FigureId FigureId::operator&(const FigureId &other) {
    return other;
}

// add instance for main scene
void FigureId::draw() const {
    render::renderInstance.scene.draw(*this);
}

void FigureScene::setRenderType(RenderType renderType) {
    m_curRenderType = renderType;
}

FigureId FigureScene::createBox(float size, MaterialId mtl) {
    int ind(static_cast<int>(m_boxes.size()));
    m_boxes.emplace_back(size);
    FigureId res(static_cast<int>(m_figures.size()));
    m_figures.push_back(Figure(ind));
    return res;
}

FigureId FigureScene::createSphere(float radius) {
    int ind(static_cast<int>(m_spheres.size()));
    m_spheres.emplace_back(radius);
    FigureId res(static_cast<int>(m_figures.size()));
    m_figures.push_back(Figure(ind));
    return res;
}

TransformationId FigureScene::createTranslation(const math::vec3 &vec) {
    math::matr4 matr;//TransformationId res(static_cast<int>(m_matrices.size()));
    // TODO fill matr
    TransformationId res(static_cast<int>(m_matrices.size()));
    m_matrices.push_back(matr);
    return res;
}

TransformationId FigureScene::createRotation(
    const math::vec3 &vec,
    const float deg
) {
    math::matr4 matr;//TransformationId res(static_cast<int>(m_matrices.size()));
    // TODO fill matr
    TransformationId res(static_cast<int>(m_matrices.size()));
    m_matrices.push_back(matr);
    return res;
}

TransformationId FigureScene::createScale(const math::vec3 &vec) {
    math::matr4 matr;//TransformationId res(static_cast<int>(m_matrices.size()));
    // TODO fill matr
    TransformationId res(static_cast<int>(m_matrices.size()));
    m_matrices.push_back(matr);
    return res;
}

FigureId FigureScene::createIntersection(const FigureId &a, const FigureId &b) {
    FigureId res(static_cast<int>(m_figures.size()));
    m_figures.push_back(Figure(CreationType::INTERSECTION, {a, b}));
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

// prepare renders
void FigureScene::init() {
    // initialization of renders
    /*
    for (auto &render : m_renders) {
        render.second->init(*this);
    }
     */
}

// draw scene
void FigureScene::response() {
    /*
    m_renders[m_curRenderType]->render(*this);
     */
}

}
