#ifndef FIGURE_RENDER_HPP
#define FIGURE_RENDER_HPP
#include "../buffers/buffer.hpp"
#include "figure_scene.hpp"
#include "figure.hpp"

namespace hse {

class FigureRender {
public:
    virtual void init() = 0;
    virtual void render() = 0;
    virtual void hide() = 0;
};

class CommonRender : public FigureRender {
public:
    CommonRender() {
    }

    void init() final;

    void render() final;

    void hide() final;

private:
    void preparePrimitives(const FigureId &id, math::matr4 tranformation);

    std::vector<primitive*> m_boxes;
    std::vector<primitive*> m_spheres;
};

class RMRender : public FigureRender {
public:
    RMRender() {
    }
    shaderStorageBuffer m_spheresSSBO;
    shaderStorageBuffer m_boxesSSBO;
    shaderStorageBuffer m_matricesSSBO;
    shaderStorageBuffer m_twistsSSBO;
    shaderStorageBuffer m_bendsSSBO;

    void init() final;

    void render() final;

    void hide() final;

private:
    std::string serializeOperation(const std::string &operationName, const std::vector<FigureId> &sources, const std::string &pos, const std::string &matr);
    // TODO maybe ref??
    std::string serializeFigureId(const FigureId &id, std::string pos, std::string matr);

    std::string getSDFSceneSource();

    std::string createFragmentSource(const std::string &filePath);

    std::string createVertexSource(const std::string &filePath) const;

    primitive *m_canvas;
};

}


#endif // FIGURE_RENDER_HPP