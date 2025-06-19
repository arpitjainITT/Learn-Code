#ifndef AUTH_ROUTES_HPP
#define AUTH_ROUTES_HPP

#include <pistache/endpoint.h>
#include <pistache/router.h>

void register_auth_routes(Pistache::Rest::Router& router);

#endif
