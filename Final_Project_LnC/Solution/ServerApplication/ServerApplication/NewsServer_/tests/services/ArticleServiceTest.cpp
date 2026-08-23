/**
 * ArticleServiceTest.cpp
 *
 * Tests ArticleService static methods against an in-memory SQLite database.
 * This is an integration test of the service + database layers together,
 * which is the correct approach for a thin service layer that delegates
 * directly to static Database:: calls.
 */

#include <gtest/gtest.h>
#include "../../services/ArticleService.hpp"
#include "../../database/DBManager.hpp"
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

// ---------------------------------------------------------------------------
// Test fixture — initialises a fresh in-memory SQLite DB for the entire suite
// ---------------------------------------------------------------------------
class ArticleServiceTest : public ::testing::Test {
protected:
    static void SetUpTestSuite() {
        ASSERT_TRUE(DBManager::getInstance().initializeDB(":memory:"))
            << "Failed to open in-memory SQLite database";
    }

    static void TearDownTestSuite() {
        DBManager::getInstance().close();
    }

    // Helper: build a minimal valid article JSON for storeArticle()
    static json makeArticle(const std::string& title,
                             const std::string& url,
                             const std::string& category = "Technology") {
        return {
            {"uuid",        ""},
            {"title",       title},
            {"description", "A test description for " + title},
            {"category",    category},
            {"language",    "en"},
            {"locale",      "us"},
            {"url",         url},
            {"image_url",   ""},
            {"content",     "Full content about " + title},
            {"source",      "TestSource"}
        };
    }
};

// ---------------------------------------------------------------------------
// getAllArticles
// ---------------------------------------------------------------------------
TEST_F(ArticleServiceTest, GetAllArticles_EmptyDatabase_ReturnsArray) {
    auto articles = ArticleService::getAllArticles();
    EXPECT_TRUE(articles.is_array());
}

TEST_F(ArticleServiceTest, GetAllArticles_AfterStore_ReturnsArticle) {
    ArticleService::storeArticle(makeArticle("Headline Alpha", "https://example.com/alpha"));
    auto articles = ArticleService::getAllArticles();
    ASSERT_FALSE(articles.empty());

    bool found = false;
    for (const auto& a : articles) {
        if (a.value("title", "") == "Headline Alpha") { found = true; break; }
    }
    EXPECT_TRUE(found);
}

// ---------------------------------------------------------------------------
// getArticleById
// ---------------------------------------------------------------------------
TEST_F(ArticleServiceTest, GetArticleById_NonExistent_ReturnsEmpty) {
    auto article = ArticleService::getArticleById(999999);
    EXPECT_TRUE(article.empty() || article.is_null());
}

// ---------------------------------------------------------------------------
// getArticlesByCategory
// ---------------------------------------------------------------------------
TEST_F(ArticleServiceTest, GetArticlesByCategory_MatchingCategory_ReturnsResults) {
    ArticleService::storeArticle(makeArticle("Sports Story", "https://example.com/sports", "Sports"));
    auto results = ArticleService::getArticlesByCategory("Sports");
    EXPECT_TRUE(results.is_array());
}

TEST_F(ArticleServiceTest, GetArticlesByCategory_NoMatch_ReturnsEmpty) {
    auto results = ArticleService::getArticlesByCategory("NonExistentXYZ");
    EXPECT_TRUE(results.is_array());
    EXPECT_TRUE(results.empty());
}

// ---------------------------------------------------------------------------
// storeArticle — deduplication by URL
// ---------------------------------------------------------------------------
TEST_F(ArticleServiceTest, StoreArticle_DuplicateUrl_NotInsertedTwice) {
    const std::string url = "https://example.com/dup-" + std::to_string(rand());
    ArticleService::storeArticle(makeArticle("Dup Article A", url));
    ArticleService::storeArticle(makeArticle("Dup Article B", url)); // same URL

    auto all = ArticleService::getAllArticles();
    int count = 0;
    for (const auto& a : all) {
        if (a.value("url", "") == url) ++count;
    }
    EXPECT_EQ(count, 1) << "Duplicate URL should be stored only once";
}

// ---------------------------------------------------------------------------
// getAllCategories
// ---------------------------------------------------------------------------
TEST_F(ArticleServiceTest, GetAllCategories_ReturnsArray) {
    auto categories = ArticleService::getAllCategories();
    EXPECT_TRUE(categories.is_array());
}

// ---------------------------------------------------------------------------
// searchArticles
// ---------------------------------------------------------------------------
TEST_F(ArticleServiceTest, SearchArticles_EmptyKeyword_ReturnsArray) {
    auto results = ArticleService::searchArticles("", "", "", "");
    EXPECT_TRUE(results.is_array());
}

TEST_F(ArticleServiceTest, SearchArticles_KnownKeyword_ReturnsMatch) {
    ArticleService::storeArticle(
        makeArticle("Quantum Physics Breakthrough", "https://example.com/quantum", "Science"));

    auto results = ArticleService::searchArticles("Quantum", "", "", "");
    EXPECT_TRUE(results.is_array());

    bool found = false;
    for (const auto& a : results) {
        if (a.value("title", "").find("Quantum") != std::string::npos) { found = true; break; }
    }
    EXPECT_TRUE(found);
}

TEST_F(ArticleServiceTest, SearchArticles_NoMatch_ReturnsEmpty) {
    auto results = ArticleService::searchArticles("ZZZNoMatchXXX123", "", "", "");
    EXPECT_TRUE(results.is_array());
    EXPECT_TRUE(results.empty());
}

// ---------------------------------------------------------------------------
// reportArticle / hideArticle / unhideArticle — no crash on valid input
// ---------------------------------------------------------------------------
TEST_F(ArticleServiceTest, ReportArticle_DoesNotThrow) {
    EXPECT_NO_THROW(ArticleService::reportArticle(1, 1, "spam"));
}

TEST_F(ArticleServiceTest, HideUnhideArticle_DoesNotThrow) {
    ArticleService::storeArticle(makeArticle("Article to Hide", "https://example.com/hide"));
    auto all = ArticleService::getAllArticles();
    if (!all.empty()) {
        int id = all[0].value("id", -1);
        if (id != -1) {
            EXPECT_NO_THROW(ArticleService::hideArticle(id));
            EXPECT_NO_THROW(ArticleService::unhideArticle(id));
        }
    }
}

// ---------------------------------------------------------------------------
// getReportedArticles
// ---------------------------------------------------------------------------
TEST_F(ArticleServiceTest, GetReportedArticles_ReturnsArray) {
    auto reports = ArticleService::getReportedArticles();
    EXPECT_TRUE(reports.is_array());
}

