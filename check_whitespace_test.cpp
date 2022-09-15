#include <gtest/gtest.h>

#include "check_whitespace.h"

TEST(strip, EmptyString) {
    const char* result = strip("");
    ASSERT_STREQ("", result);
    free((void*) result);
}

TEST(strip, NoWhitespace) {
    const char* result = strip("frog");
    ASSERT_STREQ("frog", result);
    free((void*) result);
}

TEST(strip, WhitespaceOnFront) {
    const char* result = strip("   frog");
    ASSERT_STREQ("frog", result);
    free((void*) result);
}

TEST(strip, WhitespaceOnBack) {
    const char* result = strip("frog  ");
    ASSERT_STREQ("frog", result);
    free((void*) result);
}

TEST(strip, WhitespaceOnBothEnds) {
    const char* result = strip("  frog     ");
    ASSERT_STREQ("frog", result);
    free((void*) result);
}

TEST(is_clean, EmptyString) {
    ASSERT_TRUE(is_clean(""));
}

TEST(is_clean, NoWhitespace) {
    ASSERT_TRUE(is_clean("University of Minnesota Morris"));
}

TEST(is_clean, WhitespaceOnFront) {
    ASSERT_FALSE(is_clean("   University of Minnesota Morris"));
}

TEST(is_clean, WhitespaceOnBack) {
    ASSERT_FALSE(is_clean("University of Minnesota Morris  "));
}

TEST(is_clean, WhitespaceOnBothEnds) {
    ASSERT_FALSE(is_clean(" University of Minnesota Morris"    ));
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
