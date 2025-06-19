#include "admin_routes.hpp"
#include "../services/AdminService.hpp"

using namespace Pistache;

void register_admin_routes(Rest::Router& router) {
    
    Rest::Routes::Get(router, "/admin/sources", Rest::Routes::bind(&AdminService::listSources));

    Rest::Routes::Get(router, "/admin/source-details", Rest::Routes::bind(&AdminService::viewSourceDetails));

    Rest::Routes::Put(router, "/admin/source/:id", Rest::Routes::bind(&AdminService::updateSource));

    Rest::Routes::Post(router, "/admin/category", Rest::Routes::bind(&AdminService::addCategory));
}
