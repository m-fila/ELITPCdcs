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

  public slots:
    void callSetProfile(std::string key);
    void callSaveProfile(std::string key);
    void callApplyProfile();

    void callConnect();
    void callDisconnect();
    void callSetConnectionParameters(std::string IPAddress, int port);
};
#endif  // OPC_CONTROLLER_H
