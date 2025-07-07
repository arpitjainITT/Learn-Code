#ifndef ADMINSERVICE_H
#define ADMINSERVICE_H

#include <string>
#include <vector>

struct ExternalServer {
    int id;
    std::string name;
    std::string status;
    std::string lastAccessed;
    std::string apiKey;
};

struct ReportedArticle {
    int id;
    std::string title;
    int reportCount;
    bool isHidden;
    int numReports;
};

class AdminService {
public:
    static std::vector<ExternalServer> getAllServers();
    static ExternalServer getServerDetails(int id);
    static void updateServerApiKey(int id, const std::string& newKey);
    static void updateServerStatus(int id, const std::string& newStatus);
    static void addCategory(const std::string& categoryName);
    static std::vector<ReportedArticle> getReportedArticles();
    static void hideArticle(int articleId);
    static void unhideArticle(int articleId);
    static void hideCategory(const std::string& category);
    static void unhideCategory(const std::string& category);
    static void addFilteredKeyword(const std::string& keyword);
    static void removeFilteredKeyword(const std::string& keyword);
    static std::vector<std::string> getHiddenCategories();
    static std::vector<std::string> getFilteredKeywords();
};

#endif 
