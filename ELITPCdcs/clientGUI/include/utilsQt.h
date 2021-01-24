#ifndef DCS_UTILS_QT_H
#define DCS_UTILS_QT_H
#include <QStringList>
namespace DCSUtils {
template <class T> QStringList getKeys(T iterable) {
    QStringList ls;
    for(const auto &i : iterable) {
        ls << QString::fromStdString(i.first);
    }
    return ls;
}
};      // namespace DCSUtils
#endif  // DCS_UTILS_QT_H
