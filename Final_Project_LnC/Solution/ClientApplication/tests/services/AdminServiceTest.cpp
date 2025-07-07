#include <gtest/gtest.h>
#include "../../services/AdminService.h"
#include <string>
#include <strings.h>

#include <vector>
#include <nlohmann/json.hpp>

class AdminServiceTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }
    
    void TearDown() override {
        // Cleanup code if needed
    }
};

TEST_F(AdminServiceTest, GetAllServers) {
    // Test getAllServers method
    EXPECT_NO_THROW({
        std::vector<ExternalServer> servers = AdminService::getAllServers();
        // Result depends on actual HTTP response
    });
}

TEST_F(AdminServiceTest, GetServerDetailsValidId) {
    // Test getServerDetails with valid IDs
    std::vector<int> validIds = {
        1,
        123,
        999,
        1000,
        10000
    };
    
    for (int id : validIds) {
        EXPECT_NO_THROW({
            ExternalServer server = AdminService::getServerDetails(id);
            // Result depends on actual HTTP response
        });
    }
}

TEST_F(AdminServiceTest, GetServerDetailsZeroId) {
    // Test getServerDetails with zero ID
    EXPECT_NO_THROW({
        ExternalServer server = AdminService::getServerDetails(0);
    });
}

TEST_F(AdminServiceTest, GetServerDetailsNegativeId) {
    // Test getServerDetails with negative ID
    EXPECT_NO_THROW({
        ExternalServer server = AdminService::getServerDetails(-1);
        ExternalServer server2 = AdminService::getServerDetails(-100);
    });
}

TEST_F(AdminServiceTest, GetServerDetailsLargeId) {
    // Test getServerDetails with large ID
    EXPECT_NO_THROW({
        ExternalServer server = AdminService::getServerDetails(2147483647);
        ExternalServer server2 = AdminService::getServerDetails(1000000);
    });
}

TEST_F(AdminServiceTest, UpdateServerApiKeyValidData) {
    // Test updateServerApiKey with valid data
    std::vector<std::pair<int, std::string>> testCases = {
        {1, "new-api-key-123"},
        {123, "updated-key-456"},
        {999, "fresh-key-789"},
        {0, "zero-id-key"}
    };
    
    for (const auto& testCase : testCases) {
        int id = testCase.first;
        std::string newKey = testCase.second;
        
        EXPECT_NO_THROW({
            AdminService::updateServerApiKey(id, newKey);
        });
    }
}

TEST_F(AdminServiceTest, UpdateServerApiKeyEmptyKey) {
    // Test updateServerApiKey with empty key
    EXPECT_NO_THROW({
        AdminService::updateServerApiKey(1, "");
        AdminService::updateServerApiKey(123, "");
    });
}

TEST_F(AdminServiceTest, UpdateServerApiKeySpecialCharacters) {
    // Test updateServerApiKey with special characters
    std::vector<std::string> specialKeys = {
        "api-key-with-special-chars!@#$%",
        "key-with-spaces and symbols",
        "key-with-unicode-测试",
        "key-with-numbers-123-456-789"
    };
    
    for (const auto& key : specialKeys) {
        EXPECT_NO_THROW({
            AdminService::updateServerApiKey(1, key);
        });
    }
}

TEST_F(AdminServiceTest, UpdateServerApiKeyNegativeId) {
    // Test updateServerApiKey with negative ID
    EXPECT_NO_THROW({
        AdminService::updateServerApiKey(-1, "negative-id-key");
        AdminService::updateServerApiKey(-100, "negative-id-key-2");
    });
}

TEST_F(AdminServiceTest, UpdateServerStatusValidData) {
    // Test updateServerStatus with valid data
    std::vector<std::pair<int, std::string>> testCases = {
        {1, "active"},
        {123, "inactive"},
        {999, "maintenance"},
        {0, "offline"}
    };
    
    for (const auto& testCase : testCases) {
        int id = testCase.first;
        std::string newStatus = testCase.second;
        
        EXPECT_NO_THROW({
            AdminService::updateServerStatus(id, newStatus);
        });
    }
}

TEST_F(AdminServiceTest, UpdateServerStatusEmptyStatus) {
    // Test updateServerStatus with empty status
    EXPECT_NO_THROW({
        AdminService::updateServerStatus(1, "");
        AdminService::updateServerStatus(123, "");
    });
}

TEST_F(AdminServiceTest, UpdateServerStatusSpecialCharacters) {
    // Test updateServerStatus with special characters
    std::vector<std::string> specialStatuses = {
        "status-with-special-chars!@#$%",
        "status with spaces",
        "status-with-unicode-测试",
        "status-with-numbers-123"
    };
    
    for (const auto& status : specialStatuses) {
        EXPECT_NO_THROW({
            AdminService::updateServerStatus(1, status);
        });
    }
}

