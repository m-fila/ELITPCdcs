#ifndef TPG362CONTROLLER_H
#define TPG362CONTROLLER_H
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include "TPG362.h"
#include "opc_controller.h"

#include "open62541/types_dcsnodeset_generated.h"
#include "open62541/types_dcsnodeset_generated_handling.h"
class TPG362Controller :public OpcTemplateController<UA_TPG362m,UA_TPG362c,TPG362>
{
public:
    TPG362Controller(std::string name);
//    ~HMPController();
    void init(UA_Server *server);

private:
    UA_TPG362m getMeasurements() final;
    UA_TPG362c getSettings() final;
};
#endif // TPG362CONTROLLER_H
