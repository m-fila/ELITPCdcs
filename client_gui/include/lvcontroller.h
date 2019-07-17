#ifndef LV_CONTROLLER_H
#define LV_CONTROLLER_H
#include "opcQObject.h"
class lv_controller : public opcQObject
{
    Q_OBJECT
public:
    lv_controller(std::string OName,QObject *parent=0);
    void callConnect(std::string IPAddress,int port);
private:
    std::string StatusVariableName;


    static void StatusChangedCallback(UA_Client *client, UA_UInt32 subId, void *subContext,
                                      UA_UInt32 monId, void *monContext, UA_DataValue *value);
public slots:
    void addMonitoredItem(UA_Client *client, UA_ClientConfig *config,
                          UA_CreateSubscriptionResponse response);
    void callDisconnect();
signals:
    void statusChanged(bool);
};

#endif // LV_CONTROLLER_H
