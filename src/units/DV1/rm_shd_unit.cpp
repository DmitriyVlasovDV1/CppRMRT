#include "rm_shd_unit.hpp"
#include "mth.h"
// Project namespace
namespace hse {




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
    std::vector<mth::matr> matrices;

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

    BrickId addMatr(const mth::matr &matr) {
        matrices.push_back(matr);
        int ind = static_cast<int>(matrices.size() - 1);
        return {'m', ind};
    }

};

class Figure {
public:
    std::string serialized_debug;
    std::vector<int> serialized;
    Builder *bldr;
    std::vector<int> matrs;


    Figure(BrickId brickId, Builder *bldr) : serialized_debug(brickId.type + std::to_string(brickId.ind)), bldr(bldr) {
        BrickId id = bldr->addMatr(mth::matr::Identity());
        serialized.push_back(id.type);
        serialized.push_back(id.ind);
        serialized.push_back(brickId.type);
        serialized.push_back(brickId.ind);
        matrs.push_back(id.ind);
    }


    Figure(const std::string &ser_deb, const std::vector<int> &ser, std::vector<int> &matrs_, Builder *bldr) : serialized_debug(ser_deb), serialized(ser), matrs(matrs_), bldr(bldr){
        BrickId id = bldr->addMatr(mth::matr::Identity());
        matrs.push_back(id.ind);
    }

    Figure operator|(const Figure &fig) const {
        std::vector<int> new_ser = {'U'};
        new_ser.insert(new_ser.end(), serialized.begin(), serialized.end());
        new_ser.insert(new_ser.end(), fig.serialized.begin(), fig.serialized.end());
        new_ser.push_back('E');
        std::vector<int> new_matrs(matrs);
        new_matrs.insert(new_matrs.end(), fig.matrs.begin(), fig.matrs.end());
        return {"U(" + serialized_debug + ", " + fig.serialized_debug + ")" + "E",
                    new_ser, new_matrs, bldr};
    }

    Figure operator&(const Figure &fig) const {
        std::vector<int> new_ser = {'I'};
        new_ser.insert(new_ser.end(), serialized.begin(), serialized.end());
        new_ser.insert(new_ser.end(), fig.serialized.begin(), fig.serialized.end());
        new_ser.push_back('E');
        std::vector<int> new_matrs(matrs);
        new_matrs.insert(new_matrs.end(), fig.matrs.begin(), fig.matrs.end());
        return {"I(" + serialized_debug + ", " + fig.serialized_debug + ")" + "E",
                new_ser, new_matrs, bldr};
    }

    Figure operator-(const Figure &fig) const {

        std::vector<int> new_ser = {'S'};
        new_ser.insert(new_ser.end(), serialized.begin(), serialized.end());
        new_ser.insert(new_ser.end(), fig.serialized.begin(), fig.serialized.end());
        new_ser.push_back('E');
        std::vector<int> new_matrs(matrs);
        new_matrs.insert(new_matrs.end(), fig.matrs.begin(), fig.matrs.end());
        return {"S(" + serialized_debug + ", " + fig.serialized_debug + ")" + "E",
                new_ser, new_matrs, bldr};
    }

    Figure operator*(const mth::matr &matr) {
        for (auto &m : matrs) {
            bldr->matrices[m].copy(matr.EvalInverse() * bldr->matrices[m]);
        }
        return *this;
    }


};


/*
 * Builder builder;
 * BoxBrick box;
 * SphereBrick shpere;
 * int boxId = builder.addBrick(box);
 * int sphId = builder.addBrick(sph);
 * Figure fig(boxId);
 * Figure fig2(sphId);
 * Figure inter = fig ^ fig2;
 */



void rmShdUnit::init() {
    /*
    for (char ch = 'a'; ch != 'z'; ch++)
        std::cout << ch << " " << int(ch) << std::endl;
    for (char ch = 'A'; ch != 'Z'; ch++)
        std::cout << ch << " " << int(ch) << std::endl;
        */


    ::std::vector<int> indexBuffer(6);
    ::std::vector<float> vertexBuffer = {-1, -1, 0,
                                          1, -1, 0,
                                          1, 1, 0,
                                          -1, -1, 0,
                                          1, 1, 0,
                                          -1, 1, 0};

    for (int j = 0; j < 6; j++)
        indexBuffer[j] = j;
    auto unitPrimitive = createPrimitive(
        "rm", vertexBuffer, "v3", indexBuffer
    );
    frameH = new int{static_cast<int>(render::renderInstance.getWindowHeight())};
    frameW = new int{static_cast<int>(render::renderInstance.getWindowWidth())};

}  // End of 'testUnit::initUnit' function

