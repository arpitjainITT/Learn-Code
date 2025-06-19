#ifndef ARTICLE_SERVICE_HPP
#define ARTICLE_SERVICE_HPP

#include <pistache/http.h>
#include <pistache/endpoint.h>
#include <pistache/router.h>

class ArticleService {
public:
    static void getAllArticles(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    static void getArticlesByCategory(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    static void getArticleById(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    
    static void saveArticle(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    static void getSavedArticles(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    static void deleteSavedArticle(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

    static void fetchAndStore(); // for FetchScheduler
    
};

#endif


