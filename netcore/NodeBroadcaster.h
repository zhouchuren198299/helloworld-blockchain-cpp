//
// Created by XINGKAICHUN on 2021/9/30.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_NODEBROADCASTER_H
#define HELLOWORLD_BLOCKCHAIN_CPP_NODEBROADCASTER_H


#include "netcoreconfiguration/NetCoreConfiguration.h"
#include "netcoreserver/NodeServer.h"

using namespace netcoreconfiguration;


namespace netcore{

    class NodeBroadcaster {
    private:
        NetCoreConfiguration *netCoreConfiguration;
        NodeService *nodeService;
    public:
        NodeBroadcaster(NetCoreConfiguration *netCoreConfiguration, NodeService *nodeService);
    public:
        void start();
    private:
        void broadcastNode();
    };

}

#endif //HELLOWORLD_BLOCKCHAIN_CPP_NODEBROADCASTER_H
