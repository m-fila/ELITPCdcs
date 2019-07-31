#include "../../include/utl/ConnectionStream.h"
#include <iostream>

ConnectionStream::ConnectionStream()
{

}

ConnectionStream::~ConnectionStream()
{
    std::cout << "Closing connection in destructor ..." << std::endl;
    close(m_sd);
}

ssize_t ConnectionStream::send(char* buffer, size_t len)
{
    //TODO: throw exception when write failed
    return write(m_sd, buffer, len);
}

ssize_t ConnectionStream::send(std::string s)
{
    //std::cout << s << std::endl;
    return send((char*)s.c_str(),s.length());
}

ssize_t ConnectionStream::receive(char* buffer, size_t len)
{
    //TODO: throw exception when read failed
    return read(m_sd, buffer, len);
}

std::string ConnectionStream::receive()
{
    char buffer[1024];
    ssize_t len;
    len=ConnectionStream::receive(buffer,1024);
    std::string response;
    response.append(buffer, len);
    response.erase(response.length()-1); //remove new line from answer
    return response;
}

std::string ConnectionStream::sendWithResponse(std::string s)
{
    send(s);
//    std::string r=receive();
//    std::cout<<s<<std::endl;
 //   std::cout<<r<<std::endl;
    return receive();
}
