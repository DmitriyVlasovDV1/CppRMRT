#include "../../render.hpp"
#include "figure_render.hpp"

namespace hse {
void CommonRender::init() {
    FigureScene &scene = render::renderInstance.scene;
    for (auto &box : scene.getBoxes()) {
        auto *tmp = scene.createCubePrimitive(box.size, box.size, box.size, math::vec3(0));
        tmp->addUniform(render::renderInstance.getTime(), "time");
        math::vec3 color = math::vec3(1, 0, 0);
        tmp->addUniform(color, "vertexColor");
        m_boxes.push_back(tmp);
    }
    for (auto &sphere : scene.getSpheres()) {
        auto *tmp = scene.createSpherePrimitive(sphere.radius, math::vec3(0));
        tmp->addUniform(render::renderInstance.getTime(), "time");
        math::vec3 color = math::vec3(1, 0, 0);
        tmp->addUniform(color, "vertexColor");
        m_spheres.push_back(tmp);
    }
}

void CommonRender::render() {
    FigureScene &scene = render::renderInstance.scene;
    for (auto &id : scene.getScene()) {
        math::matr4 transformation;
        preparePrimitives(id, transformation);
    }
}

void CommonRender::hide() {
    for (primitive* prim : m_boxes) {
        prim->setVisibility(false);
    }
    for (primitive* prim : m_spheres) {
        prim->setVisibility(false);
    }
}

void CommonRender::preparePrimitives(const FigureId &id, math::matr4 tranformation) {
    FigureScene &scene = render::renderInstance.scene;
    const Figure &figure = scene.getFigureById(id);

    for (auto it = figure.getTransformations().rbegin(); it != figure.getTransformations().rend(); it++) {
        tranformation = scene.getMatrixById(*it) * tranformation;
    }
    if (figure.creationType() == CreationType::PRIMITIVE) {
        PrimitiveId primId = figure.getSourcePrimitive();
        primitive* prim = nullptr;
        if (primId.type() == PrimitiveType::BOX) {
            prim = m_boxes[primId.id()];
        }
        else if (primId.type() == PrimitiveType::SPHERE) {
            prim = m_spheres[primId.id()];
        }
        if (prim) {
            prim->setVisibility(true);
            prim->transformMatrix = tranformation;
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
    FigureScene &scene = render::renderInstance.scene;
    m_spheresSSBO.setData(scene.getSpheres(), 2);
    m_boxesSSBO.setData(scene.getBoxes(), 3);
    m_matricesSSBO.setData(scene.getMatrices(), 4);

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
    uint shdId = scene.createShader(vertexSource, fragmentSource, "rm_render");
    m_canvas = scene.createPrimitive(shdId, vertexBuffer, "v3", indexBuffer);
    m_canvas->addUniform(render::renderInstance.getWindowWidth(), "frame_w");
    m_canvas->addUniform(render::renderInstance.getWindowHeight(), "frame_h");
    m_canvas->addUniform(render::renderInstance.getTime(), "time");
    m_canvas->addUniform(scene.mainCamera.getPosition(), "cam_pos");
    m_canvas->addUniform(scene.mainCamera.getDirection(), "cam_dir");
    m_canvas->addUniform(scene.mainCamera.getUp(), "cam_up");
    m_canvas->addUniform(scene.mainCamera.getRight(), "cam_right");
    //    unitPrimitive->addUniform(frameH, "frame_h");
    //    unitPrimitive->addUniform(&render::renderInstance.getTime(), "time");
}

void RMRender::render() {
    FigureScene &scene = render::renderInstance.scene;
    m_spheresSSBO.updateData(scene.getSpheres());
    m_boxesSSBO.updateData(scene.getBoxes());
    m_matricesSSBO.updateData(scene.getMatrices());
    m_canvas->setVisibility(true);
    m_canvas->addUniform(render::renderInstance.getTime(), "time");
}

void RMRender::hide() {
    m_canvas->setVisibility(false);
}

std::string RMRender::serializeOperation(const std::string &operationName, const std::vector<FigureId> &sources, const std::string &transformation) {
    std::string res = serializeFigureId(sources.front(), transformation);
    for (auto it = sources.begin() + 1; it != sources.end(); it++) {
        res = operationName + "(" + res + ", " + serializeFigureId(*it, transformation) + ")";
    }
    return res;
}

// TODO maybe ref??
std::string RMRender::serializeFigureId(const FigureId &id, std::string transformation) {
    FigureScene &scene = render::renderInstance.scene;
    const Figure &figure = scene.getFigureById(id);

    for (auto trId : figure.getTransformations()) {
        transformation += " * inverse(matrices_buffer.matrices[" + std::to_string(trId.id()) + "])";
    }
    if (figure.creationType() == CreationType::PRIMITIVE) {
        PrimitiveId primId = figure.getSourcePrimitive();
        if (primId.type() == PrimitiveType::BOX) {
            return "SDF_box(pos, " + transformation + ", box_buffer.boxes[" + std::to_string(primId.id()) + "])";
        } else if (primId.type() == PrimitiveType::SPHERE) {
            return "SDF_sphere(pos, " + transformation + ", sphere_buffer.spheres[" + std::to_string(primId.id()) + "])";
        }
    } else if (figure.creationType() == CreationType::INTERSECTION) {
        std::vector<FigureId> sources = figure.getSourceFigures();
        return serializeOperation("inter", sources, transformation);
    } else if (figure.creationType() == CreationType::UNION) {
        std::vector<FigureId> sources = figure.getSourceFigures();
        return serializeOperation("unite", sources, transformation);
    } else if (figure.creationType() == CreationType::SUBTRACTION) {
        std::vector<FigureId> sources = figure.getSourceFigures();
        return serializeOperation("sub", sources, transformation);
    }
    return "0";

}

std::string RMRender::getSDFSceneSource() {
    FigureScene &scene = render::renderInstance.scene;
    std::vector<FigureId> for_draw;
    for (auto figId : scene.getScene()) {
        for_draw.push_back(figId);
    }
    std::string res =\
        "Surface SDF_scene(vec3 pos)\n"
        "{\n"
        "\tSurface res;\n"
        "\tres = ";
    res += serializeOperation("unite", for_draw, "mat4(1)");
    res += ";\n"
           "\treturn res;\n"
           "}\n";
    return res;
}

std::string RMRender::createFragmentSource(const std::string &filePath) {
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

std::string RMRender::createVertexSource(const std::string &filePath) const {
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

}