#ifndef DCS_UTILS_QT_H
#define DCS_UTILS_QT_H
#include <QDateTime>
#include <QStringList>
#include <open62541/types.h>
namespace DCSUtils {
template <class T> QStringList getKeys(T iterable) {
    QStringList ls;
    for(const auto &i : iterable) {
        ls << QString::fromStdString(i.first);
    }
    return ls;
}
static QDateTime getTimestamp(UA_DataValue *data) {
    QDateTime qTime;
    UA_DateTime uaTime;
    if(data->hasSourceTimestamp) {
        uaTime = data->sourceTimestamp;
    } else if(data->hasServerTimestamp) {
        uaTime = data->serverTimestamp;
    } else {
        return QDateTime::currentDateTime();
    }
    qTime.setTime_t(UA_DateTime_toUnixTime(uaTime));
    return qTime;
}
};      // namespace DCSUtils
#endif  // DCS_UTILS_QT_H
