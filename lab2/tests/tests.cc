#include <gtest\gtest.h>

#include "../src/Seven.h"

TEST(equals_test_suite, equals_to_itself) {
    Seven lhs{50};
    ASSERT_TRUE(lhs.equals(lhs));
}

TEST(equals_test_suite, different_equals_objects) {
    Seven lhs{100};
    Seven rhs{100};
    ASSERT_TRUE(lhs.equals(rhs));
    ASSERT_TRUE(rhs.equals(lhs));
}

TEST(equals_test_suite, no_args_is_zero) {
    Seven lhs{};
    Seven rhs{0};
    ASSERT_TRUE(lhs.equals(rhs));
    ASSERT_TRUE(rhs.equals(lhs));
}

TEST(equals_test_suite, no_equals) {
    Seven lhs{100};
    Seven rhs{200};

    ASSERT_FALSE(lhs.equals(rhs));
    ASSERT_FALSE(rhs.equals(lhs));
}

TEST(equals_test_suite, different_container_sizes) {
    Seven a{7};
    Seven b{7};
    ASSERT_TRUE(a.equals(b));
}

TEST(comparison_test_suite, larger) {
    Seven small{100};
    Seven large{200};

    ASSERT_TRUE(large.greater_than(small));
    ASSERT_FALSE(small.greater_than(large));
}

TEST(comparison_test_suite, smaller) {
    Seven small{100};
    Seven large{200};

    ASSERT_TRUE(small.less_than(large));
    ASSERT_FALSE(large.less_than(small));
}

TEST(comparison_test_suite, equals_is_not_larger) {
    Seven value{100};
    ASSERT_FALSE(value.greater_than(value));
}

TEST(comparison_test_suite, equals_is_not_smaller) {
    Seven value{100};
    ASSERT_FALSE(value.less_than(value));
}

TEST(addition_test_suite, normal_test) {
    Seven left{10}, right{20}, expected{30};
    Seven result = left.add(right);
    ASSERT_TRUE(result.equals(expected));
}

TEST(addition_test_suite, different_ct_sizes) {
    Seven left{10}, right{20}, expected{30};
    Seven result = left.add(right);
    ASSERT_TRUE(result.equals(expected));
}

TEST(subtraction_test_suite, normal_test) {
    Seven left{30}, right{20}, expected{10};
    Seven result = left.substract(right);
    ASSERT_TRUE(result.equals(expected));
}

TEST(subtraction_test_suite, throw_on_underflow) {
    Seven left{10}, right{20};
    EXPECT_THROW(left.substract(right), std::underflow_error);
}

TEST(subtraction_test_suite, different_ct_sizes) {
    Seven left{10}, right{10}, expected{};
    Seven result = left.substract(right);
    EXPECT_TRUE(result.equals(expected));
}

TEST(copy_test_suite, copy_to_new) {
    Seven value{10};
    Seven copy{value};
    ASSERT_TRUE(copy.equals(value));
}

TEST(copy_test_suite, copy_to_existing) {
    Seven left{10};
    Seven right{1};
    left.copy(right);
    ASSERT_TRUE(left.equals(right));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}