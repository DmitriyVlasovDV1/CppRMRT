#include "rm_shd_unit.hpp"
#include <cmath>
#include "../../render/src/figures/figure_scene.hpp"

// Project namespace
namespace hse {

#define EXAMPLE 1
void rmShdScene::onCreate() {
    using namespace math;
    FigureScene &scene = Render::scene;
    auto floor = scene.createBox(1);
    floor << matr4::scale(vec3(20, 1, 20));
    floor.draw();
#if EXAMPLE == 1
    scaleId = scene.createScale(vec3(1, 2 + sin(time), 1));
    auto box = scene.createBox(1, {vec3(1, 0, 0)});
    auto sphere = scene.createSphere(1, {vec3(1, 1, 0)});
    sphere << matr4::translate(vec3(-0.5, 1.5, 0.5));
    sphere.draw();
    box << matr4::translate(vec3(0.5, 1, -0.5));
    //box &= sphere;
    box.draw();
#elif EXAMPLE == 2
    rotationId = scene.createRotation(math::vec3(0, 1, 0), 0);
    translateId = scene.createTranslation(vec3(0, 0, 0));
    scaleId = scene.createScale(vec3(1, 1, 1));

    auto stone = scene.createSphere(0.5, {vec3(1, 0, 0)});
    auto pat = scene.createBox(0.1, vec3(0, 1, 0));
    auto bnd = scene.createBend(vec3(1, 0, 0), vec3(0, 0, 1), vec3(-1, 0, 0));
    auto tw = scene.createTwist(vec3(0, 0, 0), vec3(0, 1, 0), 1);
    pat << matr4::scale(vec3(3, 10, 1)) << bnd << matr4::translate(vec3(-0.5, 0, 0)) << tw;
    int n = 6;
    for (int i = 0; i < n; i++) {
        stone /= (pat.copy() << matr4::rotate(float(360) * i / n, vec3(0, 1, 0)));
    }
    stone << translateId << scaleId;
    stone.draw();
#elif EXAMPLE == 3
    auto box = scene.createBox(0.5, {vec3(1, 0, 0)});
    auto sphere = scene.createSphere(0.1, vec3{0, 1, 0});
    bendId = scene.createBend(vec3(10 * sin(time), 0, 0), vec3(0, 0, 1), vec3(1, 1, 0) - vec3(10 * sin(time), 0, 0));
    twistId = scene.createTwist(vec3(0, 0, 0), vec3(0, 1, 0), 1);
    translateId = scene.createTranslation(vec3(10 * sin(time), 0, 0));
    box << matr4::scale(vec3(1, 5, 1)) << twistId << bendId;
    sphere << translateId;
    sphere.draw();
    //box &= sphere;
    box.draw();
    floor.hide();
#elif EXAMPLE == 4
    floor.hide();
    auto box = scene.createBox(2, {vec3(1, 0, 0)});
    auto sphere = scene.createSphere(1.4, {vec3(1, 1, 0)});
    box &= sphere;
    auto handle = scene.createBox(0.3, vec3(1, 1, 1));
    auto hole = scene.createBox(0.3, vec3(1, 1, 1));
    auto laser = scene.createBox(0.1, vec3(1, 0.5, 0));
    translateId = scene.createTranslation(vec3(0, 0, 0));
    translateId2 = scene.createTranslation(vec3(0.4, 0, 0));
    rotationId = scene.createRotation(vec3(1, 0, 0), 0);
    handle << matr4::scale(vec3(1, 10, 1)) << translateId << rotationId;
    hole << matr4::scale(vec3(8, 20, 2)) << translateId2 << translateId << rotationId;
    laser << matr4::scale(vec3(1, 27, 1)) << translateId << rotationId;
    //handle /= hole;
    box /= hole;
    box.draw();
    //handle.draw();
    laser.draw();
    //hole.draw();
#endif

    math::vec3 newCameraLocation = math::vec3(1, 0.7, 1) * 5;
    vec3 at = vec3(0, 1, 0);
    vec3 dir = (at - newCameraLocation).normalize();
    vec3 right = dir % vec3(0, 1, 0);
    vec3 up = (right % dir).normalize();
    scene.mainCamera.setAllAxis(newCameraLocation, dir, up, right);

}

// std::vector<uint> parseFigures(const std::string &str, )
void rmShdScene::onUpdate() {
    using namespace math;
    static float a = 0;
    FigureScene &scene = Render::scene;

    if (keys[GLFW_KEY_C].action == GLFW_PRESS) {
        scene.setRenderType(RenderType::COMMON);
    }
    if (keys[GLFW_KEY_R].action == GLFW_PRESS) {
        scene.setRenderType(RenderType::RM);
    }

#if EXAMPLE == 1
#elif EXAMPLE == 2
    float t = time * 2;
    float m = abs(sin(t)) - 0.3;
    translateId.set(matr4::translate(vec3(0, m + 1.3, 0)));
    float k = fmax(-abs(sin(t - 0.2)) + 0.3, 0) * 0.3;
    scaleId.set(matr4::scale(vec3(1 + k, 1 - k, 1 + k)));
#elif EXAMPLE == 3
    float t = time;
    vec3 pos(7 / (1 - fmax(-sin(t), 0)), 0 * cos(t), 0);
    //bendId.set(pos, vec3(0, 0, 1), vec3(0, 0, 0) - pos);
    twistId.set(vec3(0, 0, 0), vec3(0, 1, 0), fmax(0, sin(t)));
    bendId.set(pos, vec3(0, 0, 1), vec3(0) - pos);
    translateId.set(matr4::translate(pos));
#elif EXAMPLE == 4
    float t = time * 0.8;
    translateId.set(matr4::translate(vec3(sin(t) * 4, 0, 0)));
    if (cos(t) > 0) {
        translateId2.set(matr4::translate(vec3(-0.8, 0, 0)));
    } else {
        translateId2.set(matr4::translate(vec3(0.8, 0, 0)));
    }
    if (abs(cos(t)) < 0.2) {
        a += 3 * deltaTime;
        rotationId.set(matr4::rotate(10 * a, vec3(sin(a), 1, sin(a + 2))));
    }
#endif
    math::vec3 newCameraLocation = math::vec3(sin(time), 0.7, cos(time)) * 5;
    vec3 at = vec3(0, 1, 0);
    vec3 dir = (at - newCameraLocation).normalize();
    vec3 right = dir % vec3(0, 1, 0);
    vec3 up = (right % dir).normalize();
    scene.mainCamera.setAllAxis(newCameraLocation, dir, up, right);

    //rotationId.set(math::matr4::rotate(time * 10, math::vec3(0, 1, 0)));
    // translateId = math::matr4::translate(math::vec3(sin(time) * 4, 0, 0));
    // rotationId = math::matr4::rotate(time * 10, math::vec3(1, 0, 0));
    /*
    auto pos = math::vec3(sin(time), 0, cos(time)) * 7;
    scene.mainCamera.setPositionWithDirection(
        pos, math::vec3(0) - pos
    );
     */

}  // End of 'testUnit::responseUnit' function

// Class override destructor
rmShdScene::~rmShdScene() {
}  // End of 'testUnit::~testUnit' function
}  // namespace hse

