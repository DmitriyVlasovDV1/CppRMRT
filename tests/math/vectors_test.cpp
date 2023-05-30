#include "math_test_utils.hpp"

namespace math_test {
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

        SUBCASE("Scalar multiplication") {
            Vec v1{1, 2, 4};
            Vec v2{4, 0, 9};
            Vec v3;
            Vec v4;

            CHECK((v1 & v2) == 1 * 4 + 4 * 9);
            CHECK((v3 & v4) == 0);
        }

        SUBCASE("Cross multiplication") {
            Vec forward{1, 0, 0};
            Vec right{0, 0, 1};
            Vec up{0, 1, 0};

            CHECK(is_equal((right % forward), up));
        }

    }

    TEST_CASE("Test methods") {
        SUBCASE("Length") {
            Vec v0;
            Vec v1{2, 6, 9};
            Vec v2{34, 21, 89};

            CHECK((!v0) == 0);
            CHECK((!v1) == 11);
            CHECK(is_equal((!v2), 97.560238, EPS));
        }

        SUBCASE("Normalize") {
            Vec v1{2, 6, 9};
            Vec v2{34, 21, 89};

            float len1 = !v1;
            float len2 = !v2;
            CHECK(is_equal(!(v1.normalizing()), 1, EPS));
            CHECK(is_equal(!(v2.normalizing()), 1, EPS));
            CHECK(is_equal(v1.normalizing() * len1, v1, EPS));
            CHECK(is_equal(v2.normalizing() * len2, v2, EPS));
            v1.normalize();
            v2.normalize();
            CHECK(is_equal(!v1, 1, EPS));
            CHECK(is_equal(!v2, 1, EPS));
            Vec vtmp = v1 * len1;
            CHECK(is_equal(v1 * len1, std::vector<float>{2, 6, 9}, EPS));
            CHECK(is_equal(v2 * len2, std::vector<float>{34, 21, 89}, EPS));
        }

        SUBCASE("Angle between") {
            Vec v1{0, 1, 0};
            Vec v2{1, 0, 0};

            CHECK(is_equal(math::vec3::getAngleBetween(v1, v2), math::PI / 2, EPS));
        }

        SUBCASE("Max and min") {
            Vec v1{2, -3, -3};
            Vec v2{1, -3, -2};

            CHECK(is_equal(math::vec3::min(v1, v2), std::vector<float>{1, -3, -3}));
            CHECK(is_equal(math::vec3::max(v1, v2), std::vector<float>{2, -3, -2}));

        }

    }

}