# Learn & Code

#### FINAL PROJECT

**News Aggregation**

#### DESCRIPTION

The **News Aggregation** project consists of a Server and a Client application. The Server application
collects news feeds from various sources via API calls, processes and sorts the received JSON
responses, and provides structured news data when requested by the Client application. The Client
application features user authentication, allowing users to log in, browse news content, and save
specific articles. The Server application stores saved news and user data in a database, ensuring
efficient user management and content retrieval.

#### USER ROLES

1. Admin
2. User

#### APPLICATIONS

**1. Server Application**
    **a.** The server application should receive the news headlines items from different
       external servers and store them in the DB periodically (for every 3-4 hours) (Refer to
       the External APIs section).
    **b.** The server application should be responsible for serving the news items to the client
       application using REST APIs.
    **c.** The server application should manage the connection with the DB and perform the
       query to read the data.
    **d.** The server application should manage the user authentication from the client
       application.
    **e.** The server application should handle the user data and news data management in
       the DB.
    **f.** The server application should send news articles in the email to the user's email.


```
g. If any news item does not have the category, then the server application should
identify the news item category and store in the DB, based in the words in the news
article.
```
**2. Client Application**
    **a.** The client application should handle the user login (Admin/User).
    **b.** The client application should have a feature to create a user with the following basic
       information
          **i.** Username (name of the user)
**ii.** email (*email input validation is a must)
**iii.** password
    **c.** The client application should have a feature to validate whether the signed-up user
       already exists or not.
    **d.** Based on the email and password login, the menu should be different for the Admin
       and User accounts.
    **e.** The user should configure the notifications in the client application, and the server
       application should send the notifications to the configured user.
    **f.** The user should be able to configure keywords in order to receive notifications and
       emails about relevant news articles.

#### APPLICATION FLOW

**Application Login**
The client application should handle the user login (Admin/User).

1. The client application should initiate the user login and ask to enter the username and
    password.
2. The Sign up option should initiate the user registration into the application. It has to ask for
    username, email, and password, the client application should validate the email, username,
    and password patterns and create a user (the server application should validate the
    inputted values and make sure there are no existing users with the same values)
3. On selection of 3, the application should exit.

**Admin Menu**

On Admin user login, the application should show the admin menu.

## Welcome to the News Aggregator application. Please choose the

## options below.

## 1. Login

## 2. Sign up

## 3. Exit

## 1. View the list of external servers and status

## 2. View the external server’s details

## 3. Update/Edit the external server’s details

## 4. Add new News Category

## 5. Logout


On selection of 1, the application should show the external server’s status

Upon selection of 2, the application should show the details of the external servers.

On selection of 3, the application should show the options to update/edit the external server’s
details

In selection 4, the admin can add a new category to the application.
In selection 5 , the application should log out the user and be redirected to the home menu.

**User login**

On normal user login, the application should show the user menu.

On selection of 1 ( **Headlines** ), the console should show the sub-menu for Headlines

## List of external servers:

## 1. News API - Not Active - last accessed: 21 Mar 2025

## 2. The News API - Active - last accessed: 21 Mar 2025

## List of external server details:

## 1. News API - <API KEY>

## 2. The News API - <API KEY>

## Update/Edit the external server’s details

## Enter the external server ID

## 1

## Enter the updated API key

## <UPDATED_API_KEY>

## Welcome to the News Application, Suresh! Date: 22-Mar- 2025

## Time:1:56PM

## Please choose the options below

## 1. Headlines

## 2. Saved Articles

## 3. Search

## 4. Notifications

## 5. Logout


On the selection of 1 ( **Today** ), it should show all the news articles on that day (based on the system
date)

On the selection of 2 ( **Date range** ), it should show the menu to select the start and end date, and
then it should show the following menu to show the news articles.

- Selecting 1 should display all the news headline articles for the day.
- Selecting 2 should show the day's Business news headlines.
- Selecting 3 should show the day's Entertainment news headlines.
- Selecting 4 should show the day's Sports news headlines.
- Selecting 5 should show the day's Technology news headlines.

## Welcome to the News Application, Suresh! Date: 22-Mar- 2025

## Time:1:56PM

## Please choose the options below

## 1. Today

## 2. Date range

## 3. Logout

## Welcome to the News Application, Suresh! Date: 22-Mar- 2025

## Time:1:56PM

## Please choose the options below for Headlines

## 1. All

## 2. Business

## 3. Entertainment

## 4. Sports

## 5. Technology


- On selection of Back, it should show the previous menu
- On selection of Logout, the user should be logged out of the application.
- On selecting Save Article, the application should ask for the article ID, and after entering the
    article ID, the news article should be saved.

On selection of 2( **Saved Articles** ), the console should show the list of saved articles.

## Welcome to the News Application, Suresh! Date: 22-Mar-2025 Time:

## 1:56PM

## H E A D L I N E S

## 1. Back

## 2. Logout

