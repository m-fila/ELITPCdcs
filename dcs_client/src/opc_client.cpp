#include "opc_client.h"

opc_client::opc_client()
{
    client = UA_Client_new();
    config= UA_Client_getConfig(client);
    UA_ClientConfig_setDefault(config);


}
opc_client::~opc_client(){
    UA_Client_delete(client);
}

int opc_client::init(){
    UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://localhost:6666");
    return retval;
}

void opc_client::go(){
    hmp_variable *a=new hmp_variable("123");
    a->data=new_HMPMeasurements();
    variables.push_back(a);
    std::vector<abstract_variable*>::iterator it;
    for(it = variables.begin(); it != variables.end(); ++it) {
    std::cout<<(*it)->translateValue()<<std::endl;
    std::cout<<(*it)->translateName()<<std::endl;
    }
    std::cout<<HMP_members[0].memberName;
}
