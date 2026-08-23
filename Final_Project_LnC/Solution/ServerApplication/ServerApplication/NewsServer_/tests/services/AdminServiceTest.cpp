/**
 * AdminServiceTest.cpp
 *
 * Tests AdminService (server-side) through the real Database layer
 * using an in-memory SQLite database.
 *
 * AdminService methods are thin wrappers around Database:: calls that
 * manage external servers, categories, and content moderation.
 */
#include <gtest/gtest.h>
#include "../../services/AdminService.hpp"
#include "../../database/DBManager.hpp"
#include "../../database/DataBase.hpp"
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

class AdminServiceTest : public ::testing::Test {
protected:
    static void SetUpTestSuite() {
        ASSERT_TRUE(DBManager::getInstance().initializeDB(":memory:"))
            << "Failed to open in-memory SQLite database for AdminServiceTest";
    }
    static void TearDownTestSuite() {
        DBManager::getInstance().close();
    }
};

// ─────────────────────────────────────────────────────────
// listExternalServers
// ─────────────────────────────────────────────────────────

TEST_F(AdminServiceTest, ListServers_ReturnsJsonArray) {
    auto servers = AdminService::listExternalServers();
    EXPECT_TRUE(servers.is_array())
        << "listExternalServers() must always return a JSON array";
}

TEST_F(AdminServiceTest, ListServers_ContainsSeededServers) {
    auto servers = AdminService::listExternalServers();
    // DBManager::executeSchema() seeds TheNewsAPI and NewsAPI.org
    EXPECT_GE(servers.size(), 2u) << "At least 2 seeded servers expected";
}

TEST_F(AdminServiceTest, ListServers_EachEntryHasRequiredFields) {
    auto servers = AdminService::listExternalServers();
    for (const auto& s : servers) {
        EXPECT_TRUE(s.contains("id"))          << "Server entry must have 'id'";
        EXPECT_TRUE(s.contains("server_name")) << "Server entry must have 'server_name'";
        EXPECT_TRUE(s.contains("api_key"))     << "Server entry must have 'api_key'";
        EXPECT_TRUE(s.contains("status"))      << "Server entry must have 'status'";
    }
}

// ─────────────────────────────────────────────────────────
// viewExternalServerDetails
// ─────────────────────────────────────────────────────────

TEST_F(AdminServiceTest, ViewServerDetails_ValidId_ReturnsNonEmpty) {
    auto servers = AdminService::listExternalServers();
    ASSERT_FALSE(servers.empty());
    int firstId = servers[0]["id"].get<int>();

    auto details = AdminService::viewExternalServerDetails(firstId);
    EXPECT_FALSE(details.empty()) << "Fetching details for an existing server must return data";
    EXPECT_EQ(details["id"], firstId);
}

TEST_F(AdminServiceTest, ViewServerDetails_InvalidId_ReturnsEmpty) {
    auto details = AdminService::viewExternalServerDetails(999999);
    EXPECT_TRUE(details.empty()) << "Fetching details for a non-existent id must return empty";
}

// ─────────────────────────────────────────────────────────
// updateExternalServerApiKey
// ─────────────────────────────────────────────────────────

TEST_F(AdminServiceTest, UpdateApiKey_ExistingServer_Succeeds) {
    auto servers = AdminService::listExternalServers();
    ASSERT_FALSE(servers.empty());
    int firstId = servers[0]["id"].get<int>();

    bool ok = AdminService::updateExternalServerApiKey(firstId, "new-api-key-xyz");
    EXPECT_TRUE(ok);

    // Verify the key was actually updated
    auto details = AdminService::viewExternalServerDetails(firstId);
    EXPECT_EQ(details["api_key"], "new-api-key-xyz");
}

// ─────────────────────────────────────────────────────────
// addCategory
// ─────────────────────────────────────────────────────────

TEST_F(AdminServiceTest, AddCategory_NewCategory_Succeeds) {
    bool ok = AdminService::addCategory("SportsAdminTest");
    EXPECT_TRUE(ok);
}

TEST_F(AdminServiceTest, AddCategory_DuplicateCategory_DoesNotCrash) {
    AdminService::addCategory("DupCategory");
    // Second insert should be silently ignored (INSERT OR IGNORE)
    EXPECT_NO_THROW(AdminService::addCategory("DupCategory"));
}

// ─────────────────────────────────────────────────────────
// hideCategory / unhideCategory / getHiddenCategories
// ─────────────────────────────────────────────────────────

TEST_F(AdminServiceTest, GetHiddenCategories_Initially_Empty) {
    auto hidden = AdminService::getHiddenCategories();
    EXPECT_TRUE(hidden.is_array());
    // No categories have been hidden yet in this suite
    EXPECT_TRUE(hidden.empty()) << "No hidden categories expected at suite start";
}

TEST_F(AdminServiceTest, HideCategory_ThenGetHidden_ShowsIt) {
    AdminService::addCategory("Financials");
    AdminService::hideCategory("Financials");

    auto hidden = AdminService::getHiddenCategories();
    bool found = false;
    for (const auto& c : hidden) {
        if (c.get<std::string>() == "Financials") { found = true; break; }
    }
    EXPECT_TRUE(found) << "Hidden category must appear in getHiddenCategories()";
}

TEST_F(AdminServiceTest, UnhideCategory_AfterHide_RemovedFromList) {
    AdminService::addCategory("TempCat");
    AdminService::hideCategory("TempCat");
    AdminService::unhideCategory("TempCat");

    auto hidden = AdminService::getHiddenCategories();
    bool found = false;
    for (const auto& c : hidden) {
        if (c.get<std::string>() == "TempCat") { found = true; break; }
    }
    EXPECT_FALSE(found) << "Unhidden category must no longer appear in hidden list";
}

// ─────────────────────────────────────────────────────────
// addFilteredKeyword / removeFilteredKeyword / getFilteredKeywords
// ─────────────────────────────────────────────────────────

TEST_F(AdminServiceTest, GetFilteredKeywords_Initially_Empty) {
    auto keywords = AdminService::getFilteredKeywords();
    EXPECT_TRUE(keywords.is_array());
    EXPECT_TRUE(keywords.empty()) << "No filtered keywords expected at suite start";
}

TEST_F(AdminServiceTest, AddFilteredKeyword_ThenGet_ShowsKeyword) {
    AdminService::addFilteredKeyword("spam");

    auto keywords = AdminService::getFilteredKeywords();
    bool found = false;
    for (const auto& k : keywords) {
        if (k.get<std::string>() == "spam") { found = true; break; }
    }
    EXPECT_TRUE(found) << "Added keyword must appear in getFilteredKeywords()";
}

TEST_F(AdminServiceTest, RemoveFilteredKeyword_AfterAdd_RemovedFromList) {
    AdminService::addFilteredKeyword("junk");
    AdminService::removeFilteredKeyword("junk");

    auto keywords = AdminService::getFilteredKeywords();
    bool found = false;
    for (const auto& k : keywords) {
        if (k.get<std::string>() == "junk") { found = true; break; }
    }
    EXPECT_FALSE(found) << "Removed keyword must no longer appear in list";
}

TEST_F(AdminServiceTest, FilteredKeyword_DuplicateAdd_StoredOnce) {
    AdminService::addFilteredKeyword("duplicate_kw");
    AdminService::addFilteredKeyword("duplicate_kw");  // INSERT OR IGNORE

    auto keywords = AdminService::getFilteredKeywords();
    int count = 0;
    for (const auto& k : keywords) {
        if (k.get<std::string>() == "duplicate_kw") ++count;
    }
    EXPECT_EQ(count, 1) << "Duplicate keyword must be stored only once";
}
