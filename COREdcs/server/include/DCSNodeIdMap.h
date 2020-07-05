#ifndef DCS_NODEID_MAP_H
#define DCS_NODEID_MAP_H
#include <open62541/types.h>
struct NodeIdCmp {
    bool operator()(const UA_NodeId& a, const UA_NodeId& b) const {
        return UA_NodeId_hash(&a) < UA_NodeId_hash(&b);
    }
};
#endif // DCS_NODEID_MAP_H