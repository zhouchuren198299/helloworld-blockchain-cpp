//
// Created by XINGKAICHUN on 2021/9/30.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_BLOCKCHAINHEIGHTSEARCHER_H
#define HELLOWORLD_BLOCKCHAIN_CPP_BLOCKCHAINHEIGHTSEARCHER_H
#include "netcoreconfiguration/NetCoreConfiguration.h"
#include "netcoreserver/NodeServer.h"

using namespace netcoreconfiguration;


namespace netcore{

    class BlockchainHeightSearcher {
    private:
        NetCoreConfiguration *netCoreConfiguration;
        NodeService *nodeService;
    public:
        BlockchainHeightSearcher(NetCoreConfiguration *netCoreConfiguration, NodeService *nodeService);
    public:
        void start();
    private:
        void searchBlockchainHeight();
    };

}

#endif //HELLOWORLD_BLOCKCHAIN_CPP_BLOCKCHAINHEIGHTSEARCHER_H
