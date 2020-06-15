#include "ConnectionStream.h"
#include <iostream>

ConnectionStream::ConnectionStream()
{

}

ConnectionStream::~ConnectionStream()
{
    close(m_sd);
}

ssize_t ConnectionStream::send(char* buffer, size_t len)
{
    ssize_t s=write(m_sd, buffer, len);
    if(s==-1)
        throw std::runtime_error("Encountered error when writing to stream");
    return s;
}

ssize_t ConnectionStream::send(std::string s)
{
    return send((char*)s.c_str(),s.length());
}

ssize_t ConnectionStream::receive(char* buffer, size_t len)
{
    ssize_t s=read(m_sd, buffer, len);
    if(s==-1)
        throw std::runtime_error("Encountered error when writing to stream");
    return s;
}

std::string ConnectionStream::receive()
{
    std::string response;
    ssize_t len;
    do{
        char buffer[1024];
        len=ConnectionStream::receive(buffer,1024);
        response.append(buffer, len);
      //  std::fill(&buffer[0], &buffer[0]+len, 0);
    }
    while(len==1024);
    if (!response.empty() && response[response.length()-1] == '\n') {
        response.erase(response.length()-1);
    }   //remove new line from answer
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
