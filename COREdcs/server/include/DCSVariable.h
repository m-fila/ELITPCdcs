#ifndef DCS_VARIABLE_H
#define DCS_VARIABLE_H
#include <functional>
#include <iostream>
#include <open62541/plugin/historydata/history_data_gathering_default.h>
#include <open62541/plugin/historydata/history_database_default.h>
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config.h>
#include <string>
#include "DCSObject.h"
class DCSObject;
// OPC UA variable wrapper
class DCSVariable {
  friend DCSObject;

public:
  inline const UA_DataType *getDataType() { return &dataType; }
  inline void setValueByVariant(UA_Variant &newVal) {
    UA_Server_writeValue(server, variableNodeId, newVal);
    //  UA_Variant_deleteMembers(&newVal)
  }
  template <class T> void setValueByPointer(T newVal) {
    UA_Variant var;
    UA_Variant_setScalar(&var, newVal, &dataType);
    return setValueByVariant(var);
  }
  template <class T> void setValue(T &&newVal) {
    UA_Variant var;
    UA_Variant_setScalar(&var, &newVal, &dataType);
    return setValueByVariant(var);
  }

  inline UA_Variant getValueByVariant() {
    UA_Variant val;
    UA_Server_readValue(server, variableNodeId, &val);
    return val;
  }
  template <class T> T getValue() {
    return *static_cast<T *>(getValueByVariant());
  }
  template <class T> T getValueByPointer() {
    return static_cast<T>(getValueByVariant());
  }

  inline void setAccessRead(bool access) {
    setAccessLevel(UA_ACCESSLEVELMASK_READ, access);
  }
  inline void setAccessWrite(bool access) {
    setAccessLevel(UA_ACCESSLEVELMASK_WRITE, access);
  }
  inline void setAccessHistoryRead(bool access) {
    setAccessLevel(UA_ACCESSLEVELMASK_HISTORYREAD, access);
  }
  inline void setAccessHistoryWrite(bool access) {
    setAccessLevel(UA_ACCESSLEVELMASK_HISTORYWRITE, access);
  }
  inline void setAccessSemanticChange(bool access) {
    setAccessLevel(UA_ACCESSLEVELMASK_SEMANTICCHANGE, access);
  }
  inline void setAccessStatusWrite(bool access) {
    setAccessLevel(UA_ACCESSLEVELMASK_STATUSWRITE, access);
  }
  inline void setAccessTimestampWrite(bool access) {
    setAccessLevel(UA_ACCESSLEVELMASK_TIMESTAMPWRITE, access);
  }

  void addUpdate(uint interval_ms, std::function<void()> callback) {
    updateInterval_ms = interval_ms;
    updateCallback = callback;
    updateActive = true;
    timedCallback(server, this);
  }

  void setHistorizing(std::string backendName="default");
  void stopHistorizing();

protected:
  DCSVariable(UA_Server *server, UA_NodeId parentNodeId, std::string name,
              UA_DataType type);
  inline UA_Byte getAccesLevel() {
    UA_Byte accessLevel;
    UA_Server_readAccessLevel(server, variableNodeId, &accessLevel);
    return accessLevel;
  }
  inline bool setAccessLevel(UA_Byte mask, bool access) {
    auto accessLevel =
        access ? getAccesLevel() | mask : getAccesLevel() & ~mask;
    return UA_Server_writeAccessLevel(server, variableNodeId, accessLevel);
  }

  static void timedCallback(UA_Server *server, void *data) {
    auto var = static_cast<DCSVariable *>(data);
    auto t =
        UA_DateTime_nowMonotonic() + var->updateInterval_ms * UA_DATETIME_MSEC;
    var->updateCallback();

    UA_Server_addTimedCallback(server, timedCallback, var, t, nullptr);
  }

  UA_Server *server;
  const std::string variableName;
  UA_NodeId variableNodeId;
  UA_DataType dataType;
  bool updateActive = false;
  uint updateInterval_ms;
  std::function<void()> updateCallback;
};

#endif // DCS_VARIABLE_H
