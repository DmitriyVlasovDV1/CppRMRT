#ifndef MATH_TEST_UTILS
#define MATH_TEST_UTILS

#include <type_traits>
#include <vector>
#include <algorithm>
#include "../doctest/doctest.h"
#include "../src/utilities/math/math.hpp"

namespace math_test {

using Matr = math::matr4;
using Vec = math::vec3;

const float EPS = 0.001;

bool is_equal(const Matr &m, const std::vector<std::vector<float>> &model, float eps = 0) {
    REQUIRE(model.size() == 4);
    REQUIRE(model[0].size() == 4);
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            if (fabs(m.matrix[j][i] - model[j][i]) > eps) {
                return false;
            }
        }
    }
    return true;
}

bool is_equal(const Matr &m1, const Matr &m2, float eps = 0) {
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            if (fabs(m1.matrix[j][i] - m2.matrix[j][i]) > eps) {
                return false;
            }
        }
    }
    return true;
}

bool is_equal(const Vec &v1, const std::vector<float> &v2, float eps = 0) {
    REQUIRE(v2.size() == 3);
    REQUIRE(eps >= 0);

    return fabs(v1.x - v2[0]) <= eps && fabs(v1.y - v2[1]) <= eps && fabs(v1.z - v2[2]) <= eps;
}

bool is_equal(const Vec &v1, const Vec &v2, float eps=0) {
    return fabs(v1.x - v2.x) <= eps && fabs(v1.y - v2.y) <= eps && fabs(v1.z - v2.z) <= eps;
}


bool is_equal(float a, float b, float eps=0) {
    return fabs(a - b) <= eps;
}

std::vector<std::vector<float>> get_zeros() {
    return std::vector<std::vector<float>>(4, std::vector<float>(4, 0));
}

std::vector<std::vector<float>> get_identity() {
    std::vector<std::vector<float>> res(4, std::vector<float>(4, 0));
    for (int i = 0; i < 4; i++) {
        res[i][i] = 1;
    }
    return res;
}

std::vector<std::vector<float>> get_numbers(bool reversed = false) {
    std::vector<std::vector<float>> res(4, std::vector<float>(4, 0));
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            auto y = static_cast<float>(j);
            auto x = static_cast<float>(i);
            res[j][i] = (reversed ? 15 - y * 4 + x : y * 4 + x);
        }
    }
    return res;
}

auto apply = [](std::vector<std::vector<float>> &m, const auto &functor) {
    std::for_each(m.begin(), m.end(), [&](std::vector<float> &line) {
        std::for_each(line.begin(), line.end(), [&](float &el) { functor(el); });
    });
    return m;
};

}

#endif
