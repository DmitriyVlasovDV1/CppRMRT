#ifndef FIGURE_RENDER_HPP
#define FIGURE_RENDER_HPP
#include "src/buffers/buffer.hpp"
#include "figure_scene.hpp"

namespace hse::drawable_figures {


class FigureRender {
public:
    virtual void init(FigureScene &scene) = 0;
    virtual void render(FigureScene &scene) = 0;
};

class CommonRender : public FigureRender {
private:
    std::vector<primitive*> m_primitives;

public:

    void init(FigureScene &scene) final {
        for (auto &box : scene.m_boxes) {
            // TODO real parameters
            scene.createPrimitive("", {});

        }
    }

    void render(FigureScene &scene) final {
       // TODO realization
    }

};

class RMRender : public FigureRender {
private:
    primitive *m_canvas;
public:
    shaderStorageBuffer m_spheresSSBO;
    shaderStorageBuffer m_boxesSSBO;
    shaderStorageBuffer m_matricesSSBO;

    void init(FigureScene &scene) final;

    void render(FigureScene &scene) final;
};

}


#endif // FIGURE_RENDER_HPP