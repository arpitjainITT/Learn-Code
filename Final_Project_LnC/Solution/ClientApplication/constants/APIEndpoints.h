#ifndef API_ENDPOINTS_H
#define API_ENDPOINTS_H

#include <string>

namespace API {
    const std::string BASE_URL = "http://localhost:9080";

    // Auth
    const std::string LOGIN         = BASE_URL + "/auth/login";
    const std::string SIGNUP        = BASE_URL + "/auth/signup";

    // Articles
    const std::string ALL_ARTICLES          = BASE_URL + "/articles";
    const std::string ARTICLE_BY_ID         = BASE_URL + "/articles/";               // + id
    const std::string ARTICLES_BY_CATEGORY  = BASE_URL + "/articles/category/";      // + category
    const std::string SAVE_ARTICLE          = BASE_URL + "/articles/save";
    const std::string DELETE_SAVED_ARTICLE  = BASE_URL + "/articles/save";
    const std::string SAVED_ARTICLES        = BASE_URL + "/articles/saved/";         // + userId
    const std::string LIKE_ARTICLE          = BASE_URL + "/articles/";               // + id + /like
    const std::string DISLIKE_ARTICLE       = BASE_URL + "/articles/";               // + id + /dislike
    const std::string ARTICLE_REACTIONS     = BASE_URL + "/articles/";               // + id + /reactions
    const std::string SEARCH_ARTICLES       = BASE_URL + "/articles/search";


    // Admin
    const std::string ALL_SERVERS           = BASE_URL + "/admin/servers";
    const std::string SERVER_DETAILS        = BASE_URL + "/admin/servers/";          // + id
    const std::string UPDATE_API_KEY        = BASE_URL + "/admin/servers/";          // + id + /key
    const std::string UPDATE_SERVER_STATUS  = BASE_URL + "/admin/servers/";          // + id + /status
    const std::string ADD_CATEGORY          = BASE_URL + "/admin/category";

    // Notifications
    const std::string SET_CATEGORY_PREF     = BASE_URL + "/notifications/category";
    const std::string SET_KEYWORD_PREF      = BASE_URL + "/notifications/keyword";
    const std::string GET_PREFERENCES       = BASE_URL + "/notifications/preferences/"; // + userId
    const std::string USER_NOTIFICATIONS    = BASE_URL + "/notifications/";

}

#endif
