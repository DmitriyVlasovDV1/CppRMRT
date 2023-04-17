#include "../../render.hpp"
#include "figure_render.hpp"
#include "../resources/shaders/shader.hpp"

namespace hse {
void CommonRender::init() {
    FigureScene &scene = Render::scene;
    for (auto &box : scene.getBoxes()) {
        auto *tmp = scene.createCubePrimitive(box.size, box.size, box.size, math::vec3(0), math::vec3(box.mtl.r, box.mtl.g, box.mtl.b));
        tmp->addUniform(&time, "time");
        math::vec3 color = math::vec3(1, 0, 0);
        tmp->addUniform(&color, "vertexColor");
        m_boxes.push_back(tmp);
    }
    for (auto &sphere : scene.getSpheres()) {
        auto *tmp = scene.createSpherePrimitive(sphere.radius, math::vec3(0), math::vec3(sphere.mtl.r, sphere.mtl.g, sphere.mtl.b), 50, 50);
        tmp->addUniform(&time, "time");
        math::vec3 color = math::vec3(1, 0, 0);
        tmp->addUniform(&color, "vertexColor");
        m_spheres.push_back(tmp);
    }
}

void CommonRender::render() {
    FigureScene &scene = Render::scene;
    for (auto &id : scene.getScene()) {
        math::matr4 transformation;
        preparePrimitives(id, transformation);
    }
}

void CommonRender::hide() {
    for (Primitive* prim : m_boxes) {
        prim->setVisibility(false);
    }
    for (Primitive* prim : m_spheres) {
        prim->setVisibility(false);
    }
}

void CommonRender::preparePrimitives(const FigureId &id, math::matr4 tranformation) {
    FigureScene &scene = Render::scene;
    const Figure &figure = scene.getFigureById(id);

    for (int i = figure.getTransformations().size() - 1; i > -1; i--) {
        auto &trId = figure.getTransformations()[i];
        if (trId.type() == TransformationType::MATRIX) {
            tranformation = scene.getMatrixById(trId) * tranformation;
        }
    }
    if (figure.creationType() == CreationType::PRIMITIVE) {
        PrimitiveId primId = figure.getSourcePrimitive();
        Primitive* prim = nullptr;
        if (primId.type() == PrimitiveType::BOX) {
            prim = m_boxes[primId.id()];
        }
        else if (primId.type() == PrimitiveType::SPHERE) {
            prim = m_spheres[primId.id()];
        }
        if (prim) {
            prim->setVisibility(true);
            prim->setTransformMatrix(tranformation);
        }
    }
    else {
        const std::vector<FigureId> &sources = figure.getSourceFigures();
        for (auto &figId : sources) {
            preparePrimitives(figId, tranformation);
        }
    }
}

void RMRender::init() {
    // TODO: create sbo's, fill with data, create primitive and shader
    FigureScene &scene = Render::scene;
    m_spheresSSBO.setData(scene.getSpheres(), 2);
    m_boxesSSBO.setData(scene.getBoxes(), 3);
    m_matricesSSBO.setData(scene.getMatrices(), 4);
    m_twistsSSBO.setData(scene.getTwistings(), 5);
    m_bendsSSBO.setData(scene.getBendings(), 6);

    std::vector<int> indexBuffer(6);
    std::vector<float> vertexBuffer = {-1, -1, 0,
                                       1, -1, 0,
                                       1, 1, 0,
                                       -1, -1, 0,
                                       1, 1, 0,
                                       -1, 1, 0};

    for (int j = 0; j < 6; j++)
        indexBuffer[j] = j;

    std::string vertexSource = createVertexSource("../data/shaders/rm/vertex.glsl", "../data/shaders/rm_render/vertex.glsl");
    std::string fragmentSource = createFragmentSource("../data/shaders/rm/fragment_src.glsl", "../data/shaders/rm_render/fragment.glsl");
    // TODO uint shdId = scene.createShader(vertexSource, fragmentSource, "rm_render");
    shd = new Shader(vertexSource, fragmentSource);
    m_canvas = scene.createPrimitive(shd->getShaderProgramId(), vertexBuffer, "v3", indexBuffer);
    m_canvas->addConstantUniform((int)windowWidth, "frame_w");
    m_canvas->addConstantUniform((int)windowHeight, "frame_h");
    m_canvas->addUniform(&time, "time");
    m_canvas->addConstantUniform(scene.mainCamera.getPosition(), "cam_pos");
    m_canvas->addConstantUniform(scene.mainCamera.getDirection(), "cam_dir");
    m_canvas->addConstantUniform(scene.mainCamera.getUp(), "cam_up");
    m_canvas->addConstantUniform(scene.mainCamera.getRight(), "cam_right");
    //    unitPrimitive->addUniform(frameH, "frame_h");
    //    unitPrimitive->addUniform(&render::renderInstance.getTime(), "time");
}

void RMRender::render() {
    FigureScene &scene = Render::scene;
    m_spheresSSBO.updateData(scene.getSpheres());
    m_boxesSSBO.updateData(scene.getBoxes());
    m_matricesSSBO.updateData(scene.getMatrices());
    m_twistsSSBO.updateData(scene.getTwistings());
    m_bendsSSBO.updateData(scene.getBendings());
    m_canvas->setVisibility(true);
    m_canvas->addUniform(&time, "time");
    m_canvas->addConstantUniform(scene.mainCamera.getPosition(), "cam_pos");
    m_canvas->addConstantUniform(scene.mainCamera.getDirection(), "cam_dir");
    m_canvas->addConstantUniform(scene.mainCamera.getUp(), "cam_up");
    m_canvas->addConstantUniform(scene.mainCamera.getRight(), "cam_right");
    m_canvas->addConstantUniform((int)windowWidth, "frame_w");
    m_canvas->addConstantUniform((int)windowHeight, "frame_h");
    m_canvas->addConstantUniform((int)scene.isBulb(), "is_bulb");
    m_canvas->addConstantUniform(scene.getBulbPos(), "bulb_pos");
    m_canvas->addConstantUniform(scene.getBulbColor(), "bulb_color");
}

void RMRender::hide() {
    m_canvas->setVisibility(false);
}

std::string RMRender::serializeOperation(const std::string &operationName, const std::vector<FigureId> &sources, const std::string &pos, const std::string &matr) {
    std::string res = serializeFigureId(sources.front(), pos, matr);
    for (auto it = sources.begin() + 1; it != sources.end(); it++) {
        res = operationName + "(" + res + ", " + serializeFigureId(*it, pos, matr) + ")";
    }
    return res;
}

// TODO maybe ref??
std::string RMRender::serializeFigureId(const FigureId &id, std::string pos, std::string matr) {
    FigureScene &scene = Render::scene;
    const Figure &figure = scene.getFigureById(id);

    for (int i = figure.getTransformations().size() - 1; i > -1; i--) {
        auto &trId = figure.getTransformations()[i];
        if (trId.type() == TransformationType::MATRIX) {
            pos = "inverse(matrices_buffer.matrices[" + std::to_string(trId.id()) + "]) * " + pos;
            matr = "inverse(matrices_buffer.matrices[" + std::to_string(trId.id()) + "]) * " + matr;
        } else if (trId.type() == TransformationType::BEND) {
            std::string m = "mat4(1)";
            for (int j = 0; j < i; j++) {
                auto &trId2 = figure.getTransformations()[j];
                if (figure.getTransformations()[i].type() == TransformationType::MATRIX) {
                    m = m + "* matrices_buffer.matrices[" + std::to_string(trId2.id()) + "]";
                }
            }
            pos = "bend(" + pos + ", " + m + ", bend_buffer.bends[" + std::to_string(trId.id()) + "])";
        } else if (trId.type() == TransformationType::TWIST) {
            std::string m = "mat4(1)";
            for (int j = 0; j < i; j++) {
                auto &trId2 = figure.getTransformations()[j];
                if (figure.getTransformations()[i].type() == TransformationType::MATRIX) {
                    m = m + "* matrices_buffer.matrices[" + std::to_string(trId2.id()) + "]";
                }
            }
            pos = "twist(" + pos + ", " + matr + ", twist_buffer.twists[" + std::to_string(trId.id()) + "])";
        }
    }
    if (figure.creationType() == CreationType::PRIMITIVE) {
        PrimitiveId primId = figure.getSourcePrimitive();
        if (primId.type() == PrimitiveType::BOX) {
            return "SDF_box(" + pos + ", box_buffer.boxes[" + std::to_string(primId.id()) + "])";
        } else if (primId.type() == PrimitiveType::SPHERE) {
            return "SDF_sphere(" + pos + ", sphere_buffer.spheres[" + std::to_string(primId.id()) + "])";
        }
    } else if (figure.creationType() == CreationType::INTERSECTION) {
        std::vector<FigureId> sources = figure.getSourceFigures();
        return serializeOperation("inter", sources, pos, matr);
    } else if (figure.creationType() == CreationType::UNION) {
        std::vector<FigureId> sources = figure.getSourceFigures();
        return serializeOperation("unite", sources, pos, matr);
    } else if (figure.creationType() == CreationType::SUBTRACTION) {
        std::vector<FigureId> sources = figure.getSourceFigures();
        return serializeOperation("sub", sources, pos, matr);
    } else if (figure.creationType() == CreationType::SUNION) {
        std::vector<FigureId> sources = figure.getSourceFigures();
        return serializeOperation("sunite", sources, pos, matr);
    }
    return "0";

}

std::string RMRender::getSDFSceneSource() {
    FigureScene &scene = Render::scene;
    std::vector<FigureId> for_draw;
    for (auto figId : scene.getScene()) {
        for_draw.push_back(figId);
    }
    std::string res =\
        "Surface SDF_scene(vec3 p)\n"
        "{\n"
        "\tvec4 pos = vec4(p.xyz, 1);"
        "\tSurface res;\n";
    if (!for_draw.empty()) {
        res += "\tres = ";
        res += serializeOperation("unite", for_draw, "pos", "mat4(1)");
        res += ";\n";
    }
    res += "\treturn res;\n"
           "}\n";
    return res;
}

std::string RMRender::createFragmentSource(const std::string &filePath, const std::string &outPath) {
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

    /*
    std::ofstream output(outPath);
    output << source;
     */

    return source;
}

std::string RMRender::createVertexSource(const std::string &filePath, const std::string &outPath) const {
    std::ifstream file(filePath);
    if (!file) {
        // TODO
    }

    std::string sourceLine;
    std::string source;
    while (std::getline(file, sourceLine)) {
        source += sourceLine + '\n';
    }

    /*
    std::ofstream output(outPath);
    output << source;
     */

    return source;
}

}