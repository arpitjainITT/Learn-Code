#ifndef NOTIFICATION_ROUTES_HPP
#define NOTIFICATION_ROUTES_HPP

#include <pistache/endpoint.h>
#include <pistache/router.h>

void register_notification_routes(Pistache::Rest::Router& router);

#endif
