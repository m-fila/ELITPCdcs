#ifndef OPC_CLIENT_H
#define OPC_CLIENT_H

#include <QObject>
//#include <QThread>
#include <QTimer>
#include <open62541/client_config_default.h>
#include <open62541/client_highlevel.h>
#include <open62541/plugin/log_stdout.h>
#include <open62541/client_subscriptions.h>
#include <iostream>

class opc_client: public QObject//QThread
{
    Q_OBJECT
public:
    opc_client();
    ~opc_client();
    UA_Client *client;
    UA_ClientConfig *config;
private:
    static opc_client* context;
    QTimer* client_clock;
    void connectSignals();
    static void stateCallback (UA_Client *client, UA_ClientState clientState);
    void addSubscription();

signals:
    void subCreated(UA_CreateSubscriptionResponse response);
public slots:
    void iterate();
};

#endif // OPC_CLIENT_H
