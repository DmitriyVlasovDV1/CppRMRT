#include "rm_shd_unit.hpp"

// Project namespace
namespace hse {
void rmShdUnit::initUnit() {
    ::std::vector<int> indexBuffer(3);
    ::std::vector<float> vertexBuffer = {0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
                                         -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
                                         0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f};

    for (int j = 0; j < 3; j++)
        indexBuffer[j] = j;
    render::renderInstance.primitiveAdd(
        "rm", vertexBuffer, "v3v3", indexBuffer
    );
}  // End of 'testUnit::initUnit' function

void rmShdUnit::responseUnit() {
    struct alignas(16) Sphere {
        struct alignas(16) Material {
            float color[4] = {1, 0, 1, 0};
        } mtl;
        float center[4] = {1, 1, 0.3, 0};
        float radius = 0.5;
    };
    Sphere data[2];
    data[1].center[0] = 0;

    GLuint ssbo;
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(data), data, GL_DYNAMIC_COPY); // ??? GL_DYNAMIC_COPY mb GL_STATIC_DRAW
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind
}  // End of 'testUnit::responseUnit' function

// Class override destructor
rmShdUnit::~rmShdUnit() {
    ::std::cout << "Clear unit" << ::std::endl;
}  // End of 'testUnit::~testUnit' function
}  // namespace hse