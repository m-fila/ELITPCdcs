#ifndef OPC_CONTROLLER_H
#define OPC_CONTROLLER_H
#include "opcQObject.h"

class opc_controller : public opcQObject {
    Q_OBJECT
  public:
    opc_controller(std::string OName, QObject *parent = 0);

  protected:
    const std::string statusVariableName = "status";
    const std::string connectionParametersVariableName = "connectionParameters";
    const std::string measurementsVariableName = "measurements";
    const std::string configurationVariableName = "configuration";
    const std::string connectBrowseName = "connect";
    const std::string disconnectBrowseName = "disconnect";
    const std::string setConnectionParametersBrowseName = "setConnectionParameters";

    const std::string deviceInfoBrowseName = "deviceInfo";

    const std::string selectedProfileBrowseName = "selectedProfile";
    const std::string enabledProfilesBrowseName = "enabledProfiles";
    const std::string saveProfileBrowseName = "saveProfile";
    const std::string applyProfileBrowseName = "applyProfile";
    const std::string setProfileBrowseName = "setProfile";

    static void StatusChangedCallback(UA_Client *client, UA_UInt32 subId,
                                      void *subContext, UA_UInt32 monId, void *monContext,
                                      UA_DataValue *value);
    static void ConnectionParametersChangedCallback(UA_Client *client, UA_UInt32 subId,
                                                    void *subContext, UA_UInt32 monId,
                                                    void *monContext,
                                                    UA_DataValue *value);
    static void MeasurementsChangedCallback(UA_Client *client, UA_UInt32 subId,
                                            void *subContext, UA_UInt32 monId,
                                            void *monContext, UA_DataValue *value);
    static void ConfigurationChangedCallback(UA_Client *client, UA_UInt32 subId,
                                             void *subContext, UA_UInt32 monId,
                                             void *monContext, UA_DataValue *value);
    static void SelectedProfileChangedCallback(UA_Client *client, UA_UInt32 subId,
                                               void *subContext, UA_UInt32 monId,
                                               void *monContext, UA_DataValue *value);
    static void EnabledProfilesChangedCallback(UA_Client *client, UA_UInt32 subId,
                                               void *subContext, UA_UInt32 monId,
                                               void *monContext, UA_DataValue *value);
    static void DeviceInfoChangedCallback(UA_Client *client, UA_UInt32 subId,
                                          void *subContext, UA_UInt32 monId,
                                          void *monContext, UA_DataValue *value);
  public slots:
    void opcInit(UA_Client *client, UA_ClientConfig *config,
                 UA_CreateSubscriptionResponse response) override;
    void callSetProfile(std::string key);
    void callSaveProfile(std::string key);
    void callApplyProfile();

    void callConnect();
    void callDisconnect();
    void callSetConnectionParameters(std::string IPAddress, int port);

  signals:
    void statusChanged(void *);
    void connectionParametersChanged(void *);
    void measurementsChanged(void *);
    void configurationChanged(void *);

    void selectedProfileChanged(void *);
    void enabledProfilesChanged(void *);
    void deviceInfoChanged(void *);
};

#endif  // OPC_CONTROLLER_H
