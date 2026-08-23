#ifndef DBMANAGER_HPP
#define DBMANAGER_HPP

#include <sqlite3.h>
#include <string>

class DBManager {
public:
    static DBManager& getInstance();                  
    ~DBManager();                                     

    bool initializeDB(const std::string& dbPath);     
    sqlite3* getDB() const { return db; }            
    void close();                                  

private:
    DBManager() = default;                           
    DBManager(const DBManager&) = delete;
    DBManager& operator=(const DBManager&) = delete;

    bool executeSchema();                            

    sqlite3* db = nullptr;
};

#endif 
