#ifndef CONNECTIONSTREAM_H
#define CONNECTIONSTREAM_H

#include <unistd.h>
#include <memory>
#include <string>

using namespace std;

class ConnectionStream
{
public:
    ConnectionStream();
    virtual ~ConnectionStream();

    virtual ssize_t send(char* buffer, size_t len);
    virtual ssize_t send(string s);
    virtual ssize_t receive(char* buffer, size_t len);
    virtual string receive();
    virtual string sendWithResponse(string s);

protected:
    ConnectionStream(int sd) : m_sd(sd) {}
    int     m_sd;   //connection descriptor
};

typedef std::unique_ptr<ConnectionStream> ConnectionStreamPtr;

enum class ConnectionType
{
    None = 0x00,
    TCP = 0x01,
    Serial = 0x02,
    USB = 0x04,
    Unknown = 0xFF
};

constexpr ConnectionType operator |(ConnectionType a, ConnectionType b)
{
    return static_cast<ConnectionType>(static_cast<int>(a)|static_cast<int>(b));
}

constexpr ConnectionType operator &(ConnectionType a, ConnectionType b)
{
    return static_cast<ConnectionType>(static_cast<int>(a)&static_cast<int>(b));
}

#endif // CONNECTIONSTREAM_H
