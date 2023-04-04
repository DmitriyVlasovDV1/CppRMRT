#ifndef FIGURE_SCENE_HPP_
#define FIGURE_SCENE_HPP_

#include <memory>
#include <exception>
#include <variant>
#include "../def.hpp"
#include "src/buffers/buffer.hpp"
#include "src/units/unit.hpp"


namespace hse::drawable_figures {

class FigureRender;


/*
 * Primitives
 */
enum class PrimitiveType {
    SPHERE,
    BOX
}; // PrimitiveType

class SpherePrimitive {
public:
    SpherePrimitive(float radius) : m_radius(radius) {
    }

private:
    float m_radius;
}; // SpherePrimitive

class BoxPrimitive {
public:
    BoxPrimitive(float size) : m_size(size) {
    }

private:
    float m_size;
}; // BoxPrimitive

/*
 * Transformations
 */
class TransformationId {
    // TODO need link to storage
public:
    TransformationId(int id) : m_storage_id(id) {
    }

    // TODO operations with other instances and Transformations

private:
    const int m_storage_id;
}; // TransformationId

enum class TransformationType {
    ROTATION,
    TRANSLATION,
    SCALE
}; // TransformationType

class Transformation {
public:
    Transformation(TransformationType type) : m_transformationType(type) {
    }

    TransformationType m_transformationType;
}; // Transformation

class Translation : public Transformation {
public:
    Translation(const math::vec3 &vec) : Transformation(TransformationType::TRANSLATION), m_vec(vec) {
    }
private:
    math::vec3 m_vec;
}; // Translation

class Rotation : public Transformation {
public:
    Rotation(const math::vec3 &vec, float deg) : Transformation(TransformationType::ROTATION), m_vec(vec),
          m_deg(deg) {
    }
private:
    math::vec3 m_vec;
    float m_deg;
}; // Rotation

class Scale : public Transformation {
public:
    Scale(const math::vec3 &vec) : Transformation(TransformationType::SCALE), m_vec(vec)
    {
    }
private:
    math::vec3 m_vec;
}; // Scale

/*
 * Materials
 */
class MaterialId {
public:
    MaterialId(int id) : m_id(id) {
    }
private:
    const int m_id;
}; // MaterialId

class Material {
    math::vec3 m_color;
}; // Material

/*
 * Figures
 */
class FigureId;
struct FigureIdHasher {
    bool operator() (const FigureId &a, const FigureId &b) const;
};
class FigureId {
    friend FigureIdHasher;
public:
    FigureId(int id) : m_id(id) {
    }

    // intersection
    FigureId operator&(const FigureId &other);

    // add instance for main scene
    void draw() const;

private:
    const int m_id;
}; // FigureId

enum class CreationType {
    PRIMITIVE,
    INTERSECTION,
    UNION,
    SUBTRACTION
}; // CreationType

class FigureScene;
class Figure {
    friend FigureScene;
private:
    explicit Figure(int id) : m_creationType(CreationType::PRIMITIVE),
          m_sources(id), m_transforms() {
    }
    Figure(CreationType creationType, const std::vector<FigureId> &sources) : m_creationType(CreationType::PRIMITIVE),
          m_sources(sources), m_transforms() {
    }

    CreationType m_creationType;
    std::variant<int, std::vector<FigureId>> m_sources;
    std::vector<TransformationId> m_transforms;
}; // Figure


/*
 * Render
 */
enum class RenderType {
    COMMON,
    RM,
    RT
}; // RenderType

class FigureScene;


class CommonRender;
class FigureRender;
class RMRender;
class RTRender;
// single tone in render
class FigureScene : public unit {
    // Friendzone
    friend CommonRender;
    friend RMRender;
    friend FigureId;
public:


    FigureScene() = default; // TODO provide only for render

    void setRenderType(RenderType renderType);

    FigureId createBox(float size, MaterialId mtl);

    FigureId createSphere(float radius);

    TransformationId createTranslation(const math::vec3 &vec);

    TransformationId createRotation(const math::vec3 &vec, const float deg);

    TransformationId createScale(const math::vec3 &vec);
    // TODO other primitives

private:
    void init() override;

    void response() override;

    FigureId createIntersection(const FigureId &a, const FigureId &b);

    std::vector<BoxPrimitive> m_boxes;
    std::vector<SpherePrimitive> m_spheres;
    std::vector<Figure> m_figures;
    std::vector<Material> m_materials;
    std::vector<math::matr4> m_matrices;
    std::set<FigureId, FigureIdHasher> m_scene;
    RenderType m_curRenderType;
    //std::map<RenderType, std::unique_ptr<FigureRender>> m_renders;

    void draw(const FigureId &id);
    void hide(const FigureId &id);

}; // FigureRender

}
#endif  // RM_FIGURES_HPP