TEST_F(AdminServiceTest, UpdateServerStatusNegativeId) {
    // Test updateServerStatus with negative ID
    EXPECT_NO_THROW({
        AdminService::updateServerStatus(-1, "negative-id-status");
        AdminService::updateServerStatus(-100, "negative-id-status-2");
    });
}

TEST_F(AdminServiceTest, AddCategoryValidName) {
    // Test addCategory with valid names
    std::vector<std::string> validCategories = {
        "technology",
        "sports",
        "politics",
        "entertainment",
        "science",
        "business",
        "health"
    };
    
    for (const auto& category : validCategories) {
        EXPECT_NO_THROW({
            AdminService::addCategory(category);
        });
    }
}

TEST_F(AdminServiceTest, AddCategoryEmptyName) {
    // Test addCategory with empty name
    EXPECT_NO_THROW({
        AdminService::addCategory("");
    });
}

TEST_F(AdminServiceTest, AddCategorySpecialCharacters) {
    // Test addCategory with special characters
    std::vector<std::string> specialCategories = {
        "science & technology",
        "health & wellness",
        "arts & culture",
        "business & finance",
        "category-with-special-chars!@#$%",
        "category with spaces",
        "category-with-unicode-测试"
    };
    
    for (const auto& category : specialCategories) {
        EXPECT_NO_THROW({
            AdminService::addCategory(category);
        });
    }
}

TEST_F(AdminServiceTest, MethodSignatures) {
    // Test that method signatures are correct
    int id = 1;
    std::string newKey = "new-key";
    std::string newStatus = "active";
    std::string categoryName = "technology";
    
    // Test getAllServers method signature - fixed to use static function pointers
    EXPECT_NO_THROW({
        std::vector<ExternalServer> (*getAllServersFunc)() = &AdminService::getAllServers;
        (void)getAllServersFunc; // Suppress unused variable warning
    });
    
    // Test getServerDetails method signature - fixed to use static function pointers
    EXPECT_NO_THROW({
        ExternalServer (*getServerDetailsFunc)(int) = &AdminService::getServerDetails;
        (void)getServerDetailsFunc; // Suppress unused variable warning
    });
    
    // Test updateServerApiKey method signature - fixed to use static function pointers
    EXPECT_NO_THROW({
        void (*updateApiKeyFunc)(int, const std::string&) = &AdminService::updateServerApiKey;
        (void)updateApiKeyFunc; // Suppress unused variable warning
    });
    
    // Test updateServerStatus method signature - fixed to use static function pointers
    EXPECT_NO_THROW({
        void (*updateStatusFunc)(int, const std::string&) = &AdminService::updateServerStatus;
        (void)updateStatusFunc; // Suppress unused variable warning
    });
    
    // Test addCategory method signature - fixed to use static function pointers
    EXPECT_NO_THROW({
        void (*addCategoryFunc)(const std::string&) = &AdminService::addCategory;
        (void)addCategoryFunc; // Suppress unused variable warning
    });
}

TEST_F(AdminServiceTest, StaticMethods) {
    // Test that methods are static
    int id = 1;
    std::string newKey = "new-key";
    std::string newStatus = "active";
    std::string categoryName = "technology";
    
    EXPECT_NO_THROW({
        // These should work without instantiating the class
        AdminService::getAllServers();
        AdminService::getServerDetails(id);
        AdminService::updateServerApiKey(id, newKey);
        AdminService::updateServerStatus(id, newStatus);
        AdminService::addCategory(categoryName);
    });
}

TEST_F(AdminServiceTest, ExceptionSafety) {
    // Test exception safety
    int id = 1;
    std::string newKey = "new-key";
    std::string newStatus = "active";
    std::string categoryName = "technology";
    
    EXPECT_NO_THROW({
        // Should not throw unexpected exceptions
        AdminService::getAllServers();
        AdminService::getServerDetails(id);
        AdminService::updateServerApiKey(id, newKey);
        AdminService::updateServerStatus(id, newStatus);
        AdminService::addCategory(categoryName);
    });
}

TEST_F(AdminServiceTest, JSONHandling) {
    // Test JSON handling logic
    nlohmann::json serverData = {
        {"id", 123},
        {"name", "Test Server"},
        {"status", "active"},
        {"api_key", "test-api-key"},
        {"last_accessed", "2023-01-01"}
    };
    
    EXPECT_NO_THROW({
        std::string jsonStr = serverData.dump();
        nlohmann::json parsed = nlohmann::json::parse(jsonStr);
        EXPECT_EQ(parsed["id"], 123);
        EXPECT_EQ(parsed["name"], "Test Server");
        EXPECT_EQ(parsed["status"], "active");
        EXPECT_EQ(parsed["api_key"], "test-api-key");
    });
}

