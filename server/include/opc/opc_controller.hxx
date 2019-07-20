template <class M,class C,class D>
void opc_template_controller<M,C,D>::updateMeasurements(UA_Server *server){
    if(isConnected()){
        M now =getMeasurements();
        UA_Variant value;
        UA_Variant_setScalar(&value, &now, &VariableTypeM);
        UA_NodeId NodeId = UA_NODEID_STRING_ALLOC(1, MeasurementsVariableName.c_str());
        UA_Server_writeValue(server, NodeId, value);
        UA_NodeId_deleteMembers(&NodeId);
 //       UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Measurements updated");
    }
    else{
 //       UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Updating measurements failed, physical device disconnected");
    }
}

template <class M,class C,class D>
void opc_template_controller<M,C,D>::updateConfiguration(UA_Server *server){
    if(isConnected()){
        C now =getSettings();
        UA_Variant value;
        UA_Variant_setScalar(&value, &now, &VariableTypeC);
        UA_NodeId NodeId = UA_NODEID_STRING_ALLOC(1, ConfigurationVariableName.c_str());
        UA_Server_writeValue(server, NodeId, value);
        UA_NodeId_deleteMembers(&NodeId);
  //      UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Configuration updated");
    }
    else{
  //      UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Updating configuration failed, physical device disconnected");
    }
}


template <class M,class C,class D>
void opc_template_controller<M,C,D>::updateStatus(UA_Server *server){

        UA_Boolean now =isConnected();
        UA_Variant value;
        UA_Variant_setScalar(&value, &now, &UA_TYPES[UA_TYPES_BOOLEAN]);
        UA_NodeId NodeId = UA_NODEID_STRING_ALLOC(1, StatusVariableName.c_str());
        UA_Server_writeValue(server, NodeId, value);
        UA_NodeId_deleteMembers(&NodeId);
  //      UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Status updated");
}
