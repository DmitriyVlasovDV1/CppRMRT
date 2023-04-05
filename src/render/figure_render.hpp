#ifndef FIGURE_RENDER_HPP
#define FIGURE_RENDER_HPP
#include "src/buffers/buffer.hpp"
#include "figure_scene.hpp"

namespace hse::drawable_figures {


class FigureRender {
public:
    virtual void init() = 0;
    virtual void render() = 0;
    virtual void clear() = 0;
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
            m_boxes.push_back(tmp);
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
    void clear() final {
        for (primitive* prim : m_boxes) {
            prim->setVisibility(false);
        }
        for (primitive* prim : m_spheres) {
            prim->setVisibility(false);
        }
    }

private:
    void preparePrimitives(const FigureId &id, math::matr4 tranformation) {
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
        // TODO: create sbo's, fill with data, create primitive and shader
        m_spheresSSBO.setData(m_scene.m_spheres, 2);
        m_boxesSSBO.setData(m_scene.m_boxes, 3);
        m_matricesSSBO.setData(m_scene.m_matrices, 4);

        std::vector<int> indexBuffer(6);
        std::vector<float> vertexBuffer = {-1, -1, 0,
                                             1, -1, 0,
                                             1, 1, 0,
                                             -1, -1, 0,
                                             1, 1, 0,
                                             -1, 1, 0};

        for (int j = 0; j < 6; j++)
            indexBuffer[j] = j;
        std::string vertexSource = createVertexSource("../data/shaders/rm/vertex.glsl");
        std::string fragmentSource = createFragmentSource("../data/shaders/rm/fragment_src.glsl");
        std::cout << fragmentSource << std::endl;
        uint shdId = m_scene.createShader(vertexSource, fragmentSource, "rm_render");
        m_canvas = m_scene.createPrimitive(shdId, vertexBuffer, "v3", indexBuffer);
        m_canvas->addUniform(render::renderInstance.getWindowWidth(), "frame_w");
        m_canvas->addUniform(render::renderInstance.getWindowHeight(), "frame_h");
        m_canvas->addUniform(render::renderInstance.getTime(), "time");
        m_canvas->addUniform(m_scene.mainCamera.getPosition(), "cam_pos");
        m_canvas->addUniform(m_scene.mainCamera.getDirection(), "cam_dir");
        m_canvas->addUniform(m_scene.mainCamera.getUp(), "cam_up");
        m_canvas->addUniform(m_scene.mainCamera.getRight(), "cam_right");
        //    unitPrimitive->addUniform(frameH, "frame_h");
        //    unitPrimitive->addUniform(&render::renderInstance.getTime(), "time");
    }

    void render() final {
        m_spheresSSBO.updateData(m_scene.m_spheres);
        m_boxesSSBO.updateData(m_scene.m_boxes);
        m_matricesSSBO.updateData(m_scene.m_matrices);
        m_canvas->setVisibility(true);
    }

    void clear() final {
        m_canvas->setVisibility(false);
    }

private:
    std::string serializeOperation(const std::string &operationName, const std::vector<FigureId> &sources, const std::string &transformation) {
        std::string res = serializeFigureId(sources.front(), transformation);
        for (auto it = sources.begin() + 1; it != sources.end(); it++) {
            res = operationName + "(" + res + ", " + serializeFigureId(*it, transformation) + ")";
        }
        return res;
    }

    // TODO maybe ref??
    std::string serializeFigureId(const FigureId &id, std::string transformation) {
        const Figure &figure = m_scene.m_figures[id.m_id];

        for (auto trId : figure.m_transforms) {
            transformation += " * inverse(matrices_buffer.matrices[" + std::to_string(trId.m_id) + "])";
        }
        if (figure.m_creationType == CreationType::PRIMITIVE) {
            PrimitiveId primId = std::get<PrimitiveId>(figure.m_sources);
            if (primId.m_type == PrimitiveType::BOX) {
                return "SDF_box(pos, " + transformation + ", box_buffer.boxes[" + std::to_string(primId.m_id) + "])";
            } else if (primId.m_type == PrimitiveType::SPHERE) {
                return "SDF_sphere(pos, " + transformation + ", sphere_buffer.spheres[" + std::to_string(primId.m_id) + "])";
            }
        } else if (figure.m_creationType == CreationType::INTERSECTION) {
            std::vector<FigureId> sources = std::get<std::vector<FigureId>>(figure.m_sources);
            return serializeOperation("inter", sources, transformation);
        } else if (figure.m_creationType == CreationType::UNION) {
            std::vector<FigureId> sources = std::get<std::vector<FigureId>>(figure.m_sources);
            return serializeOperation("unite", sources, transformation);
        } else if (figure.m_creationType == CreationType::SUBTRACTION) {
            std::vector<FigureId> sources = std::get<std::vector<FigureId>>(figure.m_sources);
            return serializeOperation("sub", sources, transformation);
        }
        return "0";

    }

    std::string getSDFSceneSource() {
        std::vector<FigureId> scene;
        for (auto figId : m_scene.m_scene) {
            scene.push_back(figId);
        }
        std::string res =\
            "Surface SDF_scene(vec3 pos)\n"
            "{\n"
            "\tSurface res;\n"
            "\tres = ";
        res += serializeOperation("unite", scene, "mat4(1)");
        res += ";\n"
               "\treturn res;\n"
               "}\n";
        return res;
    }
    std::string createFragmentSource(const std::string &filePath) {
        std::ifstream file(filePath);
        if (!file) {
            // TODO
        }
        std::string sourceLine;
        std::string source;
        while (std::getline(file, sourceLine)) {
            if (sourceLine == "#include SDF_scene") {
                source += getSDFSceneSource();
            } else {
                source += sourceLine + '\n';
            }
        }

        return source;
    }
    std::string createVertexSource(const std::string &filePath) const {
        std::ifstream file(filePath);
        if (!file) {
            // TODO
        }

        std::string sourceLine;
        std::string source;
        while (std::getline(file, sourceLine)) {
            source += sourceLine + '\n';
        }

        return source;
    }

    primitive *m_canvas;
    FigureScene &m_scene;
};

}


#endif // FIGURE_RENDER_HPP