#include "CategoryClassifier.hpp"
#include <algorithm>
#include <initializer_list>

std::string toLower(const std::string& input) {
    std::string lower = input;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}

bool containsKeyword(const std::string& text, const std::initializer_list<std::string>& keywords) {
    std::string lowerText = toLower(text);
    for (const auto& keyword : keywords) {
        if (lowerText.find(keyword) != std::string::npos) return true;
    }
    return false;
}

std::string CategoryClassifier::classify(const std::string& title, const std::string& content) {
    std::string combined = title + " " + content;

    if (containsKeyword(combined, {
        "ai", "tech", "software", "hardware", "gadgets", "startup", "technology", "robot", "innovation", "cybersecurity"
    })) return "Technology";

    if (containsKeyword(combined, {
        "market", "stock", "finance", "economy", "business", "trade", "investment", "ipo", "revenue", "profit", "merger"
    })) return "Business";

    if (containsKeyword(combined, {
        "covid", "health", "medicine", "hospital", "doctor", "vaccine", "virus", "mental", "surgery", "nutrition", "disease"
    })) return "Health";

    if (containsKeyword(combined, {
        "movie", "celebrity", "tv", "film", "entertainment", "music", "drama", "series", "actor", "actress", "trailer"
    })) return "Entertainment";

    if (containsKeyword(combined, {
        "football", "cricket", "tennis", "olympics", "goal", "match", "tournament", "score", "basketball", "league", "team"
    })) return "Sports";

    if (containsKeyword(combined, {
        "nasa", "science", "space", "experiment", "research", "discovery", "astronomy", "quantum", "physics", "scientist", "theory"
    })) return "Science";

    if (containsKeyword(combined, {
        "election", "government", "president", "prime minister", "senate", "parliament", "vote", "policy", "democracy", "bill", "campaign"
    })) return "Politics";

    if (containsKeyword(combined, {
        "climate", "global warming", "environment", "pollution", "wildlife", "sustainability", "recycle", "carbon", "eco", "deforestation", "greenhouse"
    })) return "Environment";

    if (containsKeyword(combined, {
        "school", "university", "education", "students", "teachers", "curriculum", "exam", "learning", "degree", "campus", "scholarship"
    })) return "Education";

    if (containsKeyword(combined, {
        "travel", "vacation", "tourism", "destination", "trip", "flight", "hotel", "beach", "passport", "explore", "itinerary"
    })) return "Travel";

    return "General"; 
}
