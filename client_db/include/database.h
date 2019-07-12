#ifndef DB_H
#define DB_H
#include <sqlite3.h>
#include <string>
class database
{
public:
    database();
    ~database();
    int open(std::string filename);
    void createTable(std::string tablename,std::string columns);
    void logData(std::string tablename, std::string data);
    bool opened;

private:
    sqlite3 *db;
    char *err_msg = nullptr;
    const std::string tablename;
    int exec(std::string commands);

};

#endif // DB_H
