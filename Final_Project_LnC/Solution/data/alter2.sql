SELECT a.id, a.title, a.description, a.url, a.source,c.category_type
        FROM news_article a
        JOIN saved_news s ON a.id = s.news_id
        LEFT JOIN news_article_category ac ON a.id = ac.news_id
        LEFT JOIN news_category c ON ac.category_id = c.id
        WHERE s.user_id = 2