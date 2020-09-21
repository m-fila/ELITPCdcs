#ifndef OPCQOBJECT_H
#define OPCQOBJECT_H

#include <QObject>
#include <map>
#include <open62541/client_highlevel.h>
#include <open62541/client_subscriptions.h>
#include <open62541/plugin/log_stdout.h>
class opcQObject : public QObject {
    Q_OBJECT
  public:
    explicit opcQObject(std::string OName, QObject *parent = 0);
    ~opcQObject();
    const std::string ObjectName;
    UA_NodeId ObjectNodeId;

  protected:
    UA_Client *client;
    UA_ClientConfig *config;
    std::map<std::string, UA_NodeId> browsedIds;
    /*    void addMonitoredItem(std::string VariableName,UA_CreateSubscriptionResponse
       response, void (*ValueChangedCallback)(UA_Client *client, UA_UInt32 subId, void
       *subContext, UA_UInt32 monId, void *monContext, UA_DataValue *value));
    */
    void addMonitoredItem(UA_NodeId VariableId, UA_CreateSubscriptionResponse response,
                          void (*ValueChangedCallback)(UA_Client *client, UA_UInt32 subId,
                                                       void *subContext, UA_UInt32 monId,
                                                       void *monContext,
                                                       UA_DataValue *value),
                          UA_Double sampling = 250);
    void browseIds();
  signals:

  public slots:
    virtual void opcInit(UA_Client *client, UA_ClientConfig *config,
                         UA_CreateSubscriptionResponse response);
};

#endif  // OPC_QOBJECT_H
