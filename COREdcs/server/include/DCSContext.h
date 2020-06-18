#ifndef DCS_CONTEXT_H
#define DCS_CONTEXT_H
#include <open62541/server.h>
namespace DCS {
template <class T> T getContext(UA_Server *server, const UA_NodeId nodeId) {
  void *i;
  auto retv = UA_Server_getNodeContext(server, nodeId, &i);
  if (retv != UA_STATUSCODE_GOOD) {
    throw std::runtime_error("Unable to get node context");
  }
  return static_cast<T>(i);
}
}; // namespace DCS

#endif // DCS_CONTEXT_H