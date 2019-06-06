#ifndef OPC_MONITOR_H
#define OPC_MONITOR_H


class opcmonitor
{
public:
    opcmonitor();
private:
    static void addMonitoredItem(UA_Server *server);
    static void addValueCallbackToCurrentTimeVariable(UA_Server *server);

};

#endif // OPC_MONITOR_H
