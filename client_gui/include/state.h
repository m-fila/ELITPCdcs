#ifndef STATE_H
#define STATE_H
#include <QObject>
#include <string>
#include <open62541/client_highlevel.h>
#include <open62541/client_subscriptions.h>
#include <open62541/plugin/log_stdout.h>
class stateMachine: public QObject
{
    Q_OBJECT
public:
    std::string ObjectName;
    std::string VariableName;
    stateMachine();
    UA_Client* client;
    enum state {empty,filling,ready};

    static void valueChanged_callback(UA_Client *client, UA_UInt32 subId, void *subContext,
                                      UA_UInt32 monId, void *monContext, UA_DataValue *value);
public slots:
    void requestChange(int index);
    void addMonitoredItem(UA_CreateSubscriptionResponse response);
signals:
    void stateChanged(int index);

};

#endif // STATE_H
