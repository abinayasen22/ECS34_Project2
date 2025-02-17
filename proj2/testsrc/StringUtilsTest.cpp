#include <gtest/gtest.h>
#include "StringUtils.h"

TEST(StringUtilsTest, SliceTest){
    EXPECT_EQ(Slice("Hello, World!", 0, 5), "Hello");
    EXPECT_EQ(Slice("Hello, World!", 7), "Hello");
    EXPECT_EQ(Slice("Hello, World!", -3), "rld!");
}

TEST(StringUtilsTest, Capitalize){
    EXPECT_EQ(Capitalize("hello"), "Hello");
    EXPECT_EQ(Capitalize("WORLD"), "World");
    EXPECT_EQ(Capitalize("3"), "3");
}

TEST(StringUtilsTest, Upper){
    EXPECT_EQ(Upper("hello"), "HELLO");
    EXPECT_EQ(Upper("HELLO"), "HELLO");
}

TEST(StringUtilsTest, Lower){
    EXPECT_EQ(Upper("hello"), "hello");
    EXPECT_EQ(Upper("HELLO"), "hello");
}

TEST(StringUtilsTest, LStrip){
    EXPECT_EQ(LStrip("   hello"), "hello");
    EXPECT_EQ(LStrip("hello   "), "hello   ");
}

TEST(StringUtilsTest, RStrip){
    EXPECT_EQ(RStrip("hello  "), "hello");
    EXPECT_EQ(RStrip("   hello"), "   hello");
}

TEST(StringUtilsTest, Strip){
    EXPECT_EQ(Strip("  hello  "), "hello");
    EXPECT_EQ(Strip("   hello   "), "hello");
}

TEST(StringUtilsTest, Center){
    EXPECT_EQ(Center("hello", 8), " hello ");
    EXPECT_EQ(Center("hello", 3), "hello");
}

TEST(StringUtilsTest, LJust){
    EXPECT_EQ(LJust("hello", 10), "hello     ");
}

TEST(StringUtilsTest, RJust){
    EXPECT_EQ(RJust("hello", 10), "     hello");
}

TEST(StringUtilsTest, Replace){
    EXPECT_EQ(Replace("hello world", "hello", "hi"), "hi world");
}

TEST(StringUtilsTest, Split){
    EXPECT_EQ(Split("hello world", " "), "hello", "world");
}

TEST(StringUtilsTest, Join){
    EXPECT_EQ(Join(",", ("a", "b", "c")), "a,b,c");
}

TEST(StringUtilsTest, ExpandTabs){
    EXPECT_EQ(ExpandTabs("hello\tworld", 4), "hello   world");
}

TEST(StringUtilsTest, EditDistance){
    EXPECT_EQ(EditDistance("hello", "hello", false), 0);
}