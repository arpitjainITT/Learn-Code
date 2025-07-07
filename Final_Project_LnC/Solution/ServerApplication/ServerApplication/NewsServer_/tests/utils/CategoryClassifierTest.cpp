#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "utils/CategoryClassifier.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <memory>

// Test fixture for CategoryClassifier
class CategoryClassifierTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create CategoryClassifier instance
        classifier = std::make_unique<CategoryClassifier>();
        
        // Set up sample article for testing
        sampleArticle = {
            {"title", "New AI Technology Breakthrough"},
            {"content", "Scientists have developed a new machine learning algorithm that can process natural language with unprecedented accuracy. The system uses deep neural networks and transformer architecture to understand context and nuances in text. Researchers at MIT say this could revolutionize how computers interact with humans."},
            {"source", "Tech News Daily"},
            {"published_at", "2023-07-05T10:30:00Z"},
            {"author", "Jane Smith"}
        };
    }

    // Helper to create article with specific content
    nlohmann::json createArticleWithContent(const std::string& title, const std::string& content) {
        auto article = sampleArticle;
        article["title"] = title;
        article["content"] = content;
        return article;
    }

    std::unique_ptr<CategoryClassifier> classifier;
    nlohmann::json sampleArticle;
};

// Test basic classification functionality
TEST_F(CategoryClassifierTest, BasicClassification) {
    // Test technology article
    auto techArticle = createArticleWithContent(
        "Quantum Computing Advances",
        "IBM announced a new 1000-qubit quantum processor today, marking a significant milestone in quantum computing technology."
    );
    
    std::string category = classifier->classifyArticle(techArticle);
    EXPECT_EQ(category, "Technology");
    
    // Test politics article
    auto politicsArticle = createArticleWithContent(
        "New Election Law Passed",
        "Congress has passed a new law regulating campaign finance and voting procedures across the country."
    );
    
    category = classifier->classifyArticle(politicsArticle);
    EXPECT_EQ(category, "Politics");
    
    // Test business article
    auto businessArticle = createArticleWithContent(
        "Stock Market Reaches Record High",
        "The Dow Jones Industrial Average surpassed 40,000 points for the first time, as tech stocks continue to perform well."
    );
    
    category = classifier->classifyArticle(businessArticle);
    EXPECT_EQ(category, "Business");
}

// Test edge cases
TEST_F(CategoryClassifierTest, EdgeCases) {
    // Test empty content
    auto emptyArticle = createArticleWithContent("Empty Article", "");
    std::string category = classifier->classifyArticle(emptyArticle);
    EXPECT_EQ(category, "Uncategorized");
    
    // Test very short content
    auto shortArticle = createArticleWithContent("Short", "Brief note.");
    category = classifier->classifyArticle(shortArticle);
    EXPECT_NE(category, ""); // Should return some category or "Uncategorized"
    
    // Test article with mixed topics
    auto mixedArticle = createArticleWithContent(
        "Tech Company CEO Runs for Office",
        "The CEO of a major technology company has announced plans to run for Senate, citing concerns about regulation of the tech industry."
    );
    
    category = classifier->classifyArticle(mixedArticle);
    // Could be either Technology or Politics, but should not be empty
    EXPECT_TRUE(category == "Technology" || category == "Politics");
}

// Test configuration options
TEST_F(CategoryClassifierTest, ConfigurationOptions) {
    // Test with custom categories
    std::vector<std::string> customCategories = {"Science", "Arts", "Sports"};
    classifier->setCategories(customCategories);
    
    auto sportsArticle = createArticleWithContent(
        "World Cup Final Results",
        "The national team won the World Cup final match with a score of 3-2 in overtime."
    );
    
    std::string category = classifier->classifyArticle(sportsArticle);
    EXPECT_EQ(category, "Sports");
    
    // Test with custom threshold
    classifier->setConfidenceThreshold(0.8); // Set high threshold
    
    auto ambiguousArticle = createArticleWithContent(
        "Cultural Impact of Technology",
        "The article discusses how smartphones have changed social interactions and cultural norms."
    );
    
    category = classifier->classifyArticle(ambiguousArticle);
    // With high threshold, might return Uncategorized
    EXPECT_TRUE(category == "Uncategorized" || category == "Science" || category == "Arts");
    
    // Reset threshold to normal
    classifier->setConfidenceThreshold(0.5);
}

// Test accuracy with known articles
TEST_F(CategoryClassifierTest, ClassificationAccuracy) {
    // Create test set with known categories
    std::vector<std::pair<nlohmann::json, std::string>> testSet = {
        {createArticleWithContent(
            "SpaceX Launches New Satellite",
            "SpaceX successfully launched a new communication satellite into orbit yesterday using the Falcon 9 rocket."
        ), "Technology"},
        
        {createArticleWithContent(
            "New Cancer Treatment Shows Promise",
            "Researchers have developed a new immunotherapy approach that showed remarkable results in clinical trials."
        ), "Health"},
        
        {createArticleWithContent(
            "Financial Markets React to Interest Rate Hike",
            "Stock markets declined after the Federal Reserve announced a 0.5% increase in interest rates."
        ), "Business"},
        
        {createArticleWithContent(
            "Movie Review: Latest Blockbuster",
            "The new sci-fi film delivers stunning visuals but lacks character development and coherent storytelling."
        ), "Entertainment"}
    };
    
    // Test classification accuracy
    int correctCount = 0;
    for (const auto& testCase : testSet) {
        std::string predictedCategory = classifier->classifyArticle(testCase.first);
        if (predictedCategory == testCase.second) {
            correctCount++;
        }
    }
    
    // Expect at least 75% accuracy
    float accuracy = static_cast<float>(correctCount) / testSet.size();
    EXPECT_GE(accuracy, 0.75f);
}

