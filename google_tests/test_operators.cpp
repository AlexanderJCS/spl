#include <gtest/gtest.h>

#include "../spl.h"

#include <variant>


void test(const std::string& input, env::VariantType expected) {
    env::Environment env = run("a = " + input + ";\n");
    env::VariantType result = env.get("a");

    ASSERT_EQ(result, expected);
}


TEST(AdditionTest, Integers) {
    test("3 + 4", 7);
}

TEST(SubtractionTest, Integers) {
    test("3 - 4", -1);
}

TEST(MultiplicationTest, Integers) {
    test("3 * 4", 12);
}

TEST(DivisionTest, Integers) {
    test("3 / 4", 0);
    test("11 / 4", 2);
}

TEST(AndTest, Booleans) {
    test("true && false", false);
    test("true && true", true);
    test("false && false", false);
    test("false && true", false);
}

TEST(OrTest, Booleans) {
    test("true || false", true);
    test("true || true", true);
    test("false || false", false);
    test("false || true", true);
}

TEST(NotTest, Booleans) {
    test("!true;", false);
    test("!false;", true);
}

TEST(CombinedTest, AlwaysTrue) {
    env::Environment env = run("b = (!(3 == 4) && true) || false;\n");
}
