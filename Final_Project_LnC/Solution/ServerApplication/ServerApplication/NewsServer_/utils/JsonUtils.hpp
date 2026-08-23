#pragma once

#include <nlohmann/json.hpp>
#include <string>

/**
 * JsonUtils.hpp
 *
 * Small JSON helper utilities shared by the adapter implementations.
 */
namespace JsonUtils {

    /**
     * Returns the string value of key in j, or "" if the key is absent or
     * the value is not a string.  Prevents exceptions from missing/null fields
     * in external API responses.
     */
    inline std::string safeGet(const nlohmann::json& j, const std::string& key) {
        return (j.contains(key) && j[key].is_string())
               ? j[key].get<std::string>()
               : "";
    }

} // namespace JsonUtils
