#include "../include/opc_client.h"


opc_client::opc_client()
{
    client = UA_Client_new();
    config= UA_Client_getConfig(client);
    UA_ClientConfig_setDefault(config);


}
opc_client::~opc_client(){
    UA_Client_delete(client);
}

bool opc_client::init(){
    UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://localhost:6666");
    return retval;
}

void opc_client::go(){

    hmp_variable *a=new hmp_variable("HMP2.Measurements");
    a->data=new_HMPMeasurements();
    variables.push_back(a);
    std::vector<abstract_variable*>::iterator it;
    for(it = variables.begin(); it != variables.end(); ++it) {
    std::cout<<(*it)->translateValue()<<std::endl;
    std::cout<<(*it)->translateName()<<std::endl;
    }
    std::cout<<HMP_members[0].memberName;
}

UA_Boolean opc_client::running = false;

int opc_client::run(){
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);
    running=true;
    while(running) {

            UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://localhost:6666");
            if(retval != UA_STATUSCODE_GOOD) {
                UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                             "Not connected. Retrying to connect in 1 second");
                /* The connect may timeout after 1 second (see above) or it may fail immediately on network errors */
                /* E.g. name resolution errors or unreachable network. Thus there should be a small sleep here */
                UA_sleep_ms(1000);
                continue;
            }

            UA_Client_run_iterate(client, 1000);
    }
    UA_Client_disconnect(client);
    signal(SIGINT,SIG_DFL);
    signal(SIGTERM,SIG_DFL);

    return EXIT_SUCCESS;
}
