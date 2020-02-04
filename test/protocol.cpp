#include <iostream>
#include "DeviceController.h"
#include "GenericDevice.h"
#include <time.h>
#include <cstdlib>
#include <algorithm>
#include <sstream>
class TestDevice : public GenericDevice{
    public:
    TestDevice():GenericDevice(ConnectionType::TCP,ConnectionType::TCP), database(""),user(""), password(""), time("ns")
    {       
    }

    std::string query(std::string q, const std::string &db, const std::string &u, const std::string &p, const std::string &epoch){
        std::replace( q.begin(), q.end(), ' ', '+');
        std::stringstream ss;
        ss<<"GET "<<"/query?"<<pre(db,u,p)<<"&epoch="<<epoch<<"&q="<<q<<" HTTP/1.0\r\n\r\n";
        return sendWithResponse(ss.str());
    }

    std::string query(std::string q){
        return query(q,database,user,password,time);
    }

    std::string write(const  std::string &content, const std::string &db, const std::string &u, const std::string &p,const std::string &precision){
        std::stringstream ss;
        ss<<"POST "<<"/write?"<<pre(db,u,p)<<"&precision="<<precision<<" HTTP/1.0\r\nContent-Length: "<<content.length()<<"\r\n\r\n"<<content;
        return sendWithResponse(ss.str());
    }

    std::string write(std::string content){
        return write(content,database,user,password,time);
    }
    inline void setDatabase(std::string i){database=i;}
    inline void setUser(std::string i){user=i;}
    inline void setPassword(std::string i){password=i;}
    inline void setTime(std::string i){time=i;}
    private:
    std::string database,user,password,time;
    std::string pre(const std::string &db, const std::string &u="", const std::string &p=""){
    std::stringstream ss;
    ss<<"db="<<db<<"&u="<<u<<"&p="<<p;
    return ss.str();
    }
};

class TestController: public AbstractDeviceController<TestDevice> {
    public:

    void print(TCPConnectionParameters *p){
       device->setDatabase("test");
        connect(p);
        std::string s;
       /* while(true){
       // s=device->receive()
       std::string v="foo n="+to_string(rand()%100);
        s=device->sendWithResponse("POST http://localhost:8086/write?db=test HTTP/1.0\r\nContent-Length: "+to_string(v.length())+"\r\n\r\n"+v);
        //std::cout<<(s)<<std::endl;
        sleep(1);
        }*/
        //std::cout<<"reconnect!!!!!!!!!!!"<<std::endl;
        //connect(p);
        device->write("foo n=100");
        connect(p);
        s=device->query("SELECT * from foo");
       //s=device->sendWithResponse("GET /query?db=test&q=SELECT+*+from+foo HTTP/1.0\r\n\r\n");//std::cout<<(s)<<std::endl;
        std::cout<<(s)<<std::endl;
        //for (auto i : s)
           
        }
};

int main(int argc, char** argv){
    srand(time(NULL));
    TCPConnectionParameters p;
    p.IPaddress="localhost";
    p.port=8086;
 //   p.IPaddress="192.168.168.55";
 //   p.port=2244;
    TestController t;
    t.print(&p);
    return 0;
}
