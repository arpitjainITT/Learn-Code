#ifndef AUTH_ROUTES_HPP
#define AUTH_ROUTES_HPP

#include <pistache/router.h>

class AuthRoutes {
public:
    static void setup(Pistache::Rest::Router& router);
};

#endif 
