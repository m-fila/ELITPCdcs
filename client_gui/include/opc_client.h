#ifndef OPC_CLIENT_H
#define OPC_CLIENT_H

#include <QObject>
#include <open62541/client_config_default.h>
#include <open62541/client_highlevel.h>
class opc_client : public QObject
{
    Q_OBJECT
public:
    explicit opc_client(QObject *parent = 0);
    ~opc_client();
    UA_Client *client;
    UA_ClientConfig *config;


signals:

public slots:
    void run();
};

#endif // OPC_CLIENT_H
