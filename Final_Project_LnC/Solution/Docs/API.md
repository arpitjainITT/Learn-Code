# News Aggregator System — REST API Reference

**Base URL:** `http://localhost:9080`  
**Content-Type:** `application/json` (for all request bodies)

---

## Authentication

Protected endpoints require a JWT Bearer token obtained from `POST /auth/login`.

```
Authorization: Bearer <token>
```

| Icon | Meaning |
|---|---|
| 🔓 | Public — no token required |
| 🔒 | User token required |
| 🛡️ | Admin token required (`role == "admin"`) |

---

## Auth Endpoints

### `POST /auth/login` 🔓

Authenticate and receive a JWT token.

**Request body:**
```json
{
  "email": "user@example.com",
  "password": "yourpassword"
}
```

**Response `200 OK`:**
```json
{
  "status":  "success",
  "user_id": 1,
  "role":    "user",
  "token":   "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9..."
}
```

**Response `401 Unauthorized`:**
```json
{"status": "invalid credentials"}
```

> Store the `token` and include it as `Authorization: Bearer <token>` in every subsequent protected request. Tokens expire after **24 hours**.

---

### `POST /auth/signup` 🔓

Register a new user account.

**Request body:**
```json
{
  "username": "alice",
  "email":    "alice@example.com",
  "password": "securepass"
}
```

**Response `201 Created`:**
```json
{"status": "user registered"}
```

**Response `400 Bad Request`:**
```json
{"status": "registration failed"}
```

> Fails if email or username already exists. Passwords are stored as SHA-256 hashes.

---

## Article Endpoints

### `GET /articles` 🔓 *(personalized if authenticated)*

Retrieve all articles. If a valid Bearer token is present, returns a personalized feed filtered by the user's category and keyword preferences.

**Query parameters (optional, anonymous only):**
| Parameter | Type | Description |
|---|---|---|
| `userId` | integer | Personalize feed (ignored when Bearer token present) |

**Response `200 OK`:**
```json
[
  {
    "id":          1,
    "title":       "AI Breakthrough Announced",
    "description": "Researchers have developed...",
    "url":         "https://example.com/article/1",
    "source":      "Tech News",
    "category":    "Technology"
  }
]
```

---

### `GET /articles/:id` 🔓

Get a single article by its database ID.

**Response `200 OK`:** Single article object (same fields as above).

---

### `GET /articles/categories` 🔓

List all available (non-hidden) categories.

**Response `200 OK`:**
```json
["Business", "Entertainment", "Health", "Politics", "Science", "Sports", "Technology", "Travel"]
```

---

### `GET /articles/category/:category` 🔓

Filter articles by a specific category name.

**Response `200 OK`:** Array of article objects.

---

### `GET /articles/search` 🔓

Full-text search with optional date range and sort.

**Query parameters:**
| Parameter | Type | Description |
|---|---|---|
| `keyword`   | string | Search term (title, description, content) |
| `startDate` | string | Filter from date — format `YYYY-MM-DD` |
| `endDate`   | string | Filter to date — format `YYYY-MM-DD` |
| `sort`      | string | `likes`, `dislikes`, or omit for date-desc |

**Response `200 OK`:** Array of article objects with additional fields:
```json
[
  {
    "id": 5, "title": "...", "likes": 12, "dislikes": 2,
    "category": "Technology", "source": "...", "url": "..."
  }
]
```

---

### `GET /articles/:id/reactions` 🔓

Get like/dislike counts for an article.

**Response `200 OK`:**
```json
{"likes": 15, "dislikes": 3}
```

---

### `POST /articles/save` 🔒

Save an article to the authenticated user's reading list.

**Request body:** *(userId is taken from token — body only needs article_id)*
```json
{"article_id": 42}
```

**Response `200 OK`:** `"Article saved."`

---

### `DELETE /articles/save` 🔒

Remove a saved article.

**Request body:**
```json
{"articleId": 42}
```

**Response `200 OK`:** `"Article unsaved."`

---

### `GET /articles/saved/:userId` 🔒

List the authenticated user's saved articles. Users can only view their own; admins can view any.

**Response `200 OK`:** Array of article objects.

**Response `403 Forbidden`:** If `userId` in URL does not match token's user ID (non-admin).

---

### `POST /articles/:id/like` 🔒

Like an article. One reaction per user per article (upsert — switches from dislike to like if needed).

**Request body:** *(empty body is fine — userId comes from token)*
```json
{}
```

**Response `200 OK`:** `"Article liked."`

---

### `POST /articles/:id/dislike` 🔒

Dislike an article.

