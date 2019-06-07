template <class T,class D>
void opc_template_controller<T,D>::addVariable(UA_Server *server,T variable){
    char* language=const_cast<char*>("en-Us");
    UA_NodeId objectId=UA_NODEID_STRING(1,ObjectName); /* get the nodeid assigned by the server */
    UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
    oAttr.displayName = UA_LOCALIZEDTEXT(language, ObjectName);
    UA_Server_addObjectNode(server, objectId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                              UA_QUALIFIEDNAME(1, ObjectName), UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
                              oAttr, NULL, &objectId);


    UA_VariableAttributes vattr = UA_VariableAttributes_default;

    vattr.description = UA_LOCALIZEDTEXT(language, MeasurementsVariableName);
    vattr.displayName = UA_LOCALIZEDTEXT(language, MeasurementsVariableName);
    vattr.dataType = VariableType.typeId;
    vattr.valueRank = UA_VALUERANK_SCALAR;
    //UA_Variant_setScalar(&vattr.value, &variable, &VariableType);

    UA_Server_addVariableNode(server, UA_NODEID_STRING(1, MeasurementsVariableName),
                              objectId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                              UA_QUALIFIEDNAME(1, MeasurementsVariableName),
                              VariableType.typeId, vattr, this, nullptr);

    UA_VariableAttributes statusAttr = UA_VariableAttributes_default;
    UA_Boolean status = false;
    UA_Variant_setScalar(&statusAttr.value, &status, &UA_TYPES[UA_TYPES_BOOLEAN]);
    statusAttr.displayName = UA_LOCALIZEDTEXT(language,  StatusVariableName);
    statusAttr.description = UA_LOCALIZEDTEXT(language,  StatusVariableName);
    UA_Server_addVariableNode(server, UA_NODEID_STRING(1, StatusVariableName),
                                  objectId,
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                                  UA_QUALIFIEDNAME(1,  StatusVariableName),
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), statusAttr, nullptr, nullptr);

}

template <class T,class D>
void opc_template_controller<T,D>::updateVariable(UA_Server *server){
    if(isConnected()){
       auto now =this->getMeasurements();
        UA_Variant value;
        UA_Variant_setScalar(&value, &now, &VariableType);
        UA_NodeId NodeId = UA_NODEID_STRING(1, MeasurementsVariableName);
        UA_Server_writeValue(server, NodeId, value);
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Variable updated");
    }
    else{
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Updating failed, physical device disconneted");
    }
}
template <class T,class D>
void opc_template_controller<T,D>::addMonitoredItem(UA_Server *server) {
    UA_NodeId NodeId = UA_NODEID_STRING(1, MeasurementsVariableName);
    //UA_NodeId NodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_CURRENTTIME);
    UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(NodeId);
    monRequest.requestedParameters.samplingInterval = 500.0; /* 100 ms interval */
    UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, nullptr, dataChangeNotificationCallback);
}

template <class T,class D>
void opc_template_controller<T,D>::dataChangeNotificationCallback(UA_Server *server, UA_UInt32 monitoredItemId,
                              void *monitoredItemContext, const UA_NodeId *nodeId,
                              void *nodeContext, UA_UInt32 attributeId,
                              const UA_DataValue *value) {
   UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Received Notification");
}

template <class T,class D>
bool opc_template_controller<T,D>::isConnected(){
    return AbstractDeviceController<D>::device->isConnected();
}
