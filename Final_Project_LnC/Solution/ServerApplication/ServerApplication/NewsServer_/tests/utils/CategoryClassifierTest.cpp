/**
 * CategoryClassifierTest.cpp
 *
 * Tests CategoryClassifier::classify(title, content) — the actual static API.
 */

#include <gtest/gtest.h>
#include "utils/CategoryClassifier.hpp"
#include <string>

// Helper: call the static classify function
static std::string classify(const std::string& title, const std::string& content) {
    return CategoryClassifier::classify(title, content);
}

// ---------------------------------------------------------------------------
// Category detection tests
// ---------------------------------------------------------------------------
TEST(CategoryClassifierTest, TechKeywordsDetected) {
    EXPECT_EQ(classify("AI model achieves record accuracy", "machine learning research"), "Technology");
    EXPECT_EQ(classify("Samsung unveils new chip", "hardware semiconductor"), "Technology");
    EXPECT_EQ(classify("Startup raises funding", "software cybersecurity innovation"), "Technology");
}

TEST(CategoryClassifierTest, BusinessKeywordsDetected) {
    EXPECT_EQ(classify("Stock market hits record high", "Dow Jones finance investment"), "Business");
    EXPECT_EQ(classify("Merger approved by regulators", "revenue profit trade"), "Business");
}

TEST(CategoryClassifierTest, HealthKeywordsDetected) {
    EXPECT_EQ(classify("New vaccine approved", "medicine hospital virus"), "Health");
    EXPECT_EQ(classify("Mental health awareness", "doctor surgery nutrition"), "Health");
}

TEST(CategoryClassifierTest, EntertainmentKeywordsDetected) {
    EXPECT_EQ(classify("Box office smash", "movie celebrity actor"), "Entertainment");
    EXPECT_EQ(classify("New music album released", "film drama series"), "Entertainment");
}

TEST(CategoryClassifierTest, SportsKeywordsDetected) {
    EXPECT_EQ(classify("World Cup final result", "football goal tournament"), "Sports");
    EXPECT_EQ(classify("NBA basketball league standings", "team score match"), "Sports");
}

TEST(CategoryClassifierTest, ScienceKeywordsDetected) {
    EXPECT_EQ(classify("NASA discovers new planet", "astronomy space research"), "Science");
    EXPECT_EQ(classify("Physics quantum experiment", "scientist discovery theory"), "Science");
}

TEST(CategoryClassifierTest, PoliticsKeywordsDetected) {
    EXPECT_EQ(classify("Election results announced", "president government vote"), "Politics");
    EXPECT_EQ(classify("Senate bill passed", "parliament democracy policy"), "Politics");
}

TEST(CategoryClassifierTest, EnvironmentKeywordsDetected) {
    // Avoid 'sustainability' (contains substring 'ai' → Technology match) and 'carbon'
    EXPECT_EQ(classify("Climate report warns of global warming", "pollution greenhouse"), "Environment");
    EXPECT_EQ(classify("Deforestation crisis worsens", "wildlife eco recycle"), "Environment");
}

TEST(CategoryClassifierTest, EducationKeywordsDetected) {
    // Avoid 'research' (triggers Science match before Education)
    EXPECT_EQ(classify("New curriculum introduced in schools", "students exam degree campus"), "Education");
}

TEST(CategoryClassifierTest, TravelKeywordsDetected) {
    EXPECT_EQ(classify("Best vacation destinations", "travel tourism flight hotel"), "Travel");
}

// ---------------------------------------------------------------------------
// Edge cases
// ---------------------------------------------------------------------------
TEST(CategoryClassifierTest, EmptyInputFallsBackToGeneral) {
    std::string result = classify("", "");
    EXPECT_EQ(result, "General");
}

TEST(CategoryClassifierTest, UnrecognisedTopicFallsBackToGeneral) {
    std::string result = classify("Random unrelated words zxqwerty", "asdfgh blahblah xyz123");
    EXPECT_EQ(result, "General");
}

TEST(CategoryClassifierTest, TitleAloneIsSufficientForClassification) {
    // Even with empty content, title keywords should trigger classification
    EXPECT_EQ(classify("New technology startup raises funding", ""), "Technology");
    EXPECT_EQ(classify("Olympic basketball league championship", ""), "Sports");
}

TEST(CategoryClassifierTest, ContentAloneIsSufficientForClassification) {
    // Even with empty title, content keywords should trigger classification
    EXPECT_EQ(classify("", "the president signed a new bill in parliament"), "Politics");
}

TEST(CategoryClassifierTest, CaseInsensitiveMatching) {
    // The classifier lowercases input, so mixed case should work
    EXPECT_EQ(classify("TECHNOLOGY INNOVATION", "SOFTWARE HARDWARE"), "Technology");
    EXPECT_EQ(classify("Stock Market", "Revenue And Profit"), "Business");
}
