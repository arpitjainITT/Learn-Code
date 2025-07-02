#ifndef ADMINSERVICE_HPP
#define ADMINSERVICE_HPP

#include <nlohmann/json.hpp>
#include <string>

class AdminService {
public:
    static nlohmann::json listExternalServers();
    static nlohmann::json viewExternalServerDetails(int serverId);
    static bool updateExternalServerApiKey(int serverId, const std::string& newKey);
    static bool updateExternalServerStatus(int serverId, const std::string& newStatus);
    static bool updateExternalServerStatus(const std::string& serverName, const std::string& status);
    static bool addCategory(const std::string& category);
};

#endif 
