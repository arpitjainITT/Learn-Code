#include "RouteManager.hpp"

// Include all service headers
#include "../services/UserService.hpp"
#include "../services/AdminService.hpp"
#include "../services/ArticleService.hpp"
#include "../services/NotificationService.hpp"
#include "notification_routes.hpp"

using namespace Pistache;

void RouteManager::init(Rest::Router& router) {
    using namespace Rest;

    // --- User Routes ---
    Routes::Post(router, "/api/signup", Routes::bind(&UserService::signup));
    Routes::Post(router, "/api/login", Routes::bind(&UserService::login));

    // --- Admin Routes ---
    Routes::Get(router, "/api/admin/listSources", Routes::bind(&AdminService::listSources));
    Routes::Get(router, "/api/admin/viewSourceDetails", Routes::bind(&AdminService::viewSourceDetails));
    Routes::Put(router, "/api/admin/updateSource/:id", Routes::bind(&AdminService::updateSource));
    Routes::Post(router, "/api/admin/addCategory", Routes::bind(&AdminService::addCategory));

    // --- Article Routes ---
    Routes::Get(router, "/api/articles", Routes::bind(&ArticleService::getAllArticles));
    Routes::Get(router, "/api/articles/byCategory/:category", Routes::bind(&ArticleService::getArticlesByCategory));
    Routes::Get(router, "/api/articles/:id", Routes::bind(&ArticleService::getArticleById));
    Routes::Post(router, "/api/articles/save", Routes::bind(&ArticleService::saveArticle));
    Routes::Get(router, "/api/articles/saved", Routes::bind(&ArticleService::getSavedArticles));       
    Routes::Delete(router, "/api/articles/saved/:id", Routes::bind(&ArticleService::deleteSavedArticle)); 

    // --- Notification Routes ---
    register_notification_routes(router);



    // More routes are in progress .....
}

