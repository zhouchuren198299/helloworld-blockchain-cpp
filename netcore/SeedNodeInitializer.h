//
// Created by XINGKAICHUN on 2021/9/30.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_SEEDNODEINITIALIZER_H
#define HELLOWORLD_BLOCKCHAIN_CPP_SEEDNODEINITIALIZER_H


#include "netcoreconfiguration/NetCoreConfiguration.h"
#include "netcoreserver/NodeServer.h"
#include "../dto/dto.h"

using namespace netcoreconfiguration;
using namespace dto;


namespace netcore{

    class SeedNodeInitializer {
    private:
        NetCoreConfiguration *netCoreConfiguration;
        NodeService *nodeService;
    public:
        SeedNodeInitializer(NetCoreConfiguration *netCoreConfiguration, NodeService *nodeService);
    public:
        void start();
    private:
        void addSeedNodes();
    };

}

#endif //HELLOWORLD_BLOCKCHAIN_CPP_SEEDNODEINITIALIZER_H