//std::vector<uint> parseFigures(const std::string &str, )
void rmShdUnit::response() {
    static bool flag = false;
    if (flag)
        return;
    flag = true;
    float time = render::renderInstance.getTime();
    // Castle
    Builder bldr;
    Material mtl_light_grey;
    mtl_light_grey.color[0] = 95 / 255.;
    mtl_light_grey.color[1] = 158 / 255.;
    mtl_light_grey.color[2] = 160 / 255.;
    mtl_light_grey.color[3] = 1;
    Material mtl_dark_grey;
    mtl_dark_grey.color[0] = 230 / 255.;
    mtl_dark_grey.color[1] = 230 / 255.;
    mtl_dark_grey.color[2] = 250 / 255.;
    mtl_dark_grey.color[3] = 1;

    srand(130);

    Figure wall2(bldr.createBox(0.1, mtl_light_grey), &bldr);
    Figure wall = wall2 * mth::matr::Scale(10, 6, 1);
    const int num_stones = 6;
    Figure stone(bldr.createSphere(0.3, mtl_dark_grey), &bldr);
    for (int i = 0; i < num_stones; i++) {
        wall = wall | (stone * mth::matr::Translate(float(rand())/RAND_MAX*1-0.5, float(rand())/RAND_MAX*0.6-0.3, 0.1));

    }
/*
    Figure box(bldr.createBox(1, mtl_light_grey), &bldr);
    Figure box2 = box * mth::matr::Scale(1, 1, 1);
    Figure sph(bldr.createSphere(1, mtl_dark_grey), &bldr);
    Figure upSph = sph * mth::matr::Translate(1, 0, 0);
    Figure diff = box2 - upSph;
    Figure diffWorld = diff * mth::matr::Identity();

    std::cout << diffWorld.serialized_debug << std::endl;
    for (int i = 0; i < diffWorld.serialized.size(); i++) {
        std::cout << diffWorld.serialized[i] << " ";
    }
    */
    Figure scene = wall;
    std::cout << std::endl;
    GLuint ssbo, ssbo2, ssbo3, ssbo4, ssbo5, ssbo6;
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(SphereBrick) * bldr.spheres.size(), bldr.spheres.data(), GL_STATIC_DRAW); // ??? GL_DYNAMIC_COPY mb GL_STATIC_DRAW
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind

    glGenBuffers(1, &ssbo2);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo2);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(BoxBrick) * bldr.boxes.size(), bldr.boxes.data(), GL_STATIC_DRAW); // ??? GL_DYNAMIC_COPY mb GL_STATIC_DRAW
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo2);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind

    glGenBuffers(1, &ssbo3);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo3);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(mth::matr) * bldr.matrices.size(), bldr.matrices.data(), GL_STATIC_DRAW); // ??? GL_DYNAMIC_COPY mb GL_STATIC_DRAW
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, ssbo3);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind

    glGenBuffers(1, &ssbo4);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo4);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * scene.serialized.size(), scene.serialized.data(), GL_STATIC_DRAW); // ??? GL_DYNAMIC_COPY mb GL_STATIC_DRAW
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, ssbo4);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind

    /*
    struct alignas(16) Sphere {
        struct alignas(16) Material {
            float color[4] = {1, 0, 1, 1};
        } mtl;
        float radius = 0.5;
    };
    struct alignas(16) Box {
        struct alignas(16) Material {
            float color[4] = {1, 1, 0, 1};
        } mtl;
        float corner[4] = {1, 1, 1, 0};
    }box;

    Sphere data[1];
    std::cout << sizeof(data) << std::endl;
*/
    /*
    data[1].mtl.color[0] = 0;
    data[1].mtl.color[1] = 1;
    data[1].mtl.color[2] = 0;

    data[1].center[0] = 0.5;
    data[1].center[1] = 0;
    data[1].center[2] = 0;

    GLuint ssbo, ssbo2, ssbo3, ssbo4;
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(data), data, GL_DYNAMIC_COPY); // ??? GL_DYNAMIC_COPY mb GL_STATIC_DRAW
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind

    float time = render::renderInstance.getTime();
    mth::matr matr = mth::matr::Scale(1, 2, 1) * mth::matr::RotateX(render::renderInstance.getTime() * 30) * mth::matr::RotateY(render::renderInstance.getTime() * 30) * mth::matr::Translate(0, sin(time), 0);
    struct alignas(16) Matr {
        float a00, a01, a02, a03;
        float a10, a11, a12, a13;
        float a20, a21, a22, a23;
        float a30, a31, a32, a33;
    };
    Matr matr2 = {1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1};
    glGenBuffers(1, &ssbo2);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo2);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(matr), matr, GL_DYNAMIC_COPY); // ??? GL_DYNAMIC_COPY mb GL_STATIC_DRAW
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo2);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind

    glGenBuffers(1, &ssbo3);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo3);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(box), &box, GL_DYNAMIC_COPY); // ??? GL_DYNAMIC_COPY mb GL_STATIC_DRAW
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, ssbo3);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind

    // Bricks
    glGenBuffers(1, &ssbo4);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo4);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(box), &box, GL_DYNAMIC_COPY); // ??? GL_DYNAMIC_COPY mb GL_STATIC_DRAW
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, ssbo3);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind

    *frameH = static_cast<int>(render::renderInstance.getWindowHeight());
    *frameW = static_cast<int>(render::renderInstance.getWindowWidth());
     */
}  // End of 'testUnit::responseUnit' function

// Class override destructor
rmShdUnit::~rmShdUnit() {
    delete frameH;
    delete frameW;
    ::std::cout << "Clear unit" << ::std::endl;
}  // End of 'testUnit::~testUnit' function
}  // namespace hse