#ifndef DTCONTROLLER_H
#define DTCONTROLLER_H

//#include <open62541/plugin/log_stdout.h>
//#include <open62541/server.h>
#include "../DeviceController.h"
#include "../hw/DT1415ET.h"
#include "opc_controller.h"
#include "../../../common/DTmDataType.h"
#include "../../../common/DTcDataType.h"

class DTController :public opc_template_controller<DTMeasurements,DTConfiguration,DT1415ET>
{
public:
    DTController(std::string name);
//    ~DTController();
    void init(UA_Server *server);
    dtm_customType customTypeM;
    dtc_customType customTypeC;

    void addSetChannelMethod(UA_Server *server);
    void addSetVoltageMethod(UA_Server *server);
    void addSetVoltageMaxMethod(UA_Server *server);
    void addSetRampUpMethod(UA_Server *server);
    void addSetRampDownMethod(UA_Server *server);

private:
    DTMeasurements getMeasurements() final;
    DTConfiguration getSettings() final;

    static UA_StatusCode SetRampDownCallback(UA_Server *server,
                             const UA_NodeId *sessionId, void *sessionHandle,
                             const UA_NodeId *methodId, void *methodContext,
                             const UA_NodeId *objectId, void *objectContext,
                             size_t inputSize, const UA_Variant *input,
                             size_t outputSize, UA_Variant *output);
    static UA_StatusCode SetRampUpCallback(UA_Server *server,
                             const UA_NodeId *sessionId, void *sessionHandle,
                             const UA_NodeId *methodId, void *methodContext,
                             const UA_NodeId *objectId, void *objectContext,
                             size_t inputSize, const UA_Variant *input,
                             size_t outputSize, UA_Variant *output);
    static UA_StatusCode SetVoltageMaxCallback(UA_Server *server,
                             const UA_NodeId *sessionId, void *sessionHandle,
                             const UA_NodeId *methodId, void *methodContext,
                             const UA_NodeId *objectId, void *objectContext,
                             size_t inputSize, const UA_Variant *input,
                             size_t outputSize, UA_Variant *output);
    static UA_StatusCode SetVoltageCallback(UA_Server *server,
                             const UA_NodeId *sessionId, void *sessionHandle,
                             const UA_NodeId *methodId, void *methodContext,
                             const UA_NodeId *objectId, void *objectContext,
                             size_t inputSize, const UA_Variant *input,
                             size_t outputSize, UA_Variant *output);
    static UA_StatusCode SetChannelCallback(UA_Server *server,
                             const UA_NodeId *sessionId, void *sessionHandle,
                             const UA_NodeId *methodId, void *methodContext,
                             const UA_NodeId *objectId, void *objectContext,
                             size_t inputSize, const UA_Variant *input,
                             size_t outputSize, UA_Variant *output);
};



#endif // DTCONTROLLER_H