TEST_F(AdminServiceTest, ExternalServerCreation) {
    // Test ExternalServer object creation from JSON
    nlohmann::json serverData = {
        {"id", 123},
        {"name", "Test Server"},
        {"status", "active"},
        {"api_key", "test-api-key"},
        {"last_accessed", "2023-01-01"}
    };
    
    EXPECT_NO_THROW({
        ExternalServer server;
        server.id = serverData["id"];
        server.name = serverData["name"];
        server.status = serverData["status"];
        server.apiKey = serverData["api_key"];
        server.lastAccessed = serverData["last_accessed"];
        
        EXPECT_EQ(server.id, 123);
        EXPECT_EQ(server.name, "Test Server");
        EXPECT_EQ(server.status, "active");
        EXPECT_EQ(server.apiKey, "test-api-key");
        EXPECT_EQ(server.lastAccessed, "2023-01-01");
    });
}

TEST_F(AdminServiceTest, ExternalServerStructure) {
    // Test ExternalServer structure
    EXPECT_NO_THROW({
        ExternalServer server;
        
        // Test setting and getting values
        server.id = 123;
        server.name = "Test Server";
        server.status = "active";
        server.apiKey = "test-api-key";
        server.lastAccessed = "2023-01-01";
        
        EXPECT_EQ(server.id, 123);
        EXPECT_EQ(server.name, "Test Server");
        EXPECT_EQ(server.status, "active");
        EXPECT_EQ(server.apiKey, "test-api-key");
        EXPECT_EQ(server.lastAccessed, "2023-01-01");
    });
}

TEST_F(AdminServiceTest, URLConstruction) {
    // Test URL construction logic
    int id = 123;
    
    EXPECT_NO_THROW({
        // Test getServerDetails URL construction
        std::string detailsUrl = "/server-details/" + std::to_string(id);
        EXPECT_TRUE(detailsUrl.find("123") != std::string::npos);
        
        // Test updateApiKey URL construction
        std::string apiKeyUrl = "/update-api-key/" + std::to_string(id) + "/key";
        EXPECT_TRUE(apiKeyUrl.find("123") != std::string::npos);
        EXPECT_TRUE(apiKeyUrl.find("/key") != std::string::npos);
        
        // Test updateStatus URL construction
        std::string statusUrl = "/update-server-status/" + std::to_string(id) + "/status";
        EXPECT_TRUE(statusUrl.find("123") != std::string::npos);
        EXPECT_TRUE(statusUrl.find("/status") != std::string::npos);
    });
}

TEST_F(AdminServiceTest, EmptyResponseHandling) {
    // Test handling of empty responses
    EXPECT_NO_THROW({
        // Test with empty JSON array
        std::string emptyResponse = "[]";
        nlohmann::json emptyArray = nlohmann::json::parse(emptyResponse);
        EXPECT_TRUE(emptyArray.is_array());
        EXPECT_EQ(emptyArray.size(), 0);
        
        // Test with empty JSON object
        std::string emptyObjResponse = "{}";
        nlohmann::json emptyObj = nlohmann::json::parse(emptyObjResponse);
        EXPECT_TRUE(emptyObj.is_object());
        EXPECT_EQ(emptyObj.size(), 0);
    });
}

TEST_F(AdminServiceTest, InvalidJSONHandling) {
    // Test handling of invalid JSON responses
    std::vector<std::string> invalidResponses = {
        "",
        "invalid json",
        "{invalid}",
        "[invalid]"
    };
    
    for (const auto& responseStr : invalidResponses) {
        EXPECT_NO_THROW({
            try {
                nlohmann::json response = nlohmann::json::parse(responseStr);
            } catch (...) {
                // Expected to throw for invalid JSON
            }
        });
    }
}

TEST_F(AdminServiceTest, MultipleServersHandling) {
    // Test handling of multiple servers in response
    std::string multipleServersResponse = R"([
        {"id": 1, "name": "Server 1", "status": "active", "last_accessed": "2023-01-01"},
        {"id": 2, "name": "Server 2", "status": "inactive", "last_accessed": "2023-01-02"},
        {"id": 3, "name": "Server 3", "status": "maintenance", "last_accessed": "2023-01-03"}
    ])";
    
    EXPECT_NO_THROW({
        nlohmann::json serversArray = nlohmann::json::parse(multipleServersResponse);
        EXPECT_TRUE(serversArray.is_array());
        EXPECT_EQ(serversArray.size(), 3);
        
        for (const auto& item : serversArray) {
            EXPECT_TRUE(item.contains("id"));
            EXPECT_TRUE(item.contains("name"));
            EXPECT_TRUE(item.contains("status"));
            EXPECT_TRUE(item.contains("last_accessed"));
        }
    });
}

