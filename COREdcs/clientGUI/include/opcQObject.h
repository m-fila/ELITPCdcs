#ifndef OPCQOBJECT_H
#define OPCQOBJECT_H

#include <QObject>
#include <map>
#include <open62541/client_highlevel.h>
#include <open62541/client_subscriptions.h>
#include <open62541/plugin/log_stdout.h>

class DCSMonitoredItem;

class opcQObject : public QObject {
    friend DCSMonitoredItem;
    Q_OBJECT
  public:
    explicit opcQObject(std::string OName, QObject *parent = 0);
    virtual ~opcQObject();
    const std::string ObjectName;
    UA_NodeId ObjectNodeId;
    DCSMonitoredItem *getMonitoredItem(const std::string &browseName) {
        try {
            return monitoredItems.at(browseName);
        } catch(const std::out_of_range &e) {
            return nullptr;
        }
    }
    DCSMonitoredItem *addMonitoredItem(const std::string &browseName);

  protected:
    UA_Client *client;
    UA_ClientConfig *config;
    std::map<const std::string, UA_NodeId> browsedIds;
    std::map<const std::string, DCSMonitoredItem *> monitoredItems;

    void browseIds();
  signals:

  public slots:
    virtual void opcInit(UA_Client *client, UA_ClientConfig *config,
                         UA_CreateSubscriptionResponse response);
};

class DCSMonitoredItem : public QObject {
    friend opcQObject;
    Q_OBJECT
  public:
    DCSMonitoredItem(const std::string &browseName, QObject *parent = 0)
        : QObject(parent), browseName(browseName) {}
    std::string getBrowseName() { return browseName; }
  signals:
    void valueChanged(UA_DataValue *data);

  private:
    static void callback(UA_Client *client, UA_UInt32 subId, void *subContext,
                         UA_UInt32 monId, void *monContext, UA_DataValue *value);
    const std::string browseName;
};
#endif  // OPC_QOBJECT_H
