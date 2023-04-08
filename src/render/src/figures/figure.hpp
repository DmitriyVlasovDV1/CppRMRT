#ifndef HSE_PROJECT_FIGURE_HPP
#define HSE_PROJECT_FIGURE_HPP

#include <variant>
#include "figure_transformation.hpp"

namespace hse {
enum class PrimitiveType { SPHERE, BOX };

class PrimitiveId {
public:
    PrimitiveId(PrimitiveType type, int id) : m_type(type), m_id(id) {
    }

    int id() const;
    PrimitiveType type() const;

private:
    PrimitiveType m_type;
    int m_id;
};

struct SpherePrimitive {
    SpherePrimitive(float radius) : radius(radius) {
    }

    float radius;
};

struct BoxPrimitive {
    BoxPrimitive(float size) : size(size) {
    }

    float size;
};

class FigureId {
public:
    FigureId(int id) : m_id(id) {
    }

    int id() const;

    FigureId operator&(const FigureId &other) const;

    FigureId operator|(const FigureId &other) const;

    FigureId operator/(const FigureId &other) const;

    FigureId &operator&=(const FigureId &other);

    FigureId &operator|=(const FigureId &other);

    FigureId &operator/=(const FigureId &other);

    FigureId &operator<<(const TransformationMatrixId &trId);

    FigureId &operator<<(const math::matr4 &matr);

    FigureId &operator<<(const TransformationBendId &trId);

    FigureId &operator<<(const TransformationTwistId &trId);

    void draw() const;

    void hide() const;

private:
    int m_id;
};

enum class CreationType { PRIMITIVE, INTERSECTION, UNION, SUBTRACTION };

class Figure {
public:
    Figure(PrimitiveType type, int id)
        : m_creationType(CreationType::PRIMITIVE),
          m_sources(PrimitiveId{type, id}),
          m_transforms() {
    }

    Figure(CreationType creationType, const std::vector<FigureId> &sources)
        : m_creationType(creationType), m_sources(sources), m_transforms() {
    }

    CreationType creationType() const;

    PrimitiveId getSourcePrimitive() const;

    std::vector<FigureId> getSourceFigures() const;

    const std::vector<TransformationId> & getTransformations() const;

    void addTransformation(const TransformationId &tr);

private:
    CreationType m_creationType;
    std::variant<PrimitiveId, std::vector<FigureId>> m_sources;
    std::vector<TransformationId> m_transforms;
};

}
#endif  // HSE_PROJECT_FIGURE_HPP
