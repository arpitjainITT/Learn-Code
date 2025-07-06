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

class AdminService {
public:
    static std::vector<ExternalServer> getAllServers();
    static ExternalServer getServerDetails(int id);
    static void updateServerApiKey(int id, const std::string& newKey);
    static void updateServerStatus(int id, const std::string& newStatus);
    static void addCategory(const std::string& categoryName);
};

#endif 
