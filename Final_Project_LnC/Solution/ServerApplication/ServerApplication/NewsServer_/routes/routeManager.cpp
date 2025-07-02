#include "RouteManager.hpp"
#include "admin_routes.hpp"
#include "article_routes.hpp"
#include "notification_routes.hpp"
#include "auth_routes.hpp"

void RouteManager::setupRoutes(Pistache::Rest::Router& router) {
    AdminRoutes::setup(router);
    ArticleRoutes::setup(router);
    NotificationRoutes::setup(router);
    AuthRoutes::setup(router);
}
