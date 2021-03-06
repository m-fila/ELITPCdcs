#ifndef OPC_CLIENT_H
#define OPC_CLIENT_H

#include <QObject>
//#include <QThread>
#include <QTimer>
#include <iostream>
#include <open62541/client_config_default.h>
#include <open62541/client_highlevel.h>
#include <open62541/client_subscriptions.h>
#include <open62541/plugin/log_stdout.h>
class MainWindow;
class opc_client : public QObject {
    Q_OBJECT
    friend MainWindow;

  public:
    opc_client(std::string address, int port);
    void start() { client_clock->start(100); }
    ~opc_client();
    UA_Client *client;
    UA_ClientConfig *config;

  protected:
    std::string tcp_address;
    // static opc_client* context;
    QTimer *client_clock;
    void connectSignals();
    static void stateCallback(UA_Client *client, UA_SecureChannelState channelState,
                              UA_SessionState sessionState, UA_StatusCode recoveryStatus);
    void addSubscription(UA_Double interval = 500);
    enum class Connection { Closed, Opened, Initial } connection = Connection::Initial;

    // custom Types:
    void addCustomTypes(UA_DataTypeArray *custom);
  signals:
    void subCreated(UA_Client *client, UA_ClientConfig *config,
                    UA_CreateSubscriptionResponse response);
    void clientConnected(bool);
  public slots:
    void iterate();
};

#endif  // OPC_CLIENT_H
