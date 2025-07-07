#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../services/AdminService.hpp"
#include "../../database/Database.hpp"
#include <nlohmann/json.hpp>

// Mock for Database class to avoid real database operations
class MockDatabase {
public:
    static testing::NiceMock<MockDatabase>& getInstance() {
        static testing::NiceMock<MockDatabase> instance;
        return instance;
    }

    MOCK_METHOD(nlohmann::json, getAllExternalServers, ());
    MOCK_METHOD(nlohmann::json, getExternalServerById, (int));
    MOCK_METHOD(void, updateExternalServerApiKey, (int, const std::string&));
    MOCK_METHOD(void, updateExternalServerStatus, (int, const std::string&));
    MOCK_METHOD(void, updateExternalServerStatusByName, (const std::string&, const std::string&));
    MOCK_METHOD(void, addCategory, (const std::string&));
};

// Test fixture for AdminService
class AdminServiceTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up mock behavior before each test
    }

    void TearDown() override {
        // Clean up after each test
    }

    // Helper to create sample server data
    nlohmann::json createSampleServers() {
        nlohmann::json servers = nlohmann::json::array();
        
        servers.push_back({
            {"id", 1},
            {"server_name", "News API"},
            {"api_url", "https://newsapi.org/v2/"},
            {"api_key", "sample-key-1234"},
            {"status", "active"},
            {"last_accessed", "2023-07-07 12:00:00"}
        });
        
        servers.push_back({
            {"id", 2},
            {"server_name", "The Guardian API"},
            {"api_url", "https://content.guardianapis.com/"},
            {"api_key", "guardian-api-key"},
            {"status", "inactive"},
            {"last_accessed", "2023-07-06 10:30:00"}
        });
        
        return servers;
    }

    // Helper to create a sample server
    nlohmann::json createSampleServer() {
        return {
            {"id", 1},
            {"server_name", "News API"},
            {"api_url", "https://newsapi.org/v2/"},
            {"api_key", "sample-key-1234"},
            {"status", "active"},
            {"last_accessed", "2023-07-07 12:00:00"}
        };
    }
};

// Test listing all external servers
TEST_F(AdminServiceTest, ListExternalServers) {
    // Set up expectations
    auto sampleServers = createSampleServers();
    EXPECT_CALL(MockDatabase::getInstance(), getAllExternalServers())
        .WillOnce(testing::Return(sampleServers));

    // Call method under test
    auto result = AdminService::listExternalServers();

    // Verify results
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0]["id"], 1);
    EXPECT_EQ(result[0]["server_name"], "News API");
    EXPECT_EQ(result[1]["id"], 2);
    EXPECT_EQ(result[1]["server_name"], "The Guardian API");
}

// Test viewing a specific external server's details
TEST_F(AdminServiceTest, ViewExternalServerDetails) {
    // Set up expectations
    auto sampleServer = createSampleServer();
    EXPECT_CALL(MockDatabase::getInstance(), getExternalServerById(1))
        .WillOnce(testing::Return(sampleServer));

    // Call method under test
    auto result = AdminService::viewExternalServerDetails(1);

    // Verify results
    EXPECT_EQ(result["id"], 1);
    EXPECT_EQ(result["server_name"], "News API");
    EXPECT_EQ(result["api_key"], "sample-key-1234");
    EXPECT_EQ(result["status"], "active");
}

// Test updating an external server's API key
TEST_F(AdminServiceTest, UpdateExternalServerApiKey) {
    // Set up expectations
    EXPECT_CALL(MockDatabase::getInstance(), updateExternalServerApiKey(1, "new-api-key"))
        .Times(1);

    // Call method under test
    bool result = AdminService::updateExternalServerApiKey(1, "new-api-key");

    // Verify results
    EXPECT_TRUE(result);
}

// Test updating an external server's status
TEST_F(AdminServiceTest, UpdateExternalServerStatus) {
    // Set up expectations
    EXPECT_CALL(MockDatabase::getInstance(), updateExternalServerStatus(1, "inactive"))
        .Times(1);

    // Call method under test
    bool result = AdminService::updateExternalServerStatus(1, "inactive");

    // Verify results
    EXPECT_TRUE(result);
}

// Test updating a server's status by name
TEST_F(AdminServiceTest, UpdateExternalServerStatusByName) {
    // Set up expectations
    EXPECT_CALL(MockDatabase::getInstance(), updateExternalServerStatusByName("News API", "maintenance"))
        .Times(1);

    // Call method under test
    bool result = AdminService::updateExternalServerStatus("News API", "maintenance");

    // Verify results
    EXPECT_TRUE(result);
}

// Test adding a new category
TEST_F(AdminServiceTest, AddCategory) {
    // Set up expectations
    EXPECT_CALL(MockDatabase::getInstance(), addCategory("Technology"))
        .Times(1);

    // Call method under test
    bool result = AdminService::addCategory("Technology");

    // Verify results
    EXPECT_TRUE(result);
}

// Test error handling when database operations fail
TEST_F(AdminServiceTest, ErrorHandling) {
    // Set up expectations for exception
    EXPECT_CALL(MockDatabase::getInstance(), updateExternalServerApiKey(1, "new-key"))
        .WillOnce(testing::Throw(std::runtime_error("Database error")));

    // Call method under test
    bool result = AdminService::updateExternalServerApiKey(1, "new-key");

    // Verify results - should return false due to exception
    EXPECT_FALSE(result);
}

// Test with invalid inputs
TEST_F(AdminServiceTest, InvalidInputs) {
    // Empty API key
    EXPECT_CALL(MockDatabase::getInstance(), updateExternalServerApiKey(1, ""))
        .Times(1);
    bool result1 = AdminService::updateExternalServerApiKey(1, "");
    EXPECT_TRUE(result1); // The service doesn't validate inputs, just passes to DB
    
    // Empty status
    EXPECT_CALL(MockDatabase::getInstance(), updateExternalServerStatus(1, ""))
        .Times(1);
    bool result2 = AdminService::updateExternalServerStatus(1, "");
    EXPECT_TRUE(result2);
    
    // Empty category
    EXPECT_CALL(MockDatabase::getInstance(), addCategory(""))
        .Times(1);
    bool result3 = AdminService::addCategory("");
    EXPECT_TRUE(result3);
}

// Test with non-existent server ID
TEST_F(AdminServiceTest, NonExistentServer) {
    // Set up expectations - return empty JSON
    EXPECT_CALL(MockDatabase::getInstance(), getExternalServerById(999))
        .WillOnce(testing::Return(nlohmann::json()));

    // Call method under test
    auto result = AdminService::viewExternalServerDetails(999);

    // Verify results - should return empty JSON
    EXPECT_TRUE(result.empty());
}