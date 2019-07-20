#ifndef DTCONTROLLER_H
#define DTCONTROLLER_H

#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include "../DeviceController.h"
#include "../hw/DT1415ET.h"
#include "opc_controller.h"
#include "../../../common/DTmDataType.h"
#include "../../../common/DTcDataType.h"

class HMPController :public opc_template_controller<DTMeasurements,DTConfiguration,DT1415ET>
{
public:
    HMPController(std::string name);
    ~HMPController();

    dtm_customType customTypeM;
    dtc_customType customTypeC;
/*
    //HMP specific methods:
    static UA_StatusCode SetOutputCallback(UA_Server *server,
                             const UA_NodeId *sessionId, void *sessionHandle,
                             const UA_NodeId *methodId, void *methodContext,
                             const UA_NodeId *objectId, void *objectContext,
                             size_t inputSize, const UA_Variant *input,
                             size_t outputSize, UA_Variant *output);
    void addSetOutputMethod(UA_Server *server);
*/
    /*
    static UA_StatusCode SetChannelCallback(UA_Server *server,
                             const UA_NodeId *sessionId, void *sessionHandle,
                             const UA_NodeId *methodId, void *methodContext,
                             const UA_NodeId *objectId, void *objectContext,
                             size_t inputSize, const UA_Variant *input,
                             size_t outputSize, UA_Variant *output);
    void addSetChannelMethod(UA_Server *server);
*/
private:
    DTMeasurements getMeasurements();
    DTConfiguration getSettings();

};



#endif // DTCONTROLLER_H
