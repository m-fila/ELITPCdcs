#include "include/opcQObject.h"

opcQObject::opcQObject(std::string OName, QObject *parent) : QObject(parent), ObjectName(OName){
    ObjectNodeId=UA_NODEID_STRING_ALLOC(1,ObjectName.c_str());
}

opcQObject::~opcQObject(){
    UA_NodeId_deleteMembers(&ObjectNodeId);
}

void opcQObject::addMonitoredItem(UA_Client *Client, UA_ClientConfig *Config,
                                    UA_CreateSubscriptionResponse response){
    client=Client;
    config=Config;
}
