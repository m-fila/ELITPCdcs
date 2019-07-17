#ifndef STATE_H
#define STATE_H
#include "opcQObject.h"
class stateMachine: public opcQObject
{
    Q_OBJECT
public:

    stateMachine(std::string OName,QObject *parent=0);
    ~stateMachine();
    enum state {empty,filling,ready};
private:
    UA_NodeId VariableNodeId;
    UA_NodeId MethodNodeId;
    static void valueChanged_callback(UA_Client *client, UA_UInt32 subId, void *subContext,
                                      UA_UInt32 monId, void *monContext, UA_DataValue *value);
public slots:
    void requestChange(int index);
    void addMonitoredItem(UA_Client *client, UA_ClientConfig *config,
                          UA_CreateSubscriptionResponse response);
signals:
    void stateChanged(int index);

};

#endif // STATE_H
