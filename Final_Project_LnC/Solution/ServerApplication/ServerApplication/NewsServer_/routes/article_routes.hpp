#ifndef ARTICLE_ROUTES_HPP
#define ARTICLE_ROUTES_HPP

#include <pistache/router.h>

class ArticleRoutes {
public:
    static void setup(Pistache::Rest::Router& router);
};

#endif 
