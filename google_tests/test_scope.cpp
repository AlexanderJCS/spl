#include <gtest/gtest.h>

#include "../spl.h"

TEST(IfScopeTest, IfScope) {
    env::Environment env = run("a = 0; if (true) { a = 1; }");
    env::VariantType result = env.get("a");

    ASSERT_EQ(std::get<int>(result), 1);
}

// todo: write more tests, especially for functions
