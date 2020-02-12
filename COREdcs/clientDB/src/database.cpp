#include "database.h"
#include <iostream>
database::~database(){
 //  sqlite3_free(err_msg);
    if(opened)
        sqlite3_close(db);
}

database::database(): opened(false){
}

int database::open(std::string filename){

    int rc = sqlite3_open(filename.c_str(), &db);
    if (rc==SQLITE_OK){
        opened=true;
    }
    return rc;

}
int database::exec(std::string commands){
    if(opened==true){
        int rc = sqlite3_exec(db, commands.c_str(), nullptr, nullptr, &err_msg);
        if (rc != SQLITE_OK ) {
            std::cout<<"SQL error"<<err_msg<<std::endl;
            sqlite3_free(err_msg);
        }
        return rc;
    }
    return 1;
}
void database::createTable(std::string tablename,std::string columns){
    std::string c="CREATE TABLE IF NOT EXISTS '"+tablename+"'("+columns+");";
    exec(c);
}

void database::logData(std::string tablename, std::string data){
    std::string c="INSERT INTO '"+tablename+"' VALUES("+data+");";
    exec(c);
}
