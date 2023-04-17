#include "../../render.hpp"
#include "figure.hpp"

namespace hse {

int PrimitiveId::id() const {
    return m_id;
}

PrimitiveType PrimitiveId::type() const {
    return m_type;
}

int FigureId::id() const {
    return m_id;
}

FigureId FigureId::copy() const {
    return Render::scene.createCopy(*this);
}

FigureId FigureId::operator&(const FigureId &other) const {
    return Render::scene.createIntersection(*this, other);
}

FigureId FigureId::operator|(const FigureId &other) const {
    return Render::scene.createUnion(*this, other);
}

FigureId FigureId::operator/(const FigureId &other) const {
    return Render::scene.createSubtraction(*this, other);
}

FigureId FigureId::operator%(const FigureId &other) const {
    return Render::scene.createSUnion(*this, other);
}

FigureId & FigureId::operator&=(const FigureId &other) {
    *this = Render::scene.createIntersection(*this, other);
    return *this;
}

FigureId & FigureId::operator|=(const FigureId &other) {
    *this = Render::scene.createUnion(*this, other);
    return *this;
}

FigureId & FigureId::operator/=(const FigureId &other) {
    *this = Render::scene.createSubtraction(*this, other);
    return *this;
}

FigureId & FigureId::operator%=(const FigureId &other) {
    *this = Render::scene.createSUnion(*this, other);
    return *this;
}

FigureId & FigureId::operator<<(const TransformationMatrixId &trId) {
    Render::scene.getFigureById(*this).addTransformation(trId);
    return *this;
}

FigureId & FigureId::operator<<(const math::matr4 &matr) {
    TransformationId trId = Render::scene.createTransformation(matr);
    Render::scene.getFigureById(*this).addTransformation(trId);
    return *this;
}
FigureId & FigureId::operator<<(const TransformationBendId &trId) {
    Render::scene.getFigureById(*this).addTransformation(trId);
    return *this;
}

FigureId & FigureId::operator<<(const TransformationTwistId &trId) {
    Render::scene.getFigureById(*this).addTransformation(trId);
    return *this;
}

void FigureId::draw() const {
    Render::scene.draw(*this);
}

void FigureId::hide() const {
    Render::scene.hide(*this);
}

CreationType Figure::creationType() const {
    return m_creationType;
}

PrimitiveId Figure::getSourcePrimitive() const {
    return std::get<PrimitiveId>(m_sources);
}

std::vector<FigureId> Figure::getSourceFigures() const {
    return std::get<std::vector<FigureId>>(m_sources);
}

void Figure::addTransformation(const TransformationId &tr) {
    m_transforms.push_back(tr);
}

const std::vector<TransformationId> & Figure::getTransformations() const {
    return m_transforms;
}

}
