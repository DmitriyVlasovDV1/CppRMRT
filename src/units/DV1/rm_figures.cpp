//
// Created by dvlasov on 02.03.23.
//

#include "rm_figures.hpp"

namespace hse {

    Figure & Figure::operator=(const Figure &other) {
        type = other.type;
        serialized = other.serialized;
        factory = other.factory;
        return *this;
    }

    std::vector<int> Figure::wrap(char left, const std::vector<int> &ser, char right) {
        std::vector<int> res(1, left);
        res.insert(res.end(), ser.begin(), ser.end());
        res.push_back(right);
        return res;
    }


    std::vector<int> Figure::concatenate(const Figure &other) {
        std::vector<int> res(serialized);
        res.push_back(',');
        res.insert(res.end(), other.serialized.begin(), other.serialized.end());
        return res;
    }

    std::vector<int> Figure::concatenateAssociative(Type target, const Figure &other) {
        std::vector<int> ser1;
        if (type == Type::BRICK || target != type) {
            ser1 = serialized;
        } else {
            ser1.insert(ser1.end(), serialized.begin() + 1, serialized.end() - 1);
        }
        std::vector<int> ser2;
        if (other.type == Type::BRICK || target != type) {
            ser2 = other.serialized;
        } else {
            ser2.insert(ser2.end(), other.serialized.begin() + 1, other.serialized.end() - 1);
        }
        ser1.insert(ser1.end(), ser2.begin(), ser2.end());
        return ser1;
    }

    // intersection figures operator
    Figure Figure::operator&(const Figure &other) {
        if (factory != other.factory) {
            throw std::invalid_argument("Figures from different fabrics");
        }

        Figure res(factory);
        res.serialized = wrap('I', concatenate(other), 'E');
        res.type = Type::INTER;
        return res;
    }

    // unite figures operator
    Figure Figure::operator|(const Figure &other) {
        if (factory != other.factory) {
            throw std::invalid_argument("Figures from different fabrics");
        }

        Figure res(factory);
        auto tmp = wrap('U', concatenate(other), 'E');
        res.serialized = tmp;
        res.type = Type::UNITE;
        return res;
    }

    // sub figures operator
    Figure Figure::operator-(const Figure &other) {
        if (factory != other.factory) {
            throw std::invalid_argument("Figures from different fabrics");
        }

        Figure res(factory);
        res.serialized = wrap('S', concatenate(other), 'E');
        res.type = Type::SUB;
        return res;
    }

    // sub figures operator
    Figure Figure::operator*(const math::matr4 &matr) {
        int id = factory->addMatr(matr.inverting());

        Figure res(factory);
        res.serialized.push_back('m');
        res.serialized.push_back(id);
        res.serialized.insert(res.serialized.end(), serialized.begin(), serialized.end());
        res.type = Type::BRICK;
        return res;
    }

    std::string Figure::parseExpr(int &pos) const {
        std::string res;
        std::string curMatr = "mat4(1)";

        while (pos < serialized.size() && serialized[pos] != ',' && serialized[pos] != 'E') {
            int ind;
            std::string expr1;
            std::string expr2;
            switch (serialized[pos]) {
                case 'm':
                    ind = serialized[++pos];
                    curMatr = "matrices_buffer.matrices[" + std::to_string(ind) + "] * " + curMatr;
                    break;
                case 's':
                    ind = serialized[++pos];
                    res += "SDF_sphere(pos, " + curMatr + ", sph)";
                    break;
                case 'b':
                    ind = serialized[++pos];
                    res += "SDF_box(pos, " + curMatr + ", box_buffer.boxes[" + std::to_string(ind) + "])";
                    break;
                case 'I':
                    expr1 = parseExpr(++pos);
                    expr2 = parseExpr(++pos);

                    res += "inter(" + expr1 + ", " + expr2 + ")";
                    break;
                case 'U':
                    expr1 = parseExpr(++pos);
                    expr2 = parseExpr(++pos);

                    res += "unite(" + expr1 + ", " + expr2 + ")";
                    break;
                case 'S':
                    expr1 = parseExpr(++pos);
                    expr2 = parseExpr(++pos);

                    res += "sub(" + expr1 + ", " + expr2 + ")";
                    break;
            }
            pos++;
        }
        return res;
    }