## 3. Save Article

## Article Id: 123

## Tesla Unusual Options Activity - Tesla (NASDAQ: TSLA)

## Deep-pocketed investors have adopted a bearish approach towards

## Tesla TSLA, and it's something market players shouldn't ignore.

## Our tracking of public options r...

## source : benzinga.com

## URL:

## https://www.benzinga.com/insights/options/25/03/44379781/tesla-

## unusual-options-activity

## Business: business


- On selection of Back, it should show the previous menu
- On selection of Logout, the user should be logged out from the application.
- On selecting Delete Article, the application should ask for the article ID and after entering
    the article ID, then the news article should be deleted from the saved list.

On selection of 3 ( **Search** ), the application should ask the search query, and by entering the text
application should show the search results.

The search feature should have a date range feature for filtering the articles and sorting the articles
to display using no.of likes and dislikes of the article.

## Welcome to the News Application, Suresh! Date: 22-Mar- 2025

## Time:1:56PM

## S A V E D

1. Back
2. Logout
3. Delete Article

## Article Id: 123 Tesla Unusual Options Activity - Tesla (NASDAQ:

## TSLA)

## Deep-pocketed investors have adopted a bearish approach towards

## Tesla TSLA, and it's something market players shouldn't ignore.

## Our tracking of public options r...

## source : benzinga.com

## URL:

## https://www.benzinga.com/insights/options/25/03/44379781/tesla-

## unusual-options-activity

## Business: business


On selection 4 ( **Notifications** ), the application should show the notifications menu.(All the
notifications should be sent as email to the registered email also.)

### Welcome to the News Application, Suresh! Date: 22-Mar-2025 Time:1:56PM

### S E A R C H

### Results for “Tesla”

### 1. Back

### 2. Logout

### 3. Save Article

### Article Id: 123 Tesla Unusual Options Activity - Tesla (NASDAQ: TSLA)

### Deep-pocketed investors have adopted a bearish approach towards Tesla

### TSLA, and it's something market players shouldn't ignore. Our tracking

### of public options r...

### source : benzinga.com

### URL: https://www.benzinga.com/insights/options/25/03/44379781/tesla-

### unusual-options-activity

### Business: business

## Welcome to News Application, Suresh! Date: 22-Mar- 2025

## Time:1:56PM

## N O T I F I C A T I O N S

## 1. View Notifications

## 2. Configure Notifications

## 3. Back

## 4. Logout


- On selection of 1 (View Notifications), the application should show all the notifications
- On selection of 2 (Configure Notifications), the application should show the configuration

## menu.

- On selection of any option, the application should show the option to enable/disable the
    notification configuration.
- On the selection of the option Keywords, the application should take the input of keywords
    from the user, and if any news article matches those words, then the server application
    should send the notifications and email to the registered user.

On selection 5, the application should log out of the user and be redirected to the home menu.

**Solution should meet**

The solution should contain the following items.

1. The server application should retrieve data through REST APIs by referencing external APIs.
2. Communication between the server and client must be implemented using RESTful APIs.
    All data exchange should follow standard REST methods such as GET, POST, PUT, and DELETE.
3. Relational / No SQL Database (Explain why you chose the Relational/ No SQL database)
4. Multi-threading
5. SOLID Principles
6. Unit and System test cases
7. Layered Architecture
8. API documentation
9. All Clean code principles

## Welcome to the News Application, Suresh! Date: 22-Mar- 2025

## Time:1:56PM

## C O N F I G U R E - N O T I F I C A T I O N S

## 1. Business - Enabled

## 2. Entertainment - Enabled

## 3. Sports - Disabled

## 4. Technology - Disabled

## 5. Keywords - Enabled

## 6. Back

## 7. Logout

## Enter your option


**External APIs**

Here are the external servers/resources for news items.

- NewsAPI (https://newsapi.org/)
- The News API (https://www.thenewsapi.com/documentation)

Before starting the application development, please sign up with the above websites and create
the API key to access the news content.


Once you created the API key from both websites, make sure you are getting the API response for
the below APIs with the API key.

**API Details**

**News API**

- API for Headlines
- API end point
    o API Type - GET
    o URL - https://newsapi.org/v2/top-
       headlines?country=us&category=business&apiKey=<API_KEY>
- Reference - https://newsapi.org/

**The News API**

- API for Headlines
- API end point
    o API Type – GET
    o URL - https://api.thenewsapi.com/v1/news/top?api_token=<API_KEY>
       &locale=us&limit=
- Reference - https://www.thenewsapi.com/documentation

**Firebase API**

- API for Headlines (test data)
- API end point
    o API Type - GET
    o URL - https://us-central1-symbolic-gift-
       98004.cloudfunctions.net/newsapi?country=us&category=business
    o Headers


```
§ api-key - af3ce09176fb4fd3be6fcfd1e000776c
```
**API Reference**

https://newsapi.org/

https://www.thenewsapi.com/documentation


