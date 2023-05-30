#include "math_test_utils.hpp"

namespace math_test {


// TODO: Make move constructors
TEST_CASE("Rule of three") {
    CHECK(std::is_copy_constructible<Matr>());
    CHECK(std::is_copy_assignable<Matr>());
    CHECK(std::is_move_assignable<Matr>());
    CHECK(std::is_destructible<Matr>());
}

TEST_CASE("Test constructors") {
    SUBCASE("Common constructors") {
        Matr default_constructed;
        Matr constructed_from_elements{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
        float arr[4][4] {{0, 1, 2, 3}, {4, 5, 6, 7}, {8, 9, 10, 11}, {12, 13, 14, 15}};
        Matr constructed_from_array{arr};

        CHECK(is_equal(default_constructed, get_identity()));
        CHECK(is_equal(constructed_from_elements, get_numbers()));
        CHECK(is_equal(constructed_from_array, get_numbers()));
    }

    SUBCASE("Constructors from other matrix") {
        Matr constructed_from_elements{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

        Matr copied_constructed(constructed_from_elements);
        Matr copied_assigned = constructed_from_elements;

        CHECK(is_equal(copied_constructed, get_numbers()));
        CHECK(is_equal(copied_assigned, get_numbers()));
    }
}

TEST_CASE("Test operations") {
    SUBCASE("Multiplication") {
        Matr constructed_from_zeros{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        Matr constructed_from_elements{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
        Matr constructed_from_elements_rev{15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
        std::vector<std::vector<float>> nums_mul_rev {
            {34, 28, 22, 16},
            {178, 156, 134, 112},
            {322, 284, 246, 208},
            {466, 412, 358, 304}
        };

        CHECK(is_equal(constructed_from_zeros * constructed_from_elements, get_zeros()));
        CHECK(is_equal(constructed_from_elements * constructed_from_zeros, get_zeros()));
        CHECK(is_equal(constructed_from_elements * constructed_from_elements_rev, nums_mul_rev));
        CHECK(!is_equal(constructed_from_elements * constructed_from_elements_rev, constructed_from_elements_rev * constructed_from_elements));
    }

    SUBCASE("Self operations") {
        Matr constructed_from_zeros{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        Matr constructed_from_elements{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
        Matr constructed_from_elements_2{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
        auto numbers = get_numbers();

        constructed_from_elements_2 *= constructed_from_elements_2;
        CHECK(is_equal(constructed_from_elements_2, constructed_from_elements * constructed_from_elements));
        constructed_from_elements += constructed_from_elements;
        CHECK(is_equal(constructed_from_elements, apply(numbers, [](float &el) {el += el;})));
        constructed_from_elements += constructed_from_zeros;
        CHECK(is_equal(constructed_from_elements, apply(numbers, [](float &el) {})));
        constructed_from_elements -= constructed_from_elements;
        CHECK(is_equal(constructed_from_elements, apply(numbers, [](float &el) {el = 0;})));
        constructed_from_elements_2 *= constructed_from_elements_2;
    }

    SUBCASE("Addition") {
        Matr constructed_from_zeros{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        Matr constructed_from_elements{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
        Matr constructed_from_elements_rev{15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
        std::vector<std::vector<float>> nums_add_rev(4, std::vector<float>(4, 15));

        CHECK(is_equal(constructed_from_zeros + constructed_from_elements, get_numbers()));
        CHECK(is_equal(constructed_from_elements + constructed_from_zeros, get_numbers()));
        CHECK(is_equal(constructed_from_elements + constructed_from_elements_rev, nums_add_rev));
        CHECK(is_equal(constructed_from_elements_rev + constructed_from_elements, nums_add_rev));
    }

#ifndef MUTEBUGS
    SUBCASE("Subtraction") {
        Matr constructed_from_zeros{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        Matr constructed_from_elements{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
        Matr minus_constructed_from_elements = -constructed_from_elements;

        CHECK(is_equal(constructed_from_zeros - constructed_from_elements, minus_constructed_from_elements));
        CHECK(is_equal(constructed_from_elements - constructed_from_zeros, get_numbers()));
        CHECK(is_equal(constructed_from_elements + minus_constructed_from_elements, get_zeros()));
        CHECK(is_equal(-minus_constructed_from_elements, constructed_from_elements));
    }
#endif
}

TEST_CASE("Test methods") {
    SUBCASE("Determinant") {
        Matr identity;
        Matr constructed_from_zeros{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        Matr constructed_from_elements{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

        CHECK(identity.det() == 1);
        CHECK(constructed_from_zeros.det() == 0);
        CHECK((identity + constructed_from_elements).det() == -49);
    }

    SUBCASE("Inverse") {
        Matr identity;
        Matr constructed_from_zeros{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        Matr constructed_from_elements{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
        constructed_from_elements += identity;

        CHECK(is_equal(identity, identity.inverting()));
        CHECK(is_equal(constructed_from_zeros.inverting(), get_identity()));
        CHECK(is_equal(constructed_from_elements.inverting() * constructed_from_elements, get_identity(), EPS));
        CHECK(is_equal(identity, identity.inverse()));
        CHECK(is_equal(constructed_from_zeros.inverse(), get_identity()));
        constructed_from_elements.inverse();
        CHECK(is_equal(constructed_from_elements.inverting() * constructed_from_elements , get_identity(), EPS));
    }

    SUBCASE("Transpose") {
        Matr identity;
        Matr constructed_from_zeros{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        Matr constructed_from_elements{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
        std::vector<std::vector<float>> tr = get_numbers();
        for (int j = 0; j < 4; j++) {
            for (int i = 0; i < 4; i++) {
                std::swap(tr[i][j], tr[j][i]);
            }
        }

        CHECK(is_equal(identity.transpose(), get_identity()));
        CHECK(is_equal(constructed_from_zeros.transpose(), get_zeros()));
        CHECK(is_equal(constructed_from_elements.transpose(), tr));
    }

}

TEST_CASE("Test transformation matrices") {
    SUBCASE("Translate") {
        Matr tr0 = Matr::translate(Vec(0, 0, 0));
        Matr tr1 = Matr::translate(Vec(1, 2, 3));

        CHECK(is_equal(tr0, get_identity()));
        CHECK(is_equal(tr1.getPosition(), std::vector<float>{1, 2, 3}));
        CHECK(is_equal((tr1 * tr1).getPosition(), std::vector<float>{2, 4, 6}));
    }

    ///TODO make tests for scale and rotate and getView

}

}