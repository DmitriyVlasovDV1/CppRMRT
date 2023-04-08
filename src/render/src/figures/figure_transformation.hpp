#ifndef HSE_PROJECT_FIGURE_TRANSFORMATION_HPP
#define HSE_PROJECT_FIGURE_TRANSFORMATION_HPP

namespace hse {
enum class TransformationType { MATRIX, TWIST, BEND };

class FigureScene;

class TransformationId {
protected:
    TransformationId(TransformationType type, int id) : m_type(type), m_id(id) {
    }

public:
    TransformationId() {
    }

    int id() const;
    TransformationType type() const;

private:
    TransformationType m_type;
    int m_id;
};

class TransformationMatrixId : public TransformationId {
    friend FigureScene;
private:
    TransformationMatrixId(int id) : TransformationId(TransformationType::MATRIX, id) {
    }
public:
    TransformationMatrixId() {

    }
    void set(const math::matr4 &matr) const;
};

class TransformationBendId : public TransformationId {
    friend FigureScene;
private:
    TransformationBendId(int id) : TransformationId(TransformationType::BEND, id) {
    }

public:
    TransformationBendId() {
    }

    void set(
        const math::vec3 &pos,
        const math::vec3 &dir,
        const math::vec3 &rad
    ) const;
};

class TransformationTwistId : public TransformationId {
    friend FigureScene;
private:
    TransformationTwistId(int id) : TransformationId(TransformationType::TWIST, id) {
    }
public:
    TransformationTwistId() {

    }
    void set(
        const math::vec3 &pos,
        const math::vec3 &dir,
        float intensity
    ) const;
};

// struct compatible with ssbo
struct alignas(16) TransformationTwist {
    // TODO change for vec4
    float pos[4] = {};
    float dir[4] = {};
    float intensity;

    TransformationTwist(
        const math::vec3 &pos_,
        const math::vec3 &dir_,
        float intensity_
    )
        : intensity(intensity_) {
        pos[0] = pos_.x;
        pos[1] = pos_.y;
        pos[2] = pos_.z;
        pos[3] = 1;

        dir[0] = dir_.x;
        dir[1] = dir_.y;
        dir[2] = dir_.z;
        dir[3] = 0;
    }
};

// struct compatible with ssbo
struct alignas(16) TransformationBend {
    // TODO change for vec4
    float pos[4] = {};
    float dir[4] = {};
    float rad[4] = {};

    TransformationBend(
        const math::vec3 &pos_,
        const math::vec3 &dir_,
        const math::vec3 &rad_
    ) {
        pos[0] = pos_.x;
        pos[1] = pos_.y;
        pos[2] = pos_.z;
        pos[3] = 1;

        dir[0] = dir_.x;
        dir[1] = dir_.y;
        dir[2] = dir_.z;
        dir[3] = 0;

        rad[0] = rad_.x;
        rad[1] = rad_.y;
        rad[2] = rad_.z;
        rad[3] = 0;
    }
};
} // namespace "hse"

#endif  // HSE_PROJECT_FIGURE_TRANSFORMATION_HPP
