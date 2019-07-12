
#include "include/opc_client.h"
#include <iostream>
#include "include/hmp_variable.h"
int main(int argc, char *argv[])
{

    opc_client client;

    client.Database.open("test.db");


// create and add variables:
//template_variable<class T> (StringId of variable on server)

    hmp_variable hmp("HMP2","Measurements");
    status_variable status("HMP2","Status");
    state_variable state("MachineState","State");

    // create DataTypeArray for every custom DataType used
    // each should point to the next in first argument
    // add only first (pointing to 2nd pointing to 3rd ... pointing to null )

    UA_DataTypeArray hmpcustom=hmp.customType.DataTypeArray(nullptr); //{nullptr,1,types};
    client.addCustomTypes(&hmpcustom);


    client.addVariable(&hmp);
    client.addVariable(&status);
    client.addVariable(&state);
    client.run();

    return 0;
}