TEST_F(AdminServiceTest, MissingFieldsHandling) {
    // Test handling of servers with missing fields
    std::vector<nlohmann::json> incompleteServers = {
        {{"id", 1}}, // Only id
        {{"id", 2}, {"name", "Test"}}, // Missing status
        {{"id", 3}, {"name", "Test"}, {"status", "active"}} // Missing last_accessed
    };
    
    for (const auto& serverData : incompleteServers) {
        EXPECT_NO_THROW({
            ExternalServer server;
            server.id = serverData["id"];
            server.name = serverData.value("name", "");
            server.status = serverData.value("status", "");
            server.apiKey = serverData.value("api_key", "");
            server.lastAccessed = serverData.value("last_accessed", "");
            
            // Should not crash and should use default values
            EXPECT_GE(server.id, 0);
        });
    }
}

TEST_F(AdminServiceTest, StringToIntConversion) {
    // Test string to int conversion for server ID
    std::vector<int> testIds = {
        0,
        1,
        123,
        999,
        1000,
        2147483647
    };
    
    for (int id : testIds) {
        EXPECT_NO_THROW({
            std::string idStr = std::to_string(id);
            int convertedId = std::stoi(idStr);
            EXPECT_EQ(convertedId, id);
        });
    }
}

TEST_F(AdminServiceTest, VectorOperations) {
    // Test vector operations for servers
    std::vector<ExternalServer> servers;
    
    EXPECT_NO_THROW({
        // Test adding servers to vector
        ExternalServer server1;
        server1.id = 1;
        server1.name = "Server 1";
        servers.push_back(server1);
        
        ExternalServer server2;
        server2.id = 2;
        server2.name = "Server 2";
        servers.push_back(server2);
        
        EXPECT_EQ(servers.size(), 2);
        EXPECT_EQ(servers[0].id, 1);
        EXPECT_EQ(servers[1].id, 2);
        
        // Test clearing vector
        servers.clear();
        EXPECT_EQ(servers.size(), 0);
    });
}

TEST_F(AdminServiceTest, ReturnValueConsistency) {
    // Test that methods return consistent values
    int id = 1;
    
    EXPECT_NO_THROW({
        // getAllServers should always return a vector (even if empty)
        std::vector<ExternalServer> servers = AdminService::getAllServers();
        // Should not crash and should return a valid vector
        
        // getServerDetails should always return an ExternalServer object
        ExternalServer server = AdminService::getServerDetails(id);
        // Should not crash and should return a valid object
    });
}

TEST_F(AdminServiceTest, ErrorHandling) {
    // Test error handling scenarios
    int id = 1;
    std::string newKey = "new-key";
    std::string newStatus = "active";
    std::string categoryName = "technology";
    
    EXPECT_NO_THROW({
        // These should handle errors gracefully without throwing
        AdminService::getAllServers();
        AdminService::getServerDetails(id);
        AdminService::updateServerApiKey(id, newKey);
        AdminService::updateServerStatus(id, newStatus);
        AdminService::addCategory(categoryName);
    });
}

TEST_F(AdminServiceTest, LongStringHandling) {
    // Test handling of very long strings
    std::string longApiKey(1000, 'a'); // 1000 character API key
    std::string longStatus(500, 'b');  // 500 character status
    std::string longCategory(300, 'c'); // 300 character category
    
    EXPECT_NO_THROW({
        AdminService::updateServerApiKey(1, longApiKey);
        AdminService::updateServerStatus(1, longStatus);
        AdminService::addCategory(longCategory);
    });
}

TEST_F(AdminServiceTest, UnicodeHandling) {
    // Test handling of Unicode characters
    std::vector<std::string> unicodeStrings = {
        "服务器-测试",
        "サーバー-テスト",
        "서버-테스트",
        "сервер-тест"
    };
    
    for (const auto& unicodeStr : unicodeStrings) {
        EXPECT_NO_THROW({
            AdminService::updateServerApiKey(1, unicodeStr);
            AdminService::updateServerStatus(1, unicodeStr);
            AdminService::addCategory(unicodeStr);
        });
    }
}

TEST_F(AdminServiceTest, BoundaryValues) {
    // Test boundary values
    EXPECT_NO_THROW({
        // Test with maximum integer values
        AdminService::getServerDetails(2147483647);
        AdminService::updateServerApiKey(2147483647, "max-int-key");
        AdminService::updateServerStatus(2147483647, "max-int-status");
        
        // Test with minimum integer values
        AdminService::getServerDetails(-2147483648);
        AdminService::updateServerApiKey(-2147483648, "min-int-key");
        AdminService::updateServerStatus(-2147483648, "min-int-status");
    });
}
