#ifndef DB_H
#define DB_H
#include <sqlite3.h>
#include <string>
class database
{
public:
    database();
    ~database();
    int open(){return 0;}
    void createTable();
    int exec(){return 0;}
private:
    sqlite3 *db;
    char *err_msg = nullptr;
};

#endif // DB_H
