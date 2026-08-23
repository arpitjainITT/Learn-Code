#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "database/DBManager.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <memory>
#include <sqlite3.h>

// Forward declaration for SQLite mock
class SQLite3Mock;

// Mock for SQLite3 operations
class SQLite3Mock {
public:
    MOCK_METHOD(int, open, (const char* filename, sqlite3** ppDb));
    MOCK_METHOD(int, close, (sqlite3* db));
    MOCK_METHOD(int, exec, (sqlite3* db, const char* sql, int (*callback)(void*,int,char**,char**), void* data, char** errmsg));
    MOCK_METHOD(int, prepare_v2, (sqlite3* db, const char* zSql, int nByte, sqlite3_stmt** ppStmt, const char** pzTail));
    MOCK_METHOD(int, step, (sqlite3_stmt* stmt));
    MOCK_METHOD(int, finalize, (sqlite3_stmt* stmt));
    MOCK_METHOD(int, column_int, (sqlite3_stmt* stmt, int iCol));
    MOCK_METHOD(const unsigned char*, column_text, (sqlite3_stmt* stmt, int iCol));
    MOCK_METHOD(int, bind_int, (sqlite3_stmt* stmt, int idx, int value));
    MOCK_METHOD(int, bind_text, (sqlite3_stmt* stmt, int idx, const char* text, int n, void(*)(void*)));
    MOCK_METHOD(const char*, errmsg, (sqlite3* db));
};

// Global instance of SQLite3Mock that can be accessed by DBManager
SQLite3Mock* g_sqliteMock = nullptr;

// Test fixture for DBManager
class DBManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create SQLite mock
        sqliteMock = std::make_unique<testing::NiceMock<SQLite3Mock>>();
        g_sqliteMock = sqliteMock.get();
        
        // Create DBManager with mock
        dbManager = std::make_unique<DBManager>("test.db");
    }

    void TearDown() override {
        // Clean up
        dbManager.reset();
        sqliteMock.reset();
        g_sqliteMock = nullptr;
    }

    // Helper to set up basic database operations
    void setupBasicDatabaseOperations() {
        // Mock successful database connection
        EXPECT_CALL(*sqliteMock, open(testing::_, testing::_))
            .WillOnce(testing::DoAll(
                testing::SetArgPointee<1>(reinterpret_cast<sqlite3*>(1)),
                testing::Return(SQLITE_OK)
            ));
        
        // Mock successful query execution
        EXPECT_CALL(*sqliteMock, exec(testing::_, testing::_, testing::_, testing::_, testing::_))
            .WillRepeatedly(testing::Return(SQLITE_OK));
        
        // Mock successful statement preparation
        EXPECT_CALL(*sqliteMock, prepare_v2(testing::_, testing::_, testing::_, testing::_, testing::_))
            .WillRepeatedly(testing::DoAll(
                testing::SetArgPointee<3>(reinterpret_cast<sqlite3_stmt*>(1)),
                testing::Return(SQLITE_OK)
            ));
        
        // Mock successful statement execution
        EXPECT_CALL(*sqliteMock, step(testing::_))
            .WillRepeatedly(testing::Return(SQLITE_DONE));
        
        // Mock successful statement finalization
        EXPECT_CALL(*sqliteMock, finalize(testing::_))
            .WillRepeatedly(testing::Return(SQLITE_OK));
    }

    // Helper to create sample article data
    nlohmann::json createSampleArticle() {
        return {
            {"title", "Test Article Title"},
            {"content", "This is the content of the test article."},
            {"url", "https://example.com/article"},
            {"source", "Test Source"},
            {"published_at", "2023-07-05T10:30:00Z"},
            {"category", "Technology"},
            {"author", "Test Author"}
        };
    }

    // Helper to create sample user data
    nlohmann::json createSampleUser() {
        return {
            {"username", "testuser"},
            {"email", "test@example.com"},
            {"password_hash", "hashed_password"},
            {"role", "user"}
        };
    }

    // Test objects
    std::unique_ptr<SQLite3Mock> sqliteMock;
    std::unique_ptr<DBManager> dbManager;
};

// Test initialization
TEST_F(DBManagerTest, Initialization) {
    // Mock successful database connection
    EXPECT_CALL(*sqliteMock, open(testing::_, testing::_))
        .WillOnce(testing::DoAll(
            testing::SetArgPointee<1>(reinterpret_cast<sqlite3*>(1)),
            testing::Return(SQLITE_OK)
        ));
    
    // Mock successful table creation
    EXPECT_CALL(*sqliteMock, exec(testing::_, testing::StrNe(""), testing::_, testing::_, testing::_))
        .Times(testing::AtLeast(1))
        .WillRepeatedly(testing::Return(SQLITE_OK));
    
    // Initialize database
    bool success = dbManager->initialize();
    
    // Verify initialization succeeded
    EXPECT_TRUE(success);
}

