#include <gtest/gtest.h>

#include "../spl.h"

#include <variant>


void test(const std::string& input, const env::VariantType& expected) {
    env::Environment env = run("a = " + input + ";\n");
    env::VariantType result = env.get("a");

    ASSERT_EQ(result, expected) << "Input: " << input;
}


TEST(OperatorsTest, Addition) {
    test("3 + 4", 7);
    test("3 + 4.5", 7.5f);
}

TEST(OperatorsTest, Subtraction) {
    test("3 - 4", -1);
    test("3 - 4.5", -1.5f);
}

TEST(OperatorsTest, Multiplication) {
    test("3 * 4", 12);
    test("3 * 4.5", 13.5f);
    test("3.5 * 4", 14.0f);
}

TEST(OperatorsTest, Division) {
    test("3 / 4", 0);
    test("11 / 4", 2);

    test("3.5 / 4", 0.875f);
    test("11.5 / 4", 2.875f);
    test("2 / 4.0", 0.5f);
}

TEST(OperatorsTest, BooleanAnd) {
    test("true && false", false);
    test("true && true", true);
    test("false && false", false);
    test("false && true", false);
}

TEST(OperatorsTest, BooleanOr) {
    test("true || false", true);
    test("true || true", true);
    test("false || false", false);
    test("false || true", true);
}

TEST(OperatorsTest, BooleanNot) {
    test("!true;", false);
    test("!false;", true);
}

TEST(OperatorsTest, Equal) {
    test("3 == 4", false);
    test("3 == 3", true);

    test("3.5 == 3.5", true);
    test("3.5 == 3", false);

    test("true == false", false);
    test("true == true", true);
}

TEST(OperatorsTest, NotEqual) {
    test("3 != 4", true);
    test("3 != 3", false);

    test("3.5 != 3.5", false);
    test("3.5 != 3", true);

    test("true != false", true);
    test("true != true", false);
}

TEST(OperatorsTest, Modulus) {
    test("11 % 4", 3);
    test("10.5 % 4", 2.5f);
}

TEST(OperatorsTest, Greater) {
    test("3 > 4", false);
    test("3 > 3", false);
    test("3 > 2", true);

    test("3 > 3.5", false);
    test("3.5 > 3", true);
    test("3.5 > 3.5", false);
}

TEST(OperatorsTest, GreaterEqual) {
    test("3 >= 4", false);
    test("3 >= 3", true);
    test("3 >= 2", true);

    test("3 >= 3.5", false);
    test("3.5 >= 3", true);
    test("3.5 >= 3.5", true);
}

TEST(OperatorsTest, Less) {
    test("3 < 4", true);
    test("3 < 3", false);
    test("3 < 2", false);

    test("3 < 3.5", true);
    test("3.5 < 3", false);
    test("3.5 < 3.5", false);
}

TEST(OperatorsTest, LessEqual) {
    test("3 <= 4", true);
    test("3 <= 3", true);
    test("3 <= 2", false);

    test("3 <= 3.5", true);
    test("3.5 <= 3", false);
    test("3.5 <= 3.5", true);
}

TEST(OperatorsTest, Combined) {
    env::Environment env = run("b = (!(3 == 4) && true) || false;\n");
}
