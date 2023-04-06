#ifndef RM_FIGURES_HPP
#define RM_FIGURES_HPP

#include "../../render/src/units/unit.hpp"

namespace hse {

class FigureFactory;
class Figure {
    friend FigureFactory;

    enum class Type {
        BRICK,
        UNITE,
        INTER,
        SUB,
    } type;

private:
    FigureFactory *factory;
    std::vector<int> serialized;

    Figure(FigureFactory *factory_) : factory(factory_), serialized(), type(Type::BRICK) {
    }

    std::vector<int> wrap(char left, const std::vector<int> &ser, char right);

    std::vector<int> wrap(const std::string &left, const std::vector<int> &ser, const std::string &right);

public:
    std::vector<int> concatenate(const Figure &other);

    std::vector<int> concatenateAssociative(Type target, const Figure &other);

    // intersection figures operator
    Figure & operator=(const Figure &other);

    // intersection figures operator
    Figure operator&(const Figure &other);

    // unite figures operator
    Figure operator|(const Figure &other);

    // sub figures operator
    Figure operator-(const Figure &other);

    // sub figures operator
    Figure operator*(const math::matr4 &matr);

    std::string parseExpr(int &ind) const;

    std::string getSdfSceneSource() const;

    std::string getFragmentSource(const std::string &filepath) const;
    std::string getVertexSource(const std::string &filepath) const;

    void draw(Primitive *primitive) const;

    // TODO |= &= -=
};

struct alignas(16) Material {
    float color[4];
};

struct alignas(16) SphereBrick {
    Material mtl;
    float radius;
};

struct alignas(16) BoxBrick {
    Material mtl;
    float radius;
};

class FigureFactory {
    friend Figure;

private:
    std::vector<BoxBrick> boxes;
    std::vector<SphereBrick> spheres;
    std::vector<math::matr4> matricies;

    int addMatr(const math::matr4 &matr);

public:
    Figure createSphere(float radius, Material mtl={});

    Figure createBox(float radius, Material mtl={});

};

}
#endif  // RM_FIGURES_HPP
