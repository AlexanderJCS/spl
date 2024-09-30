#include <gtest/gtest.h>

// A simple test that will always pass
TEST(SimpleTest, AlwaysTrue) {
    ASSERT_EQ(5, 5);  // Checks if 5 equals 5
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
