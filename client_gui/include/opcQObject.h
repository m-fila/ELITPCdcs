#ifndef OPCQOBJECT_H
#define OPCQOBJECT_H

#include <QObject>
#include <open62541/client_highlevel.h>
#include <open62541/client_subscriptions.h>
#include <open62541/plugin/log_stdout.h>

class opcQObject : public QObject
{
    Q_OBJECT
public:
    explicit opcQObject(std::string OName, QObject *parent = 0);
    ~opcQObject();
    const std::string ObjectName;
    UA_NodeId ObjectNodeId;
protected:
    UA_Client* client;
    UA_ClientConfig *config;
signals:

public slots:
   virtual void addMonitoredItem(UA_Client *client, UA_ClientConfig *config,
                          UA_CreateSubscriptionResponse response);
};

#endif // OPC_QOBJECT_H
