#ifndef FIGURE_RENEDER_HPP_
#define FIGURE_RENEDER_HPP_

#include <memory>
#include <exception>
#include <variant>
#include "../def.hpp"

namespace hse::drawable_figures {

class FigureRender;

enum class RenderType {
    SVYATOSLAVSRENDEROK,
    RM,
    RT
}; // RenderType

/*
 * Primitives
 */
enum class PrimitiveType {
    SPHERE,
    BOX
}; // PrimitiveType

class Primitive {
    PrimitiveType m_primitiveType;
}; // Primitive

class SpherePrimitive : public Primitive {
public:
    SpherePrimitive(float radius) : m_radius(radius) {
    }

private:
    float m_radius;
}; // SpherePrimitive

class BoxPrimitive : public Primitive {
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
class FigureId {
    // TODO need link to storage
public:
    FigureId(int id) : m_id(id) {
    }

    // intersection
    FigureId operator&(const FigureId &other);

    // add instance for main scene
    void draw(RenderType renderType);

private:
    const int m_id;
}; // FigureId

enum class CreationType {
    PRIMITIVE,
    INTERSECTION,
    UNION,
    SUBTRACTION
}; // CreationType

class FigureRender;
class Figure {
    friend FigureRender;
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

// single tone in render
class FigureRender {
public:
    static FigureRender figureRender;

    friend FigureId;
    std::vector<std::unique_ptr<Primitive>> m_primitives;
    std::vector<Figure> m_figures;
    std::vector<Material> m_materials;
    std::vector<std::unique_ptr<Transformation>> m_transformations;
    std::vector<FigureId> m_forDrawing;

    FigureId createBox(float size, MaterialId mtl);

    FigureId createSphere(float radius);

    TransformationId createTranslation(const math::vec3 &vec);

    TransformationId createRotation(const math::vec3 &vec, const float deg);

    TransformationId createScale(const math::vec3 &vec);
    // TODO other primitives

private:
    FigureRender() = default;
    FigureId createIntersection(const FigureId &a, const FigureId &b);

}; // FigureRender

}
#endif  // RM_FIGURES_HPP
