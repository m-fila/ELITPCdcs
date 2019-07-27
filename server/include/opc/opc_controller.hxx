template <class M,class C,class D>
void opc_template_controller<M,C,D>::updateMeasurementsVariable(UA_Server *server){

 //   if(isConnected()){
 //       M now =getMeasurements();
        UA_Variant value;
 //       UA_Variant_setScalar(&value, &now, &VariableTypeM);
        std::lock_guard<std::mutex> lock(mMutex);
        UA_Variant_setScalar(&value, &measurements, &VariableTypeM);
        UA_Server_writeValue(server, MeasurementsId, value);

  //  }

}

template <class M,class C,class D>
void opc_template_controller<M,C,D>::updateConfigurationVariable(UA_Server *server){

   // if(isConnected()){
  //      C now =getSettings();
        UA_Variant value;
  //      UA_Variant_setScalar(&value, &now, &VariableTypeC);
        std::lock_guard<std::mutex> lock(cMutex);
        UA_Variant_setScalar(&value, &configuration, &VariableTypeC);
        UA_Server_writeValue(server, ConfigurationId, value);

 //   }

}


template <class M,class C,class D>
void opc_template_controller<M,C,D>::updateStatusVariable(UA_Server *server){
    std::lock_guard<std::mutex> lock(deviceMutex);
    UA_Boolean now =isConnected();
    UA_Variant value;
    UA_Variant_setScalar(&value, &now, &UA_TYPES[UA_TYPES_BOOLEAN]);
    UA_Server_writeValue(server, StatusId, value);
}



//?????????????????????????????????????????????????????????????????????????????

template <class M,class C,class D>
void opc_template_controller<M,C,D>::updateMeasurements(){
    if(isConnected()){
        M now =getMeasurements();
        std::lock_guard<std::mutex> lock(mMutex);
        measurements=now;
    }
}

template <class M,class C,class D>
void opc_template_controller<M,C,D>::updateConfiguration(){
    if(isConnected()){
        C now =getSettings();
        std::lock_guard<std::mutex> lock(cMutex);
        configuration=now;

    }

}