    std::string Figure::getSdfSceneSource() const {
        int pos = 0;
        std::string res =\
            "Surface SDF_scene(vec3 pos)\n"
            "{\n"
            "Surface res;\n"
            "Sphere sph;\n"
            "sph.radius = 0.1;"
            "res.sdf = max_dist;\n"
            "res = ";
        res += parseExpr(pos);
        res += ";\n"
            "return res;\n"
            "}\n";
        return res;
    }

    std::string Figure::getFragmentSource(const std::string &filepath) const {
        std::ifstream file(filepath);
        if (!file) {
            // TODO
        }

        std::string sourceLine;
        std::string source;
        while (std::getline(file, sourceLine)) {
            if (sourceLine == "#include SDF_scene") {
                source += getSdfSceneSource();
            } else {
                source += sourceLine + '\n';
            }
        }

        return source;
    }

    std::string Figure::getVertexSource(const std::string &filepath) const {
        std::ifstream file(filepath);
        if (!file) {
            // TODO
        }

        std::string sourceLine;
        std::string source;
        while (std::getline(file, sourceLine)) {
            source += sourceLine + '\n';
        }

        return source;
    }


    void Figure::draw(primitive *primitive) const {
        GLuint ssbo, ssbo2, ssbo3, ssbo4, ssbo5, ssbo6;
        glGenBuffers(1, &ssbo);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(SphereBrick) * factory->spheres.size(), factory->spheres.data(), GL_STATIC_DRAW); // ??? GL_DYNAMIC_COPY mb GL_STATIC_DRAW
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssbo);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

        glGenBuffers(1, &ssbo2);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo2);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(BoxBrick) * factory->boxes.size(), factory->boxes.data(), GL_STATIC_DRAW); // ??? GL_DYNAMIC_COPY mb GL_STATIC_DRAW
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo2);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

        glGenBuffers(1, &ssbo3);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo3);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(::math::matr4) * factory->matricies.size(), factory->matricies.data(), GL_DYNAMIC_COPY); // ??? GL_DYNAMIC_COPY mb GL_STATIC_DRAW
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, ssbo3);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

        glGenBuffers(1, &ssbo4);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo4);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * serialized.size(), serialized.data(), GL_STATIC_DRAW); // ??? GL_DYNAMIC_COPY mb GL_STATIC_DRAW
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, ssbo4);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

        // tmp
        BoxBrick data[1024];
        glGenBuffers(1, &ssbo5);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo5);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(BoxBrick) * 1024, data, GL_STATIC_DRAW); // ??? GL_DYNAMIC_COPY mb GL_STATIC_DRAW
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, ssbo5);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    int FigureFactory::addMatr(const math::matr4 &matr) {
        matricies.push_back(matr);
        return static_cast<int>(matricies.size()) - 1;
    }

    Figure FigureFactory::createSphere(float radius, Material mtl) {
        SphereBrick brick;
        brick.radius = radius;
        brick.mtl = mtl;
        spheres.push_back(brick);

        Figure res(this);
        res.serialized.push_back('s');
        res.serialized.push_back(static_cast<int>(spheres.size()) - 1);
        res.type = Figure::Type::BRICK;
        return res;
    }


    Figure FigureFactory::createBox(float radius, Material mtl) {
        BoxBrick brick;
        brick.radius = radius;
        brick.mtl = mtl;
        boxes.push_back(brick);

        Figure res(this);
        res.type = Figure::Type::BRICK;
        res.serialized.push_back('b');
        res.serialized.push_back(static_cast<int>(boxes.size()) - 1);
        return res;
    }


}
