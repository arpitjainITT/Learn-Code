#ifndef ADMIN_ROUTES_HPP
#define ADMIN_ROUTES_HPP

#include <pistache/router.h>

class AdminRoutes {
public:
    static void setup(Pistache::Rest::Router& router);
};

#endif 
