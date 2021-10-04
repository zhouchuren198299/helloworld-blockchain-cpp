//
// Created by XINGKAICHUN on 2021/9/30.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_NODESEARCHER_H
#define HELLOWORLD_BLOCKCHAIN_CPP_NODESEARCHER_H

#include "netcoreconfiguration/NetCoreConfiguration.h"
#include "netcoreserver/NodeServer.h"
#include "../dto/dto.h"

using namespace netcoreconfiguration;
using namespace dto;


namespace netcore{

    class NodeSearcher {
    private:
        NetCoreConfiguration *netCoreConfiguration;
        NodeService *nodeService;
    public:
        NodeSearcher(NetCoreConfiguration *netCoreConfiguration, NodeService *nodeService);
    public:
        void start();
    private:
        void searchNodes();
        void handleGetNodesResponse(unique_ptr<GetNodesResponse>& getNodesResponse);
    };

}


#endif //HELLOWORLD_BLOCKCHAIN_CPP_NODESEARCHER_H
