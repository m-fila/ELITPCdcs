#include "DCSEvent.h"
#include <iostream>
void DCSEvent::setSeverity(uint severity) {
  UA_QualifiedName qName = UA_QUALIFIEDNAME_ALLOC(0, "Severity");
  UA_Server_writeObjectProperty_scalar(server, eventId, qName, &severity,
                                       &UA_TYPES[UA_TYPES_UINT16]);
  UA_QualifiedName_deleteMembers(&qName);
}

void DCSEvent::setTime(UA_DateTime time) {
  UA_QualifiedName qName = UA_QUALIFIEDNAME_ALLOC(0, "Time");
  UA_Server_writeObjectProperty_scalar(server, eventId, qName, &time,
                                       &UA_TYPES[UA_TYPES_DATETIME]);
  UA_QualifiedName_deleteMembers(&qName);
}

void DCSEvent::setTimeNow() { return setTime(UA_DateTime_now()); }

void DCSEvent::setMessage(std::string message) {
  UA_QualifiedName qName = UA_QUALIFIEDNAME_ALLOC(0, "Message");
  UA_LocalizedText text = UA_LOCALIZEDTEXT_ALLOC("en-Us", message.c_str());
  UA_Server_writeObjectProperty_scalar(server, eventId, qName, &text,
                                       &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);
  UA_LocalizedText_deleteMembers(&text);
  UA_QualifiedName_deleteMembers(&qName);
}

void DCSEvent::setSourceName(std::string sourceName) {
  UA_QualifiedName qName = UA_QUALIFIEDNAME_ALLOC(0, "SourceName");
  UA_String text = UA_STRING_ALLOC(sourceName.c_str());
  UA_Server_writeObjectProperty_scalar(server, eventId, qName, &text,
                                       &UA_TYPES[UA_TYPES_STRING]);
  UA_String_deleteMembers(&text);
  UA_QualifiedName_deleteMembers(&qName);
}

void DCSEvent::trigger() {
  UA_Server_triggerEvent(server, eventId, sourceNode, NULL, UA_FALSE);
}

DCSEvent::DCSEvent(UA_Server *server, UA_NodeId sourceNode)
    : server(server), sourceNode(sourceNode) {
  UA_NodeId typeId = UA_NODEID_STRING_ALLOC(0, "DCSEvent");
  UA_StatusCode retval = UA_Server_createEvent(server, typeId, &eventId);
  if (retval != UA_STATUSCODE_GOOD) {
    //  UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_SERVER,
    //                 "createEvent failed. StatusCode %s",
    //                 UA_StatusCode_name(retval));
    //  throw std::runtime;
  }
  UA_NodeId_deleteMembers(&typeId);
  setTimeNow();
}

DCSEvent::~DCSEvent() { UA_Server_deleteNode(server, eventId, true); }