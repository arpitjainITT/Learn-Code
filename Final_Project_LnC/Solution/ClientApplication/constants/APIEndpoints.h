#pragma once

#define BASE_URL "http://localhost:9080"

// --- Auth ---
#define LOGIN_ENDPOINT "/api/login"
#define SIGNUP_ENDPOINT "/api/signup"

// --- Admin ---
#define ADMIN_LIST_SOURCES "/api/admin/listSources"
#define ADMIN_VIEW_SOURCE "/api/admin/viewSourceDetails"
#define ADMIN_UPDATE_SOURCE "/api/admin/updateSource" 
#define ADMIN_ADD_CATEGORY "/api/admin/addCategory"

// --- Articles ---
#define ARTICLES_ALL "/api/articles"
#define ARTICLES_BY_CATEGORY "/api/articles/byCategory" 
#define ARTICLE_BY_ID "/api/articles" 
#define ARTICLE_SAVE "/api/articles/save"
#define ARTICLE_SAVED "/api/articles/saved"            
#define ARTICLE_DELETE_SAVED "/api/articles/saved"       

// --- Notifications ---
#define NOTIFICATION_CONFIGURE "/api/notifications/configure"
#define NOTIFICATION_VIEW "/api/notifications"
