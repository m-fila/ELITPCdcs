#ifndef DB_H
#define DB_H

#include <sqlite3.h>
class database
{
public:
    database();
    ~database();
    int open(){return 0;}
    int exec(){return 0;}
private:
    sqlite3 *db;
    char *err_msg = nullptr;
};

#endif // DB_H
