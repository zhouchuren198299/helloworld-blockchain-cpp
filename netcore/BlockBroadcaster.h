//
// Created by XINGKAICHUN on 2021/9/30.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_BLOCKBROADCASTER_H
#define HELLOWORLD_BLOCKCHAIN_CPP_BLOCKBROADCASTER_H


#include "netcoreconfiguration/NetCoreConfiguration.h"
#include "netcoreserver/NodeServer.h"


using namespace netcoreconfiguration;

namespace netcore{

    class BlockBroadcaster {
    private:
        NetCoreConfiguration *netCoreConfiguration;
        BlockchainCore *blockchainCore;
        NodeService *nodeService;
    public:
        BlockBroadcaster(NetCoreConfiguration *netCoreConfiguration, BlockchainCore *blockchainCore, NodeService *nodeService);
    public:
        void start();
    private:
        void broadcastBlock();
    };

}


#endif //HELLOWORLD_BLOCKCHAIN_CPP_BLOCKBROADCASTER_H
