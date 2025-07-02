#ifndef ROUTE_MANAGER_HPP
#define ROUTE_MANAGER_HPP

#include <pistache/router.h>

class RouteManager {
public:
    static void setupRoutes(Pistache::Rest::Router& router);
};

#endif 
