#include <gtest/gtest.h>
#include <tuple>

#include "AlgebraicSystem.hpp"

auto get_s_and_matrix() -> std::tuple<std::vector<int>, algebraic_system::matrix<int>> {
    std::vector s{1, 2, 3};
    algebraic_system::matrix<int> matrix{s.size(), s.size()};
    matrix[0][0] = 1; matrix[0][1] = 2; matrix[0][2] = 3;
    matrix[1][0] = 2; matrix[1][1] = 1; matrix[1][2] = 3;
    matrix[2][0] = 3; matrix[2][1] = 3; matrix[2][2] = 3;

    return std::make_tuple(s, matrix);
}

TEST(is_closure_test, is_closure) {
    auto [s, matrix] = get_s_and_matrix();

    ASSERT_TRUE(algebraic_system::is_closure(s, matrix));
}

TEST(is_commutable_test, is_commutable) {
    auto [s, matrix] = get_s_and_matrix();

    ASSERT_TRUE(algebraic_system::is_commutable(s, matrix));
}

TEST(is_associative_test, is_associative) {
    auto [s, matrix] = get_s_and_matrix();

    ASSERT_FALSE(algebraic_system::is_associative(s, matrix));
}

TEST(is_idempotent_test, is_idempotent) {
    auto [s, matrix] = get_s_and_matrix();

    ASSERT_FALSE(algebraic_system::is_idempotent(s, matrix));
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}