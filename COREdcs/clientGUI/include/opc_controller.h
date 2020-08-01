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
    //    UA_NodeId StatusNodeId;
    //    UA_NodeId MeasurementsNodeId;
    //    UA_NodeId ConfigurationNodeId;
    //    UA_NodeId connectNodeId;
    //    UA_NodeId disconnectNodeId;
    static void StatusChangedCallback(UA_Client *client, UA_UInt32 subId,
                                      void *subContext, UA_UInt32 monId, void *monContext,
                                      UA_DataValue *value);
    static void MeasurementsChangedCallback(UA_Client *client, UA_UInt32 subId,
                                            void *subContext, UA_UInt32 monId,
                                            void *monContext, UA_DataValue *value);
    static void ConfigurationChangedCallback(UA_Client *client, UA_UInt32 subId,
                                             void *subContext, UA_UInt32 monId,
                                             void *monContext, UA_DataValue *value);
    //    virtual void browseIds();
  public slots:

  signals:
    void statusChanged(void *);
    void measurementsChanged(void *);
    void configurationChanged(void *);
};

#endif  // OPC_CONTROLLER_H
