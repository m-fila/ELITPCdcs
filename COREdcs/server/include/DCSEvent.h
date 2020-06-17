#ifndef DCS_EVENT_H
#define DCS_EVENT_H
#include <string>
#include <open62541/types.h>
#include <open62541/server.h>

class DCSObject;
class DCSServer;

class DCSEvent {
  friend DCSServer;
  friend DCSObject;
public:
    void setSeverity(uint severity);
  //  uint getSeverity();
    void setMessage(std::string message);
 //   std::string getMessage();
    void setTime(UA_DateTime time);
    void setTimeNow();
 //   UA_DateTime getTime();
    void setSourceName(std::string sourceName);
//  std::string getSourceName();
    void trigger();
    ~DCSEvent();
private:
  DCSEvent(UA_Server *server, UA_NodeId sourceNode);
  
  UA_NodeId eventId;
  UA_Server* server;
  UA_NodeId sourceNode;
};

#endif // DCS_EVENT_H