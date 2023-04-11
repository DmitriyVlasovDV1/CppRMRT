#ifndef FIGURE_RENDER_HPP
#define FIGURE_RENDER_HPP
#include "../resources/buffers/buffer.hpp"
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

    std::vector<Primitive*> m_boxes;
    std::vector<Primitive*> m_spheres;
};

class RMRender : public FigureRender {
public:
    RMRender() {
    }
    ShaderStorageBuffer m_spheresSSBO;
    ShaderStorageBuffer m_boxesSSBO;
    ShaderStorageBuffer m_matricesSSBO;
    ShaderStorageBuffer m_twistsSSBO;
    ShaderStorageBuffer m_bendsSSBO;
    Shader *shd;

    void init() final;

    void render() final;

    void hide() final;

    ~RMRender() {
        delete shd;
    }

private:
    std::string serializeOperation(const std::string &operationName, const std::vector<FigureId> &sources, const std::string &pos, const std::string &matr);
    // TODO maybe ref??
    std::string serializeFigureId(const FigureId &id, std::string pos, std::string matr);

    std::string getSDFSceneSource();

    std::string createFragmentSource(const std::string &filePath, const std::string &outPath);

    std::string createVertexSource(const std::string &filePath, const std::string &outPath) const;

    Primitive *m_canvas;
};

}


#endif // FIGURE_RENDER_HPP