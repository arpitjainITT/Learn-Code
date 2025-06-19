#ifndef ROUTEMANAGER_HPP
#define ROUTEMANAGER_HPP

#include <pistache/router.h>

class RouteManager {
public:
    static void init(Pistache::Rest::Router& router);
};

#endif
