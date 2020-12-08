#ifndef DCS_VARIABLE_H
#define DCS_VARIABLE_H
#include "DCSLogger.h"
#include "DCSObject.h"
#include <functional>
#include <open62541/plugin/historydata/history_data_gathering_default.h>
#include <open62541/plugin/historydata/history_database_default.h>
#include <open62541/server.h>
#include <open62541/server_config.h>
#include <string>
class DCSObject;
// OPC UA variable wrapper
class DCSVariable {
    friend DCSObject;

  public:
    std::string getName() { return variableName; }
    std::string getFullName() { return parentName + "." + variableName; }
    inline const UA_DataType *getDataType() { return &dataType; }
    inline void setNull() {
        UA_Variant var;
        UA_Variant_init(&var);
        UA_Server_writeValue(server, variableNodeId, var);
    }
    inline void setValueByVariant(UA_Variant &newVal) {
        UA_Server_writeValue(server, variableNodeId, newVal);
        if(newVal.type->pointerFree == false) {
            UA_deleteMembers(newVal.data, newVal.type);
            //  UA_Variant_deleteMembers(&newVal);
        }
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

    inline UA_Variant getValueByVariant() const {
        UA_Variant var;
        UA_Server_readValue(server, variableNodeId, &var);
        return var;
    }
    template <class T> T getValue() const {
        auto var = getValueByVariant();
        if(UA_Variant_isEmpty(&var)) {
            throw std::runtime_error(variableName + " has no value");
        }
        return *static_cast<T *>(var.data);
    }
    template <class T> T getValueByPointer() const {
        auto var = getValueByVariant();
        return UA_Variant_isEmpty(&var) ? static_cast<T>(var.data) : nullptr;
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

    void addUpdate(size_t interval_ms, std::function<void()> callback) {
        updateInterval_ms = interval_ms;
        updateCallback = callback;
        timedCallback(server, this);
    }

    void setHistorizing(std::string backendName = "default");
    void stopHistorizing();

  protected:
    DCSVariable(UA_Server *server, UA_NodeId parentNodeId, const std::string &parentName,
                const std::string &variableName, UA_DataType type);
    inline UA_Byte getAccesLevel() {
        UA_Byte accessLevel;
        UA_Server_readAccessLevel(server, variableNodeId, &accessLevel);
        return accessLevel;
    }
    inline bool setAccessLevel(UA_Byte mask, bool access) {
        auto accessLevel = access ? getAccesLevel() | mask : getAccesLevel() & ~mask;
        return UA_Server_writeAccessLevel(server, variableNodeId, accessLevel);
    }

    static void timedCallback(UA_Server *server, void *data) {
        auto var = static_cast<DCSVariable *>(data);
        auto t = UA_DateTime_nowMonotonic() + var->updateInterval_ms * UA_DATETIME_MSEC;
        var->updateCallback();

        UA_Server_addTimedCallback(server, timedCallback, var, t, nullptr);
    }

    UA_Server *server;
    const std::string parentName;
    const std::string variableName;
    UA_NodeId variableNodeId;
    UA_DataType dataType;
    size_t updateInterval_ms = 2000;
    std::function<void()> updateCallback;
};

#endif  // DCS_VARIABLE_H
