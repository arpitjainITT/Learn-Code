#ifndef NOTIFICATION_ROUTES_HPP
#define NOTIFICATION_ROUTES_HPP

#include <pistache/router.h>

class NotificationRoutes {
public:
    static void setup(Pistache::Rest::Router& router);
};

#endif 
