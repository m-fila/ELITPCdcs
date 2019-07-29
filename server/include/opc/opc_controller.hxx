template <class M,class C,class D>
void opc_template_controller<M,C,D>::disconnectDevice(){
    DeviceCommand<D> command=std::bind(&D::resetConnectionStream, _1);
    buffer.push(command);
}

template <class M,class C,class D>
void opc_template_controller<M,C,D>::connectDevice(TCPConnectionParameters *cp){
    TCPStream* stream =TCPConnector::connect(cp->IPaddress.c_str(),cp->port);
    DeviceCommand<D> command=std::bind(&D::setConnectionStream, _1,stream);
    buffer.push(command);
}


template <class M,class C,class D>
void opc_template_controller<M,C,D>::updateMeasurementsVariable(UA_Server *server){
    UA_Variant value;
    std::lock_guard<std::mutex> lock(mMutex);
    UA_Variant_setScalar(&value, &measurements, &VariableTypeM);
    UA_Server_writeValue(server, MeasurementsId, value);
}

template <class M,class C,class D>
void opc_template_controller<M,C,D>::updateConfigurationVariable(UA_Server *server){
    UA_Variant value;
    std::lock_guard<std::mutex> lock(cMutex);
    UA_Variant_setScalar(&value, &configuration, &VariableTypeC);
    UA_Server_writeValue(server, ConfigurationId, value);
}


template <class M,class C,class D>
void opc_template_controller<M,C,D>::updateStatusVariable(UA_Server *server){
    UA_Variant value;
    std::lock_guard<std::mutex> lock(sMutex);
    UA_Variant_setScalar(&value, &status, &UA_TYPES[UA_TYPES_BOOLEAN]);
    UA_Server_writeValue(server, StatusId, value);
}



template <class M,class C,class D>
void opc_template_controller<M,C,D>::update_measurements(){
    if(isConnected()){
        M now =getMeasurements();
        std::lock_guard<std::mutex> lock(mMutex);
        measurements=now;
    }
}

template <class M,class C,class D>
void opc_template_controller<M,C,D>::update_configuration(){
    if(isConnected()){
        C now =getSettings();
        std::lock_guard<std::mutex> lock(cMutex);
        configuration=now;
    }
}

template <class M,class C,class D>
void opc_template_controller<M,C,D>::update_status(){
    bool now =isConnected();
    std::lock_guard<std::mutex> lock(sMutex);
    status=now;
}

template <class M, class C, class D>
void opc_template_controller<M,C,D>::run_thread(){
    while(true){
        if(buffer.size()){
            DeviceCommand<D> command=buffer.dequeue();
            command(device);
        }
        else{
            //std::this_thread::sleep_for(std::chrono::milliseconds(1));
            update_status();
            if(isConnected()){
                  update_configuration();
                  update_measurements();
            }
        }
        if(!thread_running){
            break;
        }
    }
}
