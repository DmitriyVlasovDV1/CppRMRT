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

FigureId FigureId::operator&(const FigureId &other) const {
    return render::renderInstance.scene.createIntersection(*this, other);
}

FigureId FigureId::operator|(const FigureId &other) const {
    return render::renderInstance.scene.createUnion(*this, other);
}

FigureId FigureId::operator/(const FigureId &other) const {
    return render::renderInstance.scene.createSubtraction(*this, other);
}

FigureId & FigureId::operator&=(const FigureId &other) {
    *this = render::renderInstance.scene.createIntersection(*this, other);
    return *this;
}

FigureId & FigureId::operator|=(const FigureId &other) {
    *this = render::renderInstance.scene.createUnion(*this, other);
    return *this;
}

FigureId & FigureId::operator/=(const FigureId &other) {
    *this = render::renderInstance.scene.createSubtraction(*this, other);
    return *this;
}

FigureId & FigureId::operator<<(const TransformationMatrixId &trId) {
    render::renderInstance.scene.getFigureById(*this).addTransformation(trId);
    return *this;
}

FigureId & FigureId::operator<<(const math::matr4 &matr) {
    TransformationId trId = render::renderInstance.scene.createTransformation(matr);
    render::renderInstance.scene.getFigureById(*this).addTransformation(trId);
    return *this;
}
FigureId & FigureId::operator<<(const TransformationBendId &trId) {
    render::renderInstance.scene.getFigureById(*this).addTransformation(trId);
}

FigureId & FigureId::operator<<(const TransformationTwistId &trId) {
    render::renderInstance.scene.getFigureById(*this).addTransformation(trId);
}

void FigureId::draw() const {
    render::renderInstance.scene.draw(*this);
}

void FigureId::hide() const {
    render::renderInstance.scene.hide(*this);
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
