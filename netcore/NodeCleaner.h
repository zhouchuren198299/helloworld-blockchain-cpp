//
// Created by XINGKAICHUN on 2021/9/30.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_NODECLEANER_H
#define HELLOWORLD_BLOCKCHAIN_CPP_NODECLEANER_H


#include "netcoreconfiguration/NetCoreConfiguration.h"
#include "netcoreserver/NodeServer.h"

using namespace netcoreconfiguration;


namespace netcore{

    class NodeCleaner {
    private:
        NetCoreConfiguration *netCoreConfiguration;
        NodeService *nodeService;
    public:
        NodeCleaner(NetCoreConfiguration *netCoreConfiguration, NodeService *nodeService);
    public:
        void start();
    private:
        void cleanDeadNodes();
    };

}


#endif //HELLOWORLD_BLOCKCHAIN_CPP_NODECLEANER_H
