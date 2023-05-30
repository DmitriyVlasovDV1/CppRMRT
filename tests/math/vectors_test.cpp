#include <type_traits>
#include <vector>
#include <assert.h>
#include <algorithm>
#include "../doctest/doctest.h"
#include "../src/utilities/math/math.hpp"

namespace vec_test {

#define MUTEBUGS /// TODO remove
    using Matr = math::matr4;
    using Vec = math::vec3;

    const float EPS = 0.001;

    bool is_equal(const Matr &m, const std::vector<std::vector<float>> &model, float eps=0) {
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
    bool is_equal(const Matr &m1, const Matr &m2, float eps=0) {
        for (int j = 0; j < 4; j++) {
            for (int i = 0; i < 4; i++) {
                if (fabs(m1.matrix[j][i] - m2.matrix[j][i]) > eps) {
                    return false;
                }
            }
        }
        return true;
    }

    bool is_equal(const Vec &v1, const std::vector<float> &v2, float eps=0) {
        REQUIRE(v2.size() == 3);
        REQUIRE(eps >= 0);

        return fabs(v1.x - v2[0]) <= eps &&
               fabs(v1.y - v2[1]) <= eps &&
               fabs(v1.z - v2[2]) <= eps;
    }
    std::vector<std::vector<float>> get_zeros() {
        return std::vector<std::vector<float>> (4, std::vector<float>(4, 0));
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

// TODO: Make move constructors
    TEST_CASE("Rule of three") {
        CHECK(std::is_copy_constructible<Vec>());
        CHECK(std::is_copy_assignable<Vec>());
        CHECK(std::is_move_assignable<Vec>());
        CHECK(std::is_destructible<Vec>());
    }

    TEST_CASE("Test constructors") {
        SUBCASE("Common constructors") {
            Vec default_constructed;
            Vec constructed_from_elements{0, 1, 2};
            Vec constructed_from_one_element{1};

            CHECK(is_equal(default_constructed, std::vector<float>{0, 0, 0}));
            CHECK(is_equal(constructed_from_elements, std::vector<float>{0, 1, 2}));
            CHECK(is_equal(constructed_from_one_element, std::vector<float>{1, 1, 1}));
        }

        SUBCASE("Constructors from other vector") {
            Vec constructed_from_elements{0, 1, 2};
            Vec copied_constructed(constructed_from_elements);
            Vec copied_assigned = constructed_from_elements;

            CHECK(is_equal(copied_constructed, std::vector<float>{0, 1, 2}));
            CHECK(is_equal(copied_assigned, std::vector<float>{0, 1, 2}));
        }
    }

    TEST_CASE("Test operations") {
        SUBCASE("Multiplication") {
            Vec constructed_from_zeros;
            Vec constructed_from_elements{0, 1, 2};

            CHECK(is_equal(constructed_from_zeros * 2.f, std::vector<float>{0, 0, 0}));
            CHECK(is_equal(constructed_from_elements * 2.f, std::vector<float>{0, 2, 4}));
            constructed_from_elements *= 2.f;
            CHECK(is_equal(constructed_from_elements, std::vector<float>{0, 2, 4}));
        }

        SUBCASE("Addition") {
            Vec constructed_from_zeros;
            Vec constructed_from_elements{0, 1, 2};
            Vec constructed_from_elements_rev{2, 1, 0};

            CHECK(is_equal(constructed_from_zeros + constructed_from_elements, std::vector<float>{0, 1, 2}));
            CHECK(is_equal(constructed_from_elements + constructed_from_zeros, std::vector<float>{0, 1, 2}));
            CHECK(is_equal(constructed_from_elements + constructed_from_elements_rev, std::vector<float>{2, 2, 2}));
            CHECK(is_equal(constructed_from_elements_rev + constructed_from_elements, std::vector<float>{2, 2, 2}));
        }


        /// TODO dot, cross
    }

    /// TODO methods

}