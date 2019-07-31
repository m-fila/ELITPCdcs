/* This work is licensed under a Creative Commons CCZero 1.0 Universal License.
 * See http://creativecommons.org/publicdomain/zero/1.0/ for more information. */

#include <open62541/client_config_default.h>
#include <open62541/client_highlevel.h>

#include <stdlib.h>


//#include "open62541/namespace_testnodeset_generated.h"
#include "open62541/types_dcsnodeset_generated.h"
#include "open62541/types_dcsnodeset_generated_handling.h"

int main(void) {
    /* Make your custom datatype known to the stack */

    /* Attention! Here the custom datatypes are allocated on the stack. So they
     * cannot be accessed from parallel (worker) threads. */
UA_DataTypeArray customTypesArray = { NULL, UA_TYPES_DCSNODESET_COUNT, UA_TYPES_DCSNODESET};


    UA_Client *client = UA_Client_new();
    UA_ClientConfig *cc = UA_Client_getConfig(client);
    UA_ClientConfig_setDefault(cc);
    cc->customDataTypes = &customTypesArray;

    UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://localhost:4840");
    if(retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int)retval;
    }
    
    UA_Variant value; /* Variants can hold scalar values and arrays of any type */
    UA_Variant_init(&value);
    
     UA_NodeId nodeId =
        UA_NODEID_NUMERIC(2, 20202);

    retval = UA_Client_readValueAttribute(client, nodeId, &value);
            
    if(retval == UA_STATUSCODE_GOOD) {
        UA_HMPm *p = (UA_HMPm *)value.data;
        printf("Point =%d %d %d %f, %f, %f \n", p->ch[0], p->ch[1], p->output, p->voltage[0],p->voltage[1],p->current[1]);
    }



    /* Clean up */
    UA_Variant_clear(&value);
    UA_Client_delete(client); /* Disconnects the client internally */
    return EXIT_SUCCESS;
}