/*

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


struct BrickId {
    char type;
    int ind;
};

class Figure;

class Builder {
public:
    std::vector<SphereBrick> spheres;
    std::vector<BoxBrick> boxes;
    std::vector<::math::matr4> matrices;

    BrickId createBox(float radius, Material mtl) {
        BoxBrick br;
        br.radius = radius;
        br.mtl = mtl;
        boxes.push_back(br);
        int ind = static_cast<int>(boxes.size() - 1);
        return {'b', ind};
    }

    BrickId createSphere(float radius, Material mtl) {
        SphereBrick br;
        br.radius = radius;
        br.mtl = mtl;
        spheres.push_back(br);
        int ind = static_cast<int>(spheres.size() - 1);
        return {'s', ind};
    }

    BrickId addMatr(const ::math::matr4 &matr) {
        matrices.push_back(matr);
        int ind = static_cast<int>(matrices.size() - 1);
        return {'m', ind};
    }

};

class Figure {
    enum class Type {
        UNION,
        INTER,
        SUB,
        BRICK,
        MATR
    };

public:
    std::string serialized_debug;
    std::vector<int> serialized;
    Builder *bldr;
    std::vector<int> matrs;
    Type type;


    Figure(BrickId brickId, Builder *bldr) : serialized_debug(brickId.type + std::to_string(brickId.ind)), bldr(bldr) {
        BrickId id = bldr->addMatr(math::matr4::identity());
        serialized.push_back(id.type);
        serialized.push_back(id.ind);
        serialized.push_back(brickId.type);
        serialized.push_back(brickId.ind);
        matrs.push_back(id.ind);
        type = Type::BRICK;
    }


    Figure(const std::string &ser_deb, const std::vector<int> &ser, Builder *bldr, Type type) :
serialized_debug(ser_deb), serialized(ser), bldr(bldr), type(type){
    }

    Figure operator|(const Figure &fig) const {
        std::vector<int> new_ser;
        std::string new_deb;
        if (type == Type::UNION) {
            new_ser.insert(new_ser.end(), serialized.begin(), serialized.end() - 2);
            new_deb.insert(new_deb.end(), serialized_debug.begin(), serialized_debug.end() - 2);
        }
        else
        {
            new_ser.push_back('E');
            new_ser.insert(new_ser.end(), serialized.begin(), serialized.end());

            new_deb.push_back('E');
            new_deb.insert(new_deb.end(), serialized_debug.begin(), serialized_debug.end());
        }
        if (fig.type == Type::UNION) {
            new_ser.insert(new_ser.end(), fig.serialized.begin() + 1, fig.serialized.end() - 1);
            new_deb.insert(new_deb.end(), fig.serialized_debug.begin() + 1, fig.serialized_debug.end() - 1);
        }
        else {
            new_ser.insert(new_ser.end(), fig.serialized.begin(), fig.serialized.end());
            new_deb.insert(new_deb.end(), fig.serialized_debug.begin(), fig.serialized_debug.end());
        }

        new_ser.push_back('U');
        new_deb.push_back('U');
        std::vector<int> new_matrs(matrs);
        new_matrs.insert(new_matrs.end(), fig.matrs.begin(), fig.matrs.end());
        return {new_deb,
                new_ser, bldr, Type::UNION};
    }

    Figure operator&(const Figure &fig) const {
        std::vector<int> new_ser = {'I'};
        new_ser.insert(new_ser.end(), serialized.begin(), serialized.end());
        new_ser.insert(new_ser.end(), fig.serialized.begin(), fig.serialized.end());
        new_ser.push_back('E');
        std::vector<int> new_matrs(matrs);
        new_matrs.insert(new_matrs.end(), fig.matrs.begin(), fig.matrs.end());
        return {"I(" + serialized_debug + ", " + fig.serialized_debug + ")" + "E",
                new_ser, bldr, Type::INTER};
    }

    Figure operator-(const Figure &fig) const {

        std::vector<int> new_ser = {'S'};
        new_ser.insert(new_ser.end(), serialized.begin(), serialized.end());
        new_ser.insert(new_ser.end(), fig.serialized.begin(), fig.serialized.end());
        new_ser.push_back('E');
        std::vector<int> new_matrs(matrs);
        new_matrs.insert(new_matrs.end(), fig.matrs.begin(), fig.matrs.end());
        return {"S(" + serialized_debug + ", " + fig.serialized_debug + ")" + "E",
                new_ser, bldr, Type::SUB};
    }

    Figure operator*(const ::math::matr4 &matr) {
        BrickId id = bldr->addMatr(matr);
        BrickId id1 = bldr->addMatr(matr.inverting());
        std::vector<int> new_ser = {'m'};
        new_ser.push_back(id.ind);
        new_ser.insert(new_ser.end(), serialized.begin(), serialized.end());
        new_ser.push_back('m');
        new_ser.push_back(id1.ind);
        return {" m[" + std::to_string(id.ind) + "] " + serialized_debug + " M[" + std::to_string(id1.ind) + "] ",
                serialized, bldr, Type::MATR};
        return *this;
    }


};


*/
