//
// Created by XINGKAICHUN on 2021/9/30.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_BLOCKCHAINHEIGHTBROADCASTER_H
#define HELLOWORLD_BLOCKCHAIN_CPP_BLOCKCHAINHEIGHTBROADCASTER_H
#include "netcoreconfiguration/NetCoreConfiguration.h"
#include "netcoreserver/NodeServer.h"

using namespace netcoreconfiguration;


namespace netcore{

    class BlockchainHeightBroadcaster {
    private:
        NetCoreConfiguration *netCoreConfiguration;
        BlockchainCore *blockchainCore;
        NodeService *nodeService;
    public:
        BlockchainHeightBroadcaster(NetCoreConfiguration *netCoreConfiguration, BlockchainCore *blockchainCore, NodeService *nodeService);
    public:
        void start();
    private:
        void broadcastBlockchainHeight();
    };

}

#endif //HELLOWORLD_BLOCKCHAIN_CPP_BLOCKCHAINHEIGHTBROADCASTER_H
