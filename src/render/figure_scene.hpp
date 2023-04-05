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

class PrimitiveId {
public:
    PrimitiveId(PrimitiveType type, int id) : m_type(type), m_id(id) {
    }
    PrimitiveType m_type;
    const int m_id;
};
class  SpherePrimitive {
public:
    SpherePrimitive(float radius) : m_radius(radius) {
    }

    float m_radius;
}; // SpherePrimitive

class  BoxPrimitive {
public:
    BoxPrimitive(float size) : m_size(size) {
    }

    float m_size;
}; // BoxPrimitive

/*
 * Transformations
 */
class TransformationId {
    // TODO need link to storage
public:
    TransformationId() = default;
    TransformationId(int id) : m_id(id) {
    }

    // TODO operations with other instances and Transformations
    TransformationId & operator=(const math::matr4 &matr);

    TransformationId & operator*=(const math::matr4 &matr);

    int m_id;
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

    FigureId operator|(const FigureId &other);

    FigureId operator/(const FigureId &other);

    FigureId operator&=(const FigureId &other);

    FigureId operator|=(const FigureId &other);

    FigureId operator/=(const FigureId &other);

    FigureId & operator<<(const TransformationId &trId);

    FigureId & operator<<(const math::matr4 &matr);


    // add instance for main scene
    void draw() const;

    int m_id;
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
public:
    explicit Figure(PrimitiveType type, int id) : m_creationType(CreationType::PRIMITIVE),
          m_sources(PrimitiveId{type, id}), m_transforms() {
    }
    Figure(CreationType creationType, const std::vector<FigureId> &sources) : m_creationType(creationType),
          m_sources(sources), m_transforms() {
    }

    CreationType m_creationType;
    std::variant<PrimitiveId, std::vector<FigureId>> m_sources;
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


    FigureScene();

    void setRenderType(RenderType renderType);

    FigureId createBox(float size);

    FigureId createSphere(float radius);

    TransformationId createTransformation(const math::matr4 &matr);

    TransformationId createTranslation(const math::vec3 &vec);

    TransformationId createRotation(const math::vec3 &vec, const float deg);

    TransformationId createScale(const math::vec3 &vec);
    // TODO other primitives

public:
    void init() override;

    void response() override;

    FigureId createUnion(const FigureId &a, const FigureId &b);

    FigureId createIntersection(const FigureId &a, const FigureId &b);

    FigureId createSubtraction(const FigureId &a, const FigureId &b);

    std::vector<BoxPrimitive> m_boxes;
    std::vector<SpherePrimitive> m_spheres;
    std::vector<Figure> m_figures;
    std::vector<Material> m_materials;
    std::vector<math::matr4> m_matrices;
    std::set<FigureId, FigureIdHasher> m_scene;
    RenderType m_curRenderType;
    std::map<RenderType, std::shared_ptr<FigureRender>> m_renders;

    void draw(const FigureId &id);
    void hide(const FigureId &id);

}; // FigureRender

}
#endif  // RM_FIGURES_HPP
