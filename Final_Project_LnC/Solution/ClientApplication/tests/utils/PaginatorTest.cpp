/**
 * PaginatorTest.cpp
 *
 * Tests Paginator<T> by redirecting std::cin so display() can run
 * without blocking on the terminal.
 *
 * Strategy: supply a pre-built input stream ('B' = Back to exit immediately,
 * 'N' = Next page, 'P' = Previous page) and count how many times the
 * user-supplied render function is called.
 */
#include <gtest/gtest.h>
#include "../../utils/Paginator.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

// ─── RAII helper: redirect std::cin for the duration of a scope ──────────────
struct CinRedirect {
    std::streambuf* orig;
    std::istringstream iss;
    explicit CinRedirect(const std::string& input)
        : iss(input), orig(std::cin.rdbuf(iss.rdbuf())) {}
    ~CinRedirect() { std::cin.rdbuf(orig); }
};

// ─── Helper: build a vector of N integers ────────────────────────────────────
static std::vector<int> makeItems(int count) {
    std::vector<int> v;
    for (int i = 0; i < count; ++i) v.push_back(i);
    return v;
}

// ─── Construction ────────────────────────────────────────────────────────────

TEST(PaginatorTest, Construct_ValidDataAndDefaultPageSize_DoesNotThrow) {
    auto items = makeItems(10);
    EXPECT_NO_THROW(Paginator<int> p(items));
}

TEST(PaginatorTest, Construct_EmptyVector_DoesNotThrow) {
    std::vector<std::string> empty;
    EXPECT_NO_THROW(Paginator<std::string> p(empty));
}

TEST(PaginatorTest, Construct_CustomPageSize_DoesNotThrow) {
    auto items = makeItems(20);
    EXPECT_NO_THROW(Paginator<int> p(items, 3));
}

// ─── Render-call count verification ─────────────────────────────────────────
// These tests inject 'B\n' (Back) so display() renders one page then exits.

TEST(PaginatorTest, Display_EmptyVector_NeverCallsRenderer) {
    std::vector<int> empty;
    Paginator<int> p(empty, 5);

    int callCount = 0;
    CinRedirect cin_in("B\n");
    // Suppress stdout from Paginator's own prints
    std::ostringstream oss;
    std::streambuf* origOut = std::cout.rdbuf(oss.rdbuf());
    p.display([&](const int&) { ++callCount; });
    std::cout.rdbuf(origOut);

    EXPECT_EQ(callCount, 0) << "Empty vector must not invoke the renderer";
}

TEST(PaginatorTest, Display_FewerItemsThanPageSize_RendersAllItems) {
    auto items = makeItems(3);
    Paginator<int> p(items, 10);   // pageSize=10, but only 3 items

    int callCount = 0;
    CinRedirect cin_in("B\n");
    std::ostringstream oss;
    std::streambuf* origOut = std::cout.rdbuf(oss.rdbuf());
    p.display([&](const int&) { ++callCount; });
    std::cout.rdbuf(origOut);

    EXPECT_EQ(callCount, 3) << "All 3 items should be rendered on a single page";
}

TEST(PaginatorTest, Display_FullPage_RendersExactlyPageSize) {
    auto items = makeItems(10);
    Paginator<int> p(items, 5);    // 2 pages of 5

    int callCount = 0;
    CinRedirect cin_in("B\n");     // Back immediately (first page only)
    std::ostringstream oss;
    std::streambuf* origOut = std::cout.rdbuf(oss.rdbuf());
    p.display([&](const int&) { ++callCount; });
    std::cout.rdbuf(origOut);

    EXPECT_EQ(callCount, 5) << "First page must render exactly pageSize items";
}

TEST(PaginatorTest, Display_NextPage_RendersBothPages) {
    auto items = makeItems(7);
    Paginator<int> p(items, 5);    // page 0: 5 items, page 1: 2 items

    int callCount = 0;
    CinRedirect cin_in("N\nB\n");  // Go to page 2, then Back
    std::ostringstream oss;
    std::streambuf* origOut = std::cout.rdbuf(oss.rdbuf());
    p.display([&](const int&) { ++callCount; });
    std::cout.rdbuf(origOut);

    EXPECT_EQ(callCount, 7) << "Both pages rendered: 5 + 2 = 7 items";
}

TEST(PaginatorTest, Display_PreviousAtFirstPage_StaysOnFirstPage) {
    auto items = makeItems(10);
    Paginator<int> p(items, 5);

    int callCount = 0;
    // 'P' at page 0 should not go negative; stays on page 0; then 'B' exits
    CinRedirect cin_in("P\nB\n");
    std::ostringstream oss;
    std::streambuf* origOut = std::cout.rdbuf(oss.rdbuf());
    p.display([&](const int&) { ++callCount; });
    std::cout.rdbuf(origOut);

    // Page 0 rendered twice (once initially, once after 'P' stays on same page)
    EXPECT_EQ(callCount, 10) << "Two renders of the 5-item first page = 10 items";
}

TEST(PaginatorTest, Display_RenderFunctionReceivesCorrectValues) {
    std::vector<int> items = {42, 99, 7};
    Paginator<int> p(items, 5);

    std::vector<int> seen;
    CinRedirect cin_in("B\n");
    std::ostringstream oss;
    std::streambuf* origOut = std::cout.rdbuf(oss.rdbuf());
    p.display([&](const int& v) { seen.push_back(v); });
    std::cout.rdbuf(origOut);

    ASSERT_EQ(seen.size(), 3u);
    EXPECT_EQ(seen[0], 42);
    EXPECT_EQ(seen[1], 99);
    EXPECT_EQ(seen[2], 7);
}
