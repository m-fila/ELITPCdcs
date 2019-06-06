#ifndef NODEMAP_H
#define NODEMAP_H
#include "opc_monitor.h"
#include <map>
struct NodeMap{
    static std::map <std::string, opc_monitor*> Nodes;
};

#endif // NODEMAP_H

