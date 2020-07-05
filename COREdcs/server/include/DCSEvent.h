#ifndef DCS_EVENT_H
#define DCS_EVENT_H
#include <iostream>
#include <open62541/server.h>
#include <open62541/types.h>
#include <string>
class DCSObject;
class DCSServer;

class DCSEvent {
  friend DCSServer;
  friend DCSObject;

public:
  void setSeverity(uint severity);
  //  uint getSeverity();
  void setMessage(const std::string &message);
  //   std::string getMessage();
  void setTime(const UA_DateTime &time);
  void setTimeNow();
  //   UA_DateTime getTime();
  void setSourceName(const std::string &sourceName);
  //  std::string getSourceName();
  void trigger();
  ~DCSEvent();

private:
  DCSEvent(UA_Server *server, UA_NodeId sourceNode);

  UA_NodeId eventId;
  UA_Server *server;
  UA_NodeId sourceNode;

  UA_NodeId &getEventType();
  UA_NodeId addEventType(const char *name);
};

#endif // DCS_EVENT_H