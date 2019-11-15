template <class M,class C,class D>
void OpcTemplateController<M,C,D>::disconnectDevice(){
    DeviceCommand<D> command=std::bind(&D::resetConnectionStream, _1);
    buffer.push(command);
}

template <class M,class C,class D>
void OpcTemplateController<M,C,D>::connectDevice(TCPConnectionParameters *cp){
    try{
    TCPStream* stream =TCPConnector::connect(cp->IPaddress.c_str(),cp->port);
    DeviceCommand<D> command=std::bind(&D::setConnectionStream, _1,stream);
    buffer.push(command);
    }
    catch(std::runtime_error e){
        //TODO: return to method or send event informing of connection fail or success 
        std::cerr<<objectName+" device controller catched: "<<e.what()<<std::endl;
    }
}


template <class M,class C,class D>
void OpcTemplateController<M,C,D>::updateMeasurementsVariable(UA_Server *server){
    UA_Variant value;
    std::lock_guard<std::mutex> lock(mMutex);
    UA_Variant_setScalar(&value, &measurements, &variableTypeM);
    UA_Server_writeValue(server, measurementsId, value);
}

template <class M,class C,class D>
void OpcTemplateController<M,C,D>::updateConfigurationVariable(UA_Server *server){
    UA_Variant value;
    std::lock_guard<std::mutex> lock(cMutex);
    UA_Variant_setScalar(&value, &configuration, &variableTypeC);
    UA_Server_writeValue(server, configurationId, value);
}


template <class M,class C,class D>
void OpcTemplateController<M,C,D>::updateStatusVariable(UA_Server *server){
    UA_Variant value;
    std::lock_guard<std::mutex> lock(sMutex);
    UA_Variant_setScalar(&value, &status, &UA_TYPES[UA_TYPES_BOOLEAN]);
    UA_Server_writeValue(server, statusId, value);
}



template <class M,class C,class D>
void OpcTemplateController<M,C,D>::update_measurements(){
    if(isConnected()){
        try{
            M now =getMeasurements();
            std::lock_guard<std::mutex> lock(mMutex);
            measurements=now;
        }
        catch(...){
            std::cerr<<objectName+" device controller encountered error when updating measurements. Disconnecting..."<<std::endl;
            device.resetConnectionStream();
        }
    }
}

template <class M,class C,class D>
void OpcTemplateController<M,C,D>::update_configuration(){
    if(isConnected()){
        try{
            C now =getSettings();
            std::lock_guard<std::mutex> lock(cMutex);
            configuration=now;
        }
        catch(...){
            std::cerr<<objectName+" device controller encountered error when updating configuration. Disconnecting..."<<std::endl;
            device.resetConnectionStream();
        }
    }
}

template <class M,class C,class D>
void OpcTemplateController<M,C,D>::update_status(){
    bool now =isConnected();
    std::lock_guard<std::mutex> lock(sMutex);
    status=now;
}

template <class M, class C, class D>
void OpcTemplateController<M,C,D>::run_thread(){
    while(true){
        if(buffer.size()){
            DeviceCommand<D> command=buffer.dequeue();
            try{
                command(device);
            }
            catch(runtime_error e ){
                std::cerr<<objectName+" device controller catched "<<e.what()<<" when running device command"<<std::endl;
                device.resetConnectionStream();
            }
        }
        else{
            //std::this_thread::sleep_for(std::chrono::milliseconds(1));
            update_status();
            if(isConnected()){
                  update_configuration();
                  update_measurements();
                  UA_sleep_ms(100);
            }
        }
        if(!thread_running){
            break;
        }
    }
}
