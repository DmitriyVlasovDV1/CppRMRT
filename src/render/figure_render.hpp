#ifndef FIGURE_RENDER_HPP
#define FIGURE_RENDER_HPP
#include "src/buffers/buffer.hpp"
#include "figure_scene.hpp"

namespace hse::drawable_figures {


class FigureRender {
public:
    virtual void init() = 0;
    virtual void render() = 0;
};

class CommonRender : public FigureRender {
public:
    CommonRender(FigureScene &scene) : m_scene(scene) {
    }

    void init() final {
        for (auto &box : m_scene.m_boxes) {
            auto *tmp = m_scene.createCubePrimitive(box.m_size, box.m_size, box.m_size, math::vec3(0));
            tmp->addUniform(render::renderInstance.getTime(), "time");
            math::vec3 color = math::vec3(1, 0, 0);
            tmp->addUniform(color, "vertexColor");
            m_spheres.push_back(tmp);
        }
        for (auto &sphere : m_scene.m_spheres) {
            auto *tmp = m_scene.createSpherePrimitive(sphere.m_radius, math::vec3(0));
            tmp->addUniform(render::renderInstance.getTime(), "time");
            math::vec3 color = math::vec3(1, 0, 0);
            tmp->addUniform(color, "vertexColor");
            m_spheres.push_back(tmp);
        }
    }

    void render() final {
        for (auto &id : m_scene.m_scene) {
            math::matr4 transformation;
            preparePrimitives(id, transformation);
        }

    }

private:
    void preparePrimitives(const FigureId &id, math::matr4 &tranformation) {
        const Figure &figure = m_scene.m_figures[id.m_id];

        for (auto it = figure.m_transforms.rbegin(); it != figure.m_transforms.rend(); it++) {
            tranformation = m_scene.m_matrices[it->m_id] * tranformation;
        }
        if (figure.m_creationType == CreationType::PRIMITIVE) {
            PrimitiveId primId = std::get<PrimitiveId>(figure.m_sources);
            primitive* prim = nullptr;
            if (primId.m_type == PrimitiveType::BOX) {
                prim = m_boxes[primId.m_id];
            }
            else if (primId.m_type == PrimitiveType::SPHERE) {
                prim = m_spheres[primId.m_id];
            }
            if (prim) {
                prim->setVisibility(true);
                prim->transformMatrix = tranformation;
            }
        }
        else {
            for (auto &figId : std::get<std::vector<FigureId>>(figure.m_sources)) {
                preparePrimitives(figId, tranformation);
            }
        }
    }
    std::vector<primitive*> m_boxes;
    std::vector<primitive*> m_spheres;
    FigureScene &m_scene;

};

class RMRender : public FigureRender {
public:
    RMRender(FigureScene &scene) : m_scene(scene) {
    }
    shaderStorageBuffer m_spheresSSBO;
    shaderStorageBuffer m_boxesSSBO;
    shaderStorageBuffer m_matricesSSBO;

    void init() final {

    }

    void render() final {

    }

    primitive *m_canvas;
    FigureScene &m_scene;
};

}


#endif // FIGURE_RENDER_HPP