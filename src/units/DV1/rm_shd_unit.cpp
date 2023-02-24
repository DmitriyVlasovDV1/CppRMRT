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


    Figure(BrickId brickId, Builder *bldr) : serialized_debug(brickId.type + std::to_string(brickId.ind)), bldr(bldr) {
        serialized.push_back(brickId.type);
        serialized.push_back(brickId.ind);
    }

    Figure(const std::string &ser_deb, const std::vector<int> &ser, Builder *bldr) : serialized_debug(ser_deb), serialized(ser), bldr(bldr){
    }

    Figure operator|(const Figure &fig) const {
        std::vector<int> new_ser = {'U'};
        new_ser.insert(new_ser.end(), serialized.begin(), serialized.end());
        new_ser.insert(new_ser.end(), fig.serialized.begin(), fig.serialized.end());
        return {"U(" + serialized_debug + ", " + fig.serialized_debug + ")",
                    new_ser, bldr};
    }

    Figure operator&(const Figure &fig) const {
        std::vector<int> new_ser = {'I'};
        new_ser.insert(new_ser.end(), serialized.begin(), serialized.end());
        new_ser.insert(new_ser.end(), fig.serialized.begin(), fig.serialized.end());
        return {"I(" + serialized_debug + ", " + fig.serialized_debug + ")",
                new_ser, bldr};
    }

    Figure operator-(const Figure &fig) const {

        std::vector<int> new_ser = {'S'};
        new_ser.insert(new_ser.end(), serialized.begin(), serialized.end());
        new_ser.insert(new_ser.end(), fig.serialized.begin(), fig.serialized.end());
        return {"S(" + serialized_debug + ", " + fig.serialized_debug + ")",
                new_ser, bldr};
    }

    Figure operator*(const mth::matr &matr) {
        std::vector<int> new_ser(serialized);
        BrickId matrId = bldr->addMatr(matr);
        new_ser.push_back(matrId.type);
        new_ser.push_back(matrId.ind);
        return {serialized_debug + "[" + matrId.type + std::to_string(matrId.ind) + "]",
                new_ser, bldr};
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


void rmShdUnit::initUnit() {
    Builder bldr;
    Material mtl;
    mtl.color[0] = 1;
    mtl.color[1] = 0;
    mtl.color[2] = 0;
    mtl.color[3] = 1;

    Figure box(bldr.createBox(1, mtl), &bldr);
    Figure sph(bldr.createSphere(1, mtl), &bldr);
    Figure upSph = sph * mth::matr::Translate(0, 0.5, 0);
    Figure diff = box - upSph;
    Figure diffWorld = diff * mth::matr::RotateY(45);
    std::cout << diffWorld.serialized_debug << std::endl;
    for (int i = 0; i < diffWorld.serialized.size(); i++) {
        std::cout << diffWorld.serialized[i] << " ";
    }
    std::cout << std::endl;
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
    auto unitPrimitive = render::renderInstance.addPrimitive(
        "rm", vertexBuffer, "v3", indexBuffer
    );
    frameH = new int{static_cast<int>(render::renderInstance.getWindowHeight())};
    frameW = new int{static_cast<int>(render::renderInstance.getWindowWidth())};
    unitPrimitive->shaderAddUniform1f(&render::renderInstance.getTime(), "time");
    unitPrimitive->shaderAddUniform1i(frameH, "frame_h");
    unitPrimitive->shaderAddUniform1i(frameW, "frame_w");
}  // End of 'testUnit::initUnit' function

//std::vector<uint> parseFigures(const std::string &str, )
void rmShdUnit::responseUnit() {
    float time = render::renderInstance.getTime();
    Builder bldr;
    Material mtl;
    mtl.color[0] = 1;
    mtl.color[1] = 0;
    mtl.color[2] = 0;
    mtl.color[3] = 1;

    Figure box(bldr.createBox(1, mtl), &bldr);
    Figure sph(bldr.createSphere(1, mtl), &bldr);
    Figure upSph = sph * mth::matr::Translate(0, sin(time), 0);
    Figure diff = box - upSph;
    Figure diffWorld = diff * mth::matr::RotateY(45);

    GLuint ssbo, ssbo2, ssbo3, ssbo4;
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(SphereBrick) * bldr.spheres.size(), bldr.spheres.data(), GL_DYNAMIC_COPY); // ??? GL_DYNAMIC_COPY mb GL_STATIC_DRAW
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind

    glGenBuffers(1, &ssbo2);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo2);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(BoxBrick) * bldr.boxes.size(), bldr.boxes.data(), GL_DYNAMIC_COPY); // ??? GL_DYNAMIC_COPY mb GL_STATIC_DRAW
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo2);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind

    glGenBuffers(1, &ssbo3);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo3);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(mth::matr) * bldr.matrices.size(), bldr.matrices.data(), GL_DYNAMIC_COPY); // ??? GL_DYNAMIC_COPY mb GL_STATIC_DRAW
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, ssbo3);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind

    glGenBuffers(1, &ssbo4);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo4);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * diffWorld.serialized.size(), diffWorld.serialized.data(), GL_DYNAMIC_COPY); // ??? GL_DYNAMIC_COPY mb GL_STATIC_DRAW
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