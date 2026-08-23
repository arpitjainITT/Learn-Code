#ifndef SAVEDARTICLESERVICE_H
#define SAVEDARTICLESERVICE_H

#include "../models/Article.h"
#include <vector>

class SavedArticleService {
public:
    static std::vector<Article> getSavedArticles(int userId);
    static void deleteSavedArticle(int userId, int articleId);
};

#endif 
