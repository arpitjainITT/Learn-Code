#ifndef USERSERVICE_HPP
#define USERSERVICE_HPP

#include <pistache/http.h>
#include <pistache/router.h>

class UserService {
public:
    static void signup(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    static void login(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
};

#endif