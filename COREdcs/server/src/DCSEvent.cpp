#include "DCSEvent.h"
void DCSEvent::setSeverity(size_t severity) {
    auto s = severity > 1000 ? 1000 : severity;
    UA_QualifiedName qName = UA_QUALIFIEDNAME_ALLOC(0, "Severity");
    UA_Server_writeObjectProperty_scalar(server, eventId, qName, &s,
                                         &UA_TYPES[UA_TYPES_UINT16]);
    UA_QualifiedName_deleteMembers(&qName);
}

void DCSEvent::setTime(const UA_DateTime &time) {
    UA_QualifiedName qName = UA_QUALIFIEDNAME_ALLOC(0, "Time");
    UA_Server_writeObjectProperty_scalar(server, eventId, qName, &time,
                                         &UA_TYPES[UA_TYPES_DATETIME]);
    UA_QualifiedName_deleteMembers(&qName);
}

void DCSEvent::setTimeNow() { return setTime(UA_DateTime_now()); }

void DCSEvent::setMessage(const std::string &message) {
    UA_QualifiedName qName = UA_QUALIFIEDNAME_ALLOC(0, "Message");
    UA_LocalizedText text = UA_LOCALIZEDTEXT_ALLOC("en-Us", message.c_str());
    UA_Server_writeObjectProperty_scalar(server, eventId, qName, &text,
                                         &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);
    UA_LocalizedText_deleteMembers(&text);
    UA_QualifiedName_deleteMembers(&qName);
}

void DCSEvent::setSourceName(const std::string &sourceName) {
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
    UA_StatusCode retval = UA_Server_createEvent(server, getEventType(), &eventId);
    if(retval != UA_STATUSCODE_GOOD) {
        //  UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_SERVER,
        //                 "createEvent failed. StatusCode %s",
        //                 UA_StatusCode_name(retval));
        //  throw std::runtime;
    }
    // UA_NodeId_deleteMembers(&typeId);
    setTimeNow();
}

DCSEvent::~DCSEvent() { UA_Server_deleteNode(server, eventId, true); }

UA_NodeId &DCSEvent::getEventType() {
    static UA_NodeId eventType = addEventType("DCSEvent");
    return eventType;
}

UA_NodeId DCSEvent::addEventType(const char *name) {
    UA_ObjectTypeAttributes attr = UA_ObjectTypeAttributes_default;
    UA_NodeId eventTypeId;
    attr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US", name);
    attr.description = UA_LOCALIZEDTEXT_ALLOC("en-US", "Generic information event");
    UA_QualifiedName qName = UA_QUALIFIEDNAME_ALLOC(1, name);
    UA_Server_addObjectTypeNode(
        server, UA_NODEID_NULL, UA_NODEID_NUMERIC(0, UA_NS0ID_BASEEVENTTYPE),
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE), qName, attr, nullptr, &eventTypeId);
    UA_ObjectTypeAttributes_deleteMembers(&attr);
    UA_QualifiedName_deleteMembers(&qName);
    return eventTypeId;
}