// Test initialization failure
TEST_F(DBManagerTest, InitializationFailure) {
    // Mock failed database connection
    EXPECT_CALL(*sqliteMock, open(testing::_, testing::_))
        .WillOnce(testing::Return(SQLITE_ERROR));
    
    // Mock error message
    EXPECT_CALL(*sqliteMock, errmsg(testing::_))
        .WillOnce(testing::Return("Failed to open database"));
    
    // Initialize database
    bool success = dbManager->initialize();
    
    // Verify initialization failed
    EXPECT_FALSE(success);
}

// Test storing an article
TEST_F(DBManagerTest, StoreArticle) {
    // Setup basic database operations
    setupBasicDatabaseOperations();
    
    // Create sample article
    auto article = createSampleArticle();
    
    // Expect specific SQL for article storage
    EXPECT_CALL(*sqliteMock, prepare_v2(testing::_, testing::HasSubstr("INSERT INTO articles"), testing::_, testing::_, testing::_))
        .WillOnce(testing::DoAll(
            testing::SetArgPointee<3>(reinterpret_cast<sqlite3_stmt*>(1)),
            testing::Return(SQLITE_OK)
        ));
    
    // Expect binding of values
    EXPECT_CALL(*sqliteMock, bind_text(testing::_, testing::_, testing::_, testing::_, testing::_))
        .Times(testing::AtLeast(5))
        .WillRepeatedly(testing::Return(SQLITE_OK));
    
    // Store article
    bool success = dbManager->storeArticle(article);
    
    // Verify storage succeeded
    EXPECT_TRUE(success);
}

// Test retrieving articles
TEST_F(DBManagerTest, GetArticles) {
    // Setup basic database operations
    setupBasicDatabaseOperations();
    
    // Mock step returning rows and then done
    EXPECT_CALL(*sqliteMock, step(testing::_))
        .WillOnce(testing::Return(SQLITE_ROW))
        .WillOnce(testing::Return(SQLITE_ROW))
        .WillOnce(testing::Return(SQLITE_DONE));
    
    // Mock column values
    EXPECT_CALL(*sqliteMock, column_int(testing::_, 0))
        .WillRepeatedly(testing::Return(1));
    
    EXPECT_CALL(*sqliteMock, column_text(testing::_, testing::_))
        .WillRepeatedly(testing::Return(reinterpret_cast<const unsigned char*>("Test Value")));
    
    // Get articles
    auto articles = dbManager->getArticles(10, 0, "");
    
    // Verify we got 2 articles
    EXPECT_EQ(articles.size(), 2);
}

// Test retrieving articles by category
TEST_F(DBManagerTest, GetArticlesByCategory) {
    // Setup basic database operations
    setupBasicDatabaseOperations();
    
    // Expect specific SQL for category filtering
    EXPECT_CALL(*sqliteMock, prepare_v2(testing::_, testing::HasSubstr("WHERE category = ?"), testing::_, testing::_, testing::_))
        .WillOnce(testing::DoAll(
            testing::SetArgPointee<3>(reinterpret_cast<sqlite3_stmt*>(1)),
            testing::Return(SQLITE_OK)
        ));
    
    // Expect binding of category
    EXPECT_CALL(*sqliteMock, bind_text(testing::_, 1, testing::StrEq("Technology"), testing::_, testing::_))
        .WillOnce(testing::Return(SQLITE_OK));
    
    // Mock step returning rows and then done
    EXPECT_CALL(*sqliteMock, step(testing::_))
        .WillOnce(testing::Return(SQLITE_ROW))
        .WillOnce(testing::Return(SQLITE_DONE));
    
    // Mock column values
    EXPECT_CALL(*sqliteMock, column_int(testing::_, 0))
        .WillRepeatedly(testing::Return(1));
    
    EXPECT_CALL(*sqliteMock, column_text(testing::_, testing::_))
        .WillRepeatedly(testing::Return(reinterpret_cast<const unsigned char*>("Test Value")));
    
    // Get articles by category
    auto articles = dbManager->getArticlesByCategory("Technology", 10, 0);
    
    // Verify we got 1 article
    EXPECT_EQ(articles.size(), 1);
}

// Test searching articles
TEST_F(DBManagerTest, SearchArticles) {
    // Setup basic database operations
    setupBasicDatabaseOperations();
    
    // Expect specific SQL for search
    EXPECT_CALL(*sqliteMock, prepare_v2(testing::_, testing::HasSubstr("LIKE ?"), testing::_, testing::_, testing::_))
        .WillOnce(testing::DoAll(
            testing::SetArgPointee<3>(reinterpret_cast<sqlite3_stmt*>(1)),
            testing::Return(SQLITE_OK)
        ));
    
    // Expect binding of search term
    EXPECT_CALL(*sqliteMock, bind_text(testing::_, 1, testing::HasSubstr("%test%"), testing::_, testing::_))
        .WillOnce(testing::Return(SQLITE_OK));
    
    // Mock step returning rows and then done
    EXPECT_CALL(*sqliteMock, step(testing::_))
        .WillOnce(testing::Return(SQLITE_ROW))
        .WillOnce(testing::Return(SQLITE_DONE));
    
    // Mock column values
    EXPECT_CALL(*sqliteMock, column_int(testing::_, 0))
        .WillRepeatedly(testing::Return(1));
    
    EXPECT_CALL(*sqliteMock, column_text(testing::_, testing::_))
        .WillRepeatedly(testing::Return(reinterpret_cast<const unsigned char*>("Test Value")));
    
    // Search articles
    auto articles = dbManager->searchArticles("test", 10, 0);
    
    // Verify we got 1 article
    EXPECT_EQ(articles.size(), 1);
}

// Test user operations
TEST_F(DBManagerTest, UserOperations) {
    // Setup basic database operations
    setupBasicDatabaseOperations();
    
    // Create sample user
    auto user = createSampleUser();
    
    // Mock user creation
    EXPECT_CALL(*sqliteMock, prepare_v2(testing::_, testing::HasSubstr("INSERT INTO users"), testing::_, testing::_, testing::_))
        .WillOnce(testing::DoAll(
            testing::SetArgPointee<3>(reinterpret_cast<sqlite3_stmt*>(1)),
            testing::Return(SQLITE_OK)
        ));
    
    // Create user
    bool createSuccess = dbManager->createUser(user);
    EXPECT_TRUE(createSuccess);
    
    // Mock user retrieval
    EXPECT_CALL(*sqliteMock, prepare_v2(testing::_, testing::HasSubstr("SELECT * FROM users WHERE id = ?"), testing::_, testing::_, testing::_))
        .WillOnce(testing::DoAll(
            testing::SetArgPointee<3>(reinterpret_cast<sqlite3_stmt*>(1)),
            testing::Return(SQLITE_OK)
        ));
    
    EXPECT_CALL(*sqliteMock, bind_int(testing::_, 1, 1))
        .WillOnce(testing::Return(SQLITE_OK));
    
    EXPECT_CALL(*sqliteMock, step(testing::_))
        .WillOnce(testing::Return(SQLITE_ROW));
    
    // Get user
    auto retrievedUser = dbManager->getUserById(1);
    EXPECT_FALSE(retrievedUser.empty());
    
    // Mock user update
    EXPECT_CALL(*sqliteMock, prepare_v2(testing::_, testing::HasSubstr("UPDATE users"), testing::_, testing::_, testing::_))
        .WillOnce(testing::DoAll(
            testing::SetArgPointee<3>(reinterpret_cast<sqlite3_stmt*>(1)),
            testing::Return(SQLITE_OK)
        ));
    
    // Update user
    bool updateSuccess = dbManager->updateUser(1, {{"username", "updateduser"}});
    EXPECT_TRUE(updateSuccess);
}

// Test authentication
TEST_F(DBManagerTest, Authentication) {
    // Setup basic database operations
    setupBasicDatabaseOperations();
    
    // Mock user lookup by email
    EXPECT_CALL(*sqliteMock, prepare_v2(testing::_, testing::HasSubstr("SELECT * FROM users WHERE email = ?"), testing::_, testing::_, testing::_))
        .WillOnce(testing::DoAll(
            testing::SetArgPointee<3>(reinterpret_cast<sqlite3_stmt*>(1)),
            testing::Return(SQLITE_OK)
        ));
    
    EXPECT_CALL(*sqliteMock, bind_text(testing::_, 1, testing::StrEq("test@example.com"), testing::_, testing::_))
        .WillOnce(testing::Return(SQLITE_OK));
    
    EXPECT_CALL(*sqliteMock, step(testing::_))
        .WillOnce(testing::Return(SQLITE_ROW));
    
    // Mock column values for password hash
    EXPECT_CALL(*sqliteMock, column_text(testing::_, testing::_))
        .WillRepeatedly(testing::Return(reinterpret_cast<const unsigned char*>("hashed_password")));
    
    // Authenticate user
    auto userInfo = dbManager->authenticateUser("test@example.com", "password");
    EXPECT_FALSE(userInfo.empty());
}

// Test category operations
TEST_F(DBManagerTest, CategoryOperations) {
    // Setup basic database operations
    setupBasicDatabaseOperations();
    
    // Mock get all categories
    EXPECT_CALL(*sqliteMock, prepare_v2(testing::_, testing::HasSubstr("SELECT DISTINCT category FROM articles"), testing::_, testing::_, testing::_))
        .WillOnce(testing::DoAll(
            testing::SetArgPointee<3>(reinterpret_cast<sqlite3_stmt*>(1)),
            testing::Return(SQLITE_OK)
        ));
    
    EXPECT_CALL(*sqliteMock, step(testing::_))
        .WillOnce(testing::Return(SQLITE_ROW))
        .WillOnce(testing::Return(SQLITE_ROW))
        .WillOnce(testing::Return(SQLITE_DONE));
    
    // Mock column values for categories
    EXPECT_CALL(*sqliteMock, column_text(testing::_, 0))
        .WillOnce(testing::Return(reinterpret_cast<const unsigned char*>("Technology")))
        .WillOnce(testing::Return(reinterpret_cast<const unsigned char*>("Business")));
    
    // Get all categories
    auto categories = dbManager->getAllCategories();
    
    // Verify we got 2 categories
    EXPECT_EQ(categories.size(), 2);
    EXPECT_EQ(categories[0], "Technology");
    EXPECT_EQ(categories[1], "Business");
}

// Test external server operations
TEST_F(DBManagerTest, ExternalServerOperations) {
    // Setup basic database operations
    setupBasicDatabaseOperations();
    
    // Mock get all external servers
    EXPECT_CALL(*sqliteMock, prepare_v2(testing::_, testing::HasSubstr("SELECT * FROM external_servers"), testing::_, testing::_, testing::_))
        .WillOnce(testing::DoAll(
            testing::SetArgPointee<3>(reinterpret_cast<sqlite3_stmt*>(1)),
            testing::Return(SQLITE_OK)
        ));
    
    EXPECT_CALL(*sqliteMock, step(testing::_))
        .WillOnce(testing::Return(SQLITE_ROW))
        .WillOnce(testing::Return(SQLITE_DONE));
    
    // Get all external servers
    auto servers = dbManager->getAllExternalServers();
    EXPECT_EQ(servers.size(), 1);
    
    // Mock update server status
    EXPECT_CALL(*sqliteMock, prepare_v2(testing::_, testing::HasSubstr("UPDATE external_servers SET status = ? WHERE server_name = ?"), testing::_, testing::_, testing::_))
        .WillOnce(testing::DoAll(
            testing::SetArgPointee<3>(reinterpret_cast<sqlite3_stmt*>(1)),
            testing::Return(SQLITE_OK)
        ));
    
    // Update server status
    dbManager->updateExternalServerStatusByName("News API", "active");
}

// Test transaction operations
TEST_F(DBManagerTest, TransactionOperations) {
    // Setup basic database operations
    setupBasicDatabaseOperations();
    
    // Mock transaction begin
    EXPECT_CALL(*sqliteMock, exec(testing::_, testing::StrEq("BEGIN TRANSACTION"), testing::_, testing::_, testing::_))
        .WillOnce(testing::Return(SQLITE_OK));
    
    // Begin transaction
    bool beginSuccess = dbManager->beginTransaction();
    EXPECT_TRUE(beginSuccess);
    
    // Mock transaction commit
    EXPECT_CALL(*sqliteMock, exec(testing::_, testing::StrEq("COMMIT"), testing::_, testing::_, testing::_))
        .WillOnce(testing::Return(SQLITE_OK));
    
    // Commit transaction
    bool commitSuccess = dbManager->commitTransaction();
    EXPECT_TRUE(commitSuccess);
    
    // Mock transaction rollback
    EXPECT_CALL(*sqliteMock, exec(testing::_, testing::StrEq("ROLLBACK"), testing::_, testing::_, testing::_))
        .WillOnce(testing::Return(SQLITE_OK));
    
    // Rollback transaction
    bool rollbackSuccess = dbManager->rollbackTransaction();
    EXPECT_TRUE(rollbackSuccess);
}

// Test error handling
TEST_F(DBManagerTest, ErrorHandling) {
    // Setup basic database operations
    setupBasicDatabaseOperations();
    
    // Mock failed statement preparation
    EXPECT_CALL(*sqliteMock, prepare_v2(testing::_, testing::HasSubstr("INSERT INTO articles"), testing::_, testing::_, testing::_))
        .WillOnce(testing::Return(SQLITE_ERROR));
    
    // Mock error message
    EXPECT_CALL(*sqliteMock, errmsg(testing::_))
        .WillOnce(testing::Return("SQL syntax error"));
    
    // Try to store article - should fail
    bool success = dbManager->storeArticle(createSampleArticle());
    EXPECT_FALSE(success);
}

// Test database cleanup
TEST_F(DBManagerTest, DatabaseCleanup) {
    // Setup basic database operations
    setupBasicDatabaseOperations();
    
    // Mock database close
    EXPECT_CALL(*sqliteMock, close(testing::_))
        .WillOnce(testing::Return(SQLITE_OK));
    
    // Close database (this happens in destructor, but we can test the method directly)
    bool success = dbManager->close();
    EXPECT_TRUE(success);
}

// Test bulk operations
TEST_F(DBManagerTest, BulkOperations) {
    // Setup basic database operations
    setupBasicDatabaseOperations();
    
    // Create multiple articles
    std::vector<nlohmann::json> articles;
    articles.push_back(createSampleArticle());
    articles.push_back(createSampleArticle());
    
    // Mock transaction begin
    EXPECT_CALL(*sqliteMock, exec(testing::_, testing::StrEq("BEGIN TRANSACTION"), testing::_, testing::_, testing::_))
        .WillOnce(testing::Return(SQLITE_OK));
    
    // Mock prepare statement twice (once per article)
    EXPECT_CALL(*sqliteMock, prepare_v2(testing::_, testing::HasSubstr("INSERT INTO articles"), testing::_, testing::_, testing::_))
        .Times(2)
        .WillRepeatedly(testing::DoAll(
            testing::SetArgPointee<3>(reinterpret_cast<sqlite3_stmt*>(1)),
            testing::Return(SQLITE_OK)
        ));
    
    // Mock transaction commit
    EXPECT_CALL(*sqliteMock, exec(testing::_, testing::StrEq("COMMIT"), testing::_, testing::_, testing::_))
        .WillOnce(testing::Return(SQLITE_OK));
    
    // Store articles in bulk
    bool success = dbManager->bulkStoreArticles(articles);
    EXPECT_TRUE(success);
}

// Test pagination
TEST_F(DBManagerTest, Pagination) {
    // Setup basic database operations
    setupBasicDatabaseOperations();
    
    // Expect specific SQL with LIMIT and OFFSET
    EXPECT_CALL(*sqliteMock, prepare_v2(testing::_, testing::HasSubstr("LIMIT ? OFFSET ?"), testing::_, testing::_, testing::_))
        .WillOnce(testing::DoAll(
            testing::SetArgPointee<3>(reinterpret_cast<sqlite3_stmt*>(1)),
            testing::Return(SQLITE_OK)
        ));
    
    // Expect binding of limit and offset
    EXPECT_CALL(*sqliteMock, bind_int(testing::_, 1, 10))
        .WillOnce(testing::Return(SQLITE_OK));
    
    EXPECT_CALL(*sqliteMock, bind_int(testing::_, 2, 20))
        .WillOnce(testing::Return(SQLITE_OK));
    
    // Get paginated articles
    auto articles = dbManager->getArticles(10, 20, "");
    
    // Success is verified by the expectations above
}

// Test handling malformed JSON
TEST_F(DBManagerTest, MalformedJson) {
    // Setup basic database operations
    setupBasicDatabaseOperations();
    
    // Create malformed article (missing required fields)
    nlohmann::json malformedArticle = {
        {"title", "Test Article"}
        // Missing other required fields
    };
    
    // Store malformed article - should fail validation
    bool success = dbManager->storeArticle(malformedArticle);
    EXPECT_FALSE(success);
}

// Test handling database connection issues
TEST_F(DBManagerTest, ConnectionIssues) {
    // Mock failed database connection
    EXPECT_CALL(*sqliteMock, open(testing::_, testing::_))
        .WillOnce(testing::Return(SQLITE_CANTOPEN));
    
    // Mock error message
    EXPECT_CALL(*sqliteMock, errmsg(testing::_))
        .WillOnce(testing::Return("unable to open database file"));
    
    // Try to store article with no connection - should fail
    bool success = dbManager->storeArticle(createSampleArticle());
    EXPECT_FALSE(success);
}