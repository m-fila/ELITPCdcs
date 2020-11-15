#ifndef OPC_CONTROLLER_H
#define OPC_CONTROLLER_H
#include "opcQObject.h"

class opc_controller : public opcQObject {
    Q_OBJECT
  public:
    opc_controller(std::string OName, QObject *parent = 0);
    void opcInit(UA_Client *client, UA_ClientConfig *config,
                 UA_CreateSubscriptionResponse response) override;
    void callConnect(std::string IPAddress, int port);
    void callDisconnect();

  protected:
    const std::string statusVariableName = "status";
    const std::string measurementsVariableName = "measurements";
    const std::string configurationVariableName = "configuration";
    const std::string connectBrowseName = "connect";
    const std::string disconnectBrowseName = "disconnect";

    const std::string activeProfileBrowseName = "activeProfile";
    const std::string enabledProfilesBrowseName = "enabledProfiles";
    const std::string dumpProfileBrowseName = "dumpProfile";
    const std::string applyProfileBrowseName = "applyProfile";
    const std::string setProfileBrowseName = "setProfile";

    static void StatusChangedCallback(UA_Client *client, UA_UInt32 subId,
                                      void *subContext, UA_UInt32 monId, void *monContext,
                                      UA_DataValue *value);
    static void MeasurementsChangedCallback(UA_Client *client, UA_UInt32 subId,
                                            void *subContext, UA_UInt32 monId,
                                            void *monContext, UA_DataValue *value);
    static void ConfigurationChangedCallback(UA_Client *client, UA_UInt32 subId,
                                             void *subContext, UA_UInt32 monId,
                                             void *monContext, UA_DataValue *value);
    static void ActiveProfileChangedCallback(UA_Client *client, UA_UInt32 subId,
                                             void *subContext, UA_UInt32 monId,
                                             void *monContext, UA_DataValue *value);
    static void EnabledProfilesChangedCallback(UA_Client *client, UA_UInt32 subId,
                                               void *subContext, UA_UInt32 monId,
                                               void *monContext, UA_DataValue *value);
    //    virtual void browseIds();
  public slots:
    void callSetProfile(std::string key);
    void callDumpProfile(std::string key);
    void callApplyProfile();

  signals:
    void statusChanged(void *);
    void measurementsChanged(void *);
    void configurationChanged(void *);

    void activeProfileChanged(void *);
    void enabledProfilesChanged(void *);
};

#endif  // OPC_CONTROLLER_H