// Test classification with multilingual content
TEST_F(CategoryClassifierTest, MultilingualContent) {
    // Test Spanish content
    auto spanishArticle = createArticleWithContent(
        "Avances en Tecnología de Energía Solar",
        "Científicos han desarrollado nuevos paneles solares con eficiencia récord del 35% en condiciones estándar."
    );
    
    std::string category = classifier->classifyArticle(spanishArticle);
    // Should still detect this as Technology or Energy/Science
    EXPECT_TRUE(category == "Technology" || category == "Science" || category == "Energy");
    
    // Test French content
    auto frenchArticle = createArticleWithContent(
        "Élections Législatives en France",
        "Le premier tour des élections législatives a eu lieu hier avec une participation record dans plusieurs régions."
    );
    
    category = classifier->classifyArticle(frenchArticle);
    // Should detect as Politics despite language
    EXPECT_TRUE(category == "Politics" || category == "World" || category == "International");
}

// Test performance with very long articles
TEST_F(CategoryClassifierTest, LongArticlePerformance) {
    // Create a very long article
    std::string longContent = "Technology news. ";
    // Repeat the content to make it very long
    for (int i = 0; i < 1000; i++) {
        longContent += "More technology news about computers, software, hardware, algorithms, and artificial intelligence. ";
    }
    
    auto longArticle = createArticleWithContent("Technology News Roundup", longContent);
    
    // Measure time to classify
    auto start = std::chrono::high_resolution_clock::now();
    std::string category = classifier->classifyArticle(longArticle);
    auto end = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double, std::milli> elapsed = end - start;
    
    // Classification should be correct
    EXPECT_EQ(category, "Technology");
    
    // Classification should be reasonably fast (adjust threshold as needed)
    EXPECT_LT(elapsed.count(), 1000.0); // Less than 1 second
}

// Test classification consistency
TEST_F(CategoryClassifierTest, ClassificationConsistency) {
    // Test the same article multiple times
    auto article = createArticleWithContent(
        "New Economic Policy Announced",
        "The government has announced a new economic stimulus package aimed at supporting small businesses and creating jobs."
    );
    
    std::string category1 = classifier->classifyArticle(article);
    std::string category2 = classifier->classifyArticle(article);
    std::string category3 = classifier->classifyArticle(article);
    
    // All classifications should be the same
    EXPECT_EQ(category1, category2);
    EXPECT_EQ(category2, category3);
}

// Test with complex article structure
TEST_F(CategoryClassifierTest, ComplexArticleStructure) {
    // Create article with nested structure
    nlohmann::json complexArticle = {
        {"title", "Climate Change Impact on Economics"},
        {"content", "Analysis of how climate policies affect global markets and supply chains."},
        {"sections", {
            {
                {"heading", "Environmental Regulations"},
                {"content", "New carbon taxes in Europe are changing manufacturing strategies."}
            },
            {
                {"heading", "Market Response"},
                {"content", "Stock prices of green energy companies have risen 15% this quarter."}
            }
        }},
        {"source", "Economic Review"},
        {"published_at", "2023-07-05T10:30:00Z"},
        {"author", "John Economist"}
    };
    
    // Test if classifier can handle complex structure
    std::string category = classifier->classifyArticle(complexArticle);
    
    // Should classify as Business, Economics, or Environment
    EXPECT_TRUE(category == "Business" || category == "Economics" || category == "Environment");
}

// Test category customization
TEST_F(CategoryClassifierTest, CategoryCustomization) {
    // Test adding custom category with keywords
    classifier->addCategory("Cryptocurrency", {"bitcoin", "ethereum", "blockchain", "crypto", "token", "mining"});
    
    auto cryptoArticle = createArticleWithContent(
        "Bitcoin Price Surges",
        "Bitcoin reached a new all-time high today as institutional investors continue to adopt the cryptocurrency."
    );
    
    std::string category = classifier->classifyArticle(cryptoArticle);
    EXPECT_EQ(category, "Cryptocurrency");
    
    // Test removing a category
    classifier->removeCategory("Cryptocurrency");
    
    category = classifier->classifyArticle(cryptoArticle);
    // Should now classify as Business or Technology
    EXPECT_TRUE(category == "Business" || category == "Technology");
}

// Test error handling
TEST_F(CategoryClassifierTest, ErrorHandling) {
    // Test with invalid JSON
    nlohmann::json invalidArticle = nlohmann::json::parse("{}");
    
    std::string category = classifier->classifyArticle(invalidArticle);
    EXPECT_EQ(category, "Uncategorized");
    
    // Test with null content
    auto nullContentArticle = sampleArticle;
    nullContentArticle["content"] = nullptr;
    
    category = classifier->classifyArticle(nullContentArticle);
    EXPECT_EQ(category, "Uncategorized");
}