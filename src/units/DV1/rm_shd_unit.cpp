#include "rm_shd_unit.hpp"

// Project namespace
namespace hse {


void rmShdUnit::init() {


    // Scene
    {
        factory = std::make_unique<FigureFactory>();
        Material mtl_light_grey;
        mtl_light_grey.color[0] = 223 / 255.;
        mtl_light_grey.color[1] = 150 / 255.;
        mtl_light_grey.color[2] = 44 / 255.;
        mtl_light_grey.color[3] = 1;
        Material mtl_dark_grey;
        mtl_dark_grey.color[0] = 230 / 255.;
        mtl_dark_grey.color[1] = 230 / 255.;
        mtl_dark_grey.color[2] = 250 / 255.;
        mtl_dark_grey.color[3] = 1;


        srand(12);
        Figure wall2 = factory->createBox(0.01, mtl_light_grey);
        Figure wall = wall2 * (::math::matr4::scale(::math::vec3(50, 30, 1)) * ::math::matr4::translate(math::vec3(0, 0, 0)));
        const int num_stones = 12;
        Figure stone = factory->createSphere(0.05, mtl_dark_grey);
        for (int i = 0; i < num_stones; i++) {
            wall = wall - (stone * ::math::matr4::translate(::math::vec3(float(rand())/RAND_MAX*0.6-0.3, float(rand())/RAND_MAX*0.6-0.3, float(rand())/RAND_MAX*0.08-0.04)));
        }
        Figure wall3 = (wall * (::math::matr4::translate(::math::vec3(0, 100, 10))));// * ::math::matr4::rotateX(math::PI / 2)));

        wall3.draw(unitPrimitive);
        std::string source = wall3.getFragmentSource("../data/shaders/rm/fragment_src.glsl");
        std::ofstream file("../data/shaders/rm/fragment.glsl");
        file << source;
        file.close();
    }
    ::std::vector<int> indexBuffer(6);
    ::std::vector<float> vertexBuffer = {-1, -1, 0,
                                         1, -1, 0,
                                         1, 1, 0,
                                         -1, -1, 0,
                                         1, 1, 0,
                                         -1, 1, 0};

    for (int j = 0; j < 6; j++)
        indexBuffer[j] = j;
    unitPrimitive = createPrimitive(
        "rm", vertexBuffer, "v3", indexBuffer
    );
    frameH = new int{static_cast<int>(render::renderInstance.getWindowHeight())};
    frameW = new int{static_cast<int>(render::renderInstance.getWindowWidth())};
    unitPrimitive->addUniform(frameW, "frame_w");
    unitPrimitive->addUniform(frameH, "frame_h");
    unitPrimitive->addUniform(&render::renderInstance.getTime(), "time");

}  // End of 'testUnit::initUnit' function

//std::vector<uint> parseFigures(const std::string &str, )
void rmShdUnit::response() {

}  // End of 'testUnit::responseUnit' function

// Class override destructor
rmShdUnit::~rmShdUnit() {
    delete frameH;
    delete frameW;
    ::std::cout << "Clear unit" << ::std::endl;
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


    Figure(const std::string &ser_deb, const std::vector<int> &ser, Builder *bldr, Type type) : serialized_debug(ser_deb), serialized(ser), bldr(bldr), type(type){
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
