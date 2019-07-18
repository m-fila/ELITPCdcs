#ifndef LV_CONTROLLER_H
#define LV_CONTROLLER_H
#include "opcQObject.h"
#include "../common/HMPDataType.h"
class lv_controller : public opcQObject
{
    Q_OBJECT
public:
    lv_controller(std::string OName,QObject *parent=0);
    void callSetChannel(int nr, bool state);
    void callSetOutput(bool state);
    void callConnect(std::string IPAddress,int port);
private:
    const std::string StatusVariableName;
    const std::string MeasurementsVariableName;
    const std::string ConfigurationVariableName;

    static void StatusChangedCallback(UA_Client *client, UA_UInt32 subId, void *subContext,
                                      UA_UInt32 monId, void *monContext, UA_DataValue *value);
    static void MeasurementsChangedCallback(UA_Client *client, UA_UInt32 subId, void *subContext,
                                      UA_UInt32 monId, void *monContext, UA_DataValue *value);
    static void ConfigurationChangedCallback(UA_Client *client, UA_UInt32 subId, void *subContext,
                                      UA_UInt32 monId, void *monContext, UA_DataValue *value);

public slots:
    void opcInit(UA_Client *client, UA_ClientConfig *config,
                          UA_CreateSubscriptionResponse response);
    void callDisconnect();
signals:
    void statusChanged(bool);
    void measurementsChanged(HMPMeasurements);
    void configurationChanged(HMPMeasurements);
};

#endif // LV_CONTROLLER_H
