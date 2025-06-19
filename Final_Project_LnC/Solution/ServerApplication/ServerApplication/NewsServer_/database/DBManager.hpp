#ifndef DBMANAGER_HPP
#define DBMANAGER_HPP

#include <sqlite3.h>
#include <string>

class DBManager {
public:
    static void init(const std::string& dbPath);
    static sqlite3* getDB();
private:
    static sqlite3* db;
};

#endif 