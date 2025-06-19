#ifndef ADMIN_SERVICE_HPP
#define ADMIN_SERVICE_HPP

#include <pistache/http.h>
#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <string>

class AdminService {
public:
    static void listSources(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    static void viewSourceDetails(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    static void updateSource(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    static void addCategory(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
};

#endif