**Response `200 OK`:** `"Article disliked."`

---

### `POST /articles/:id/report` 🔒

Report an article. After **5 reports** the article is automatically hidden.

**Request body:**
```json
{"reason": "Misleading information"}
```

**Response `200 OK`:**
```json
{"status": "ok", "message": "Article reported."}
```

---

## Admin Endpoints

> All admin endpoints require a valid JWT with `role == "admin"`.  
> Response `401` if token missing/invalid. Response `403` if role is not admin.

### `GET /admin/servers` 🛡️

List all configured external news API servers.

**Response `200 OK`:**
```json
[
  {
    "id": 1, "server_name": "TheNewsAPI",
    "api_url": "https://api.thenewsapi.com/v1/news/top?api_token=",
    "api_key": "YOUR_KEY", "status": "active",
    "last_accessed": "2025-08-23T10:00:00"
  }
]
```

---

### `GET /admin/servers/:id` 🛡️

Get details of a specific server by ID.

---

### `PUT /admin/servers/:id/key` 🛡️

Update the API key for a news server.

**Request body:**
```json
{"api_key": "new-api-key-here"}
```

**Response `200 OK`** on success, `400` on failure.

---

### `PUT /admin/servers/:id/status` 🛡️

Activate or deactivate a news server.

**Request body:**
```json
{"status": "active"}
```

Values: `"active"` | `"inactive"`

---

### `POST /admin/category` 🛡️

Add a new article category.

**Request body:**
```json
{"category": "Cryptocurrency"}
```

**Response `201 Created`** on success.

---

### `POST /admin/categories/hide` 🛡️

Hide a category. Articles in hidden categories are excluded from all feeds.

**Request body:**
```json
{"category": "Sports"}
```

---

### `POST /admin/categories/unhide` 🛡️

Un-hide a category.

---

### `GET /admin/hidden-categories` 🛡️

List all currently hidden categories.

**Response `200 OK`:**
```json
["Sports", "Entertainment"]
```

---

### `POST /admin/keywords/filter` 🛡️

Add a filtered keyword. Articles whose title/description/content match this keyword are excluded from all feeds.

**Request body:**
```json
{"keyword": "advertisement"}
```

---

### `POST /admin/keywords/unfilter` 🛡️

Remove a filtered keyword.

---

### `GET /admin/filtered-keywords` 🛡️

List all active filtered keywords.

---

### `GET /admin/reported-articles` 🛡️

List all articles that have received at least one report.

**Response `200 OK`:**
```json
[
  {"id": 12, "title": "...", "report_count": 3, "is_hidden": 0, "num_reports": 3}
]
```

---

### `POST /admin/articles/:id/hide` 🛡️

Manually hide an article (removes it from all feeds).

---

### `POST /admin/articles/:id/unhide` 🛡️

Un-hide an article.

---

## Notification Endpoints

> All notification endpoints require a valid JWT. Users can only access their own data; admins can access any user's data.

### `POST /notifications/category` 🔒

Enable or disable notifications for a category.

**Request body:**
```json
{
  "category": "Technology",
  "enabled":  true
}
```

**Response `200 OK`:** `"Category preference updated."`

---

### `POST /notifications/keyword` 🔒

Enable or disable keyword-based notifications.

**Request body:**
```json
{
  "keyword": "Elon Musk",
  "enabled": true
}
```

**Response `200 OK`:** `"Keyword preference updated."`

---

### `GET /notifications/preferences/:userId` 🔒

Get the user's current notification preferences.

**Response `200 OK`:**
```json
{
  "categories": [
    {"category": "Technology", "enabled": true},
    {"category": "Sports",     "enabled": false}
  ],
  "keywords": [
    {"keyword": "AI", "enabled": true}
  ]
}
```

---

### `GET /notifications/:userId` 🔒

Get all delivered notifications for the user.

**Response `200 OK`:**
```json
[
  {
    "id":          1,
    "title":       "New AI breakthrough article",
    "description": "New article in category: Technology",
    "category":    "Technology",
    "url":         "https://example.com/article/5",
    "source":      "Tech News",
    "read":        false
  }
]
```

---

### `POST /notifications/markread/:userId` 🔒

Mark all notifications as read for the user.

**Response `200 OK`:** `"Notifications marked as read."`

---

## HTTP Status Codes Used

| Code | Meaning |
|---|---|
| 200 | Success |
| 201 | Resource created (signup, add category) |
| 400 | Bad request (invalid input, registration failed) |
| 401 | Unauthorized — missing or invalid JWT token |
| 403 | Forbidden — valid token but insufficient role/ownership |
| 500 | Internal server error |
