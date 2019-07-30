/* This work is licensed under a Creative Commons CCZero 1.0 Universal License.
 * See http://creativecommons.org/publicdomain/zero/1.0/ for more information.
 */

#include <open62541/client_config_default.h>
#include <open62541/client_highlevel.h>
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include "open62541/namespace_testnodeset_generated.h"
#include "open62541/types_testnodeset_generated_handling.h"
#include <signal.h>
#include <stdlib.h>

UA_Boolean running = true;

UA_DataTypeArray customTypesArray = { NULL, UA_TYPES_TESTNODESET_COUNT, UA_TYPES_TESTNODESET};

UA_HMPm get_hmpm(){
        UA_HMPm hmpm;
        UA_HMPm_init(&hmpm);
        hmpm.output=true;
        hmpm.chSize=2;
        hmpm.ch=UA_Array_new(2, &UA_TYPES[UA_TYPES_BOOLEAN]);
        hmpm.ch[0]=true;
        hmpm.ch[1]=true;        
        hmpm.currentSize=2;
        hmpm.current=UA_Array_new(2, &UA_TYPES[UA_TYPES_DOUBLE]);
        hmpm.current[0]=3;
        hmpm.current[1]=4;
        hmpm.voltageSize=2;
        hmpm.voltage=UA_Array_new(2, &UA_TYPES[UA_TYPES_DOUBLE]);
        hmpm.voltage[0]=6;
        hmpm.voltage[1]=7;
        return hmpm;
}
static void stopHandler(int sign) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "received ctrl-c");
    running = false;
}

int main(int argc, char **argv)
{
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);

    UA_Server *server = UA_Server_new();
    UA_ServerConfig *config = UA_Server_getConfig(server);
    UA_ServerConfig_setDefault(config);

    config->customDataTypes = &customTypesArray;

    UA_StatusCode retval;
    /* create nodes from nodeset */
    if(namespace_testnodeset_generated(server) != UA_STATUSCODE_GOOD) {
        UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_SERVER,
                     "Could not add the example nodeset. "
                     "Check previous output for any error.");
        retval = UA_STATUSCODE_BADUNEXPECTEDERROR;
    } else {
        UA_Variant out;
        UA_Variant_init(&out);
        UA_Server_readValue(server, UA_NODEID_NUMERIC(2, 10002), &out);
        UA_Point *p = (UA_Point *)out.data;
        printf("point 2d x: %f \n", p->x);
        printf("point 2d x: %f y: %f z: %d \n", p->x, p->y,p->z);
        UA_VariableAttributes attr = UA_VariableAttributes_default;
//        UA_PointWithArray* myPoint=UA_PointWithArray_new();
        UA_PointWithArray myPoint;
        UA_PointWithArray_init(&myPoint);
        myPoint.x=1;
        myPoint.y=12;
        myPoint.array1Size=2;
        myPoint.array1=UA_Array_new(2, &UA_TYPES[UA_TYPES_DOUBLE]);
        myPoint.array1[0]=1;
        myPoint.array1[1]=15;
        UA_Variant_setScalar(&attr.value, &myPoint, &UA_TYPES_TESTNODESET[UA_TYPES_TESTNODESET_POINTWITHARRAY]);
        attr.description = UA_LOCALIZEDTEXT("en-US","the answer");
        attr.displayName = UA_LOCALIZEDTEXT("en-US","the answer");
        attr.dataType = UA_TYPES_TESTNODESET[UA_TYPES_TESTNODESET_POINTWITHARRAY].typeId;
        attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
        UA_NodeId myIntegerNodeId = UA_NODEID_STRING(1, "the.answer");
        UA_QualifiedName myIntegerName = UA_QUALIFIEDNAME(1, "the answer");
        UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
        UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
        retval=UA_Server_addVariableNode(server, myIntegerNodeId, parentNodeId,
                              parentReferenceNodeId, myIntegerName,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), attr, NULL, NULL);

   if(retval==UA_STATUSCODE_GOOD){
   printf("good");
   }
   else{
   printf("bad");
   }
        UA_NodeId hmpmId=UA_NODEID_NUMERIC(2,20202);
        UA_Variant vhmp;
        UA_Variant_init(&vhmp);
        UA_HMPm hmpm=get_hmpm();
        UA_Variant_setScalar(&vhmp, &hmpm, &UA_TYPES_TESTNODESET[UA_TYPES_TESTNODESET_HMPM]);

        UA_Server_writeValue(server, hmpmId, vhmp);


   retval = UA_Server_run(server, &running);
        
    }

    UA_Server_delete(server);
    return retval == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
}
