template <class T,class D>
void opc_template_controller<T,D>::updateMeasurements(UA_Server *server){
    if(isConnected()){
        T now =getMeasurements();
        UA_Variant value;
        UA_Variant_setScalar(&value, &now, &VariableType);
        UA_NodeId NodeId = UA_NODEID_STRING_ALLOC(1, MeasurementsVariableName.c_str());
        UA_Server_writeValue(server, NodeId, value);
        UA_NodeId_deleteMembers(&NodeId);
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Measurements updated");
    }
    else{
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Updating measurements failed, physical device disconnected");
    }
}

template <class T,class D>
void opc_template_controller<T,D>::updateConfiguration(UA_Server *server){
    if(isConnected()){
        T now =getSettings();
        UA_Variant value;
        UA_Variant_setScalar(&value, &now, &VariableType);
        UA_NodeId NodeId = UA_NODEID_STRING_ALLOC(1, ConfigurationVariableName.c_str());
        UA_Server_writeValue(server, NodeId, value);
        UA_NodeId_deleteMembers(&NodeId);
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Configuration updated");
    }
    else{
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Updating configuration failed, physical device disconnected");
    }
}


template <class T,class D>
void opc_template_controller<T,D>::updateStatus(UA_Server *server){

        UA_Boolean now =isConnected();
        UA_Variant value;
        UA_Variant_setScalar(&value, &now, &UA_TYPES[UA_TYPES_BOOLEAN]);
        UA_NodeId NodeId = UA_NODEID_STRING_ALLOC(1, StatusVariableName.c_str());
        UA_Server_writeValue(server, NodeId, value);
        UA_NodeId_deleteMembers(&NodeId);
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Status updated");
}



template <class T,class D>
bool opc_template_controller<T,D>::isConnected(){
    return AbstractDeviceController<D>::device->connected;
}
