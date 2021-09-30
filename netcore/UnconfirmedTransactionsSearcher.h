//
// Created by XINGKAICHUN on 2021/9/30.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_UNCONFIRMEDTRANSACTIONSSEARCHER_H
#define HELLOWORLD_BLOCKCHAIN_CPP_UNCONFIRMEDTRANSACTIONSSEARCHER_H



#include "netcoreconfiguration/NetCoreConfiguration.h"
#include "netcoreserver/NodeServer.h"
#include "../dto/dto.h"

using namespace netcoreconfiguration;
using namespace dto;


namespace netcore{

    class UnconfirmedTransactionsSearcher {
    private:
        NetCoreConfiguration *netCoreConfiguration;
        BlockchainCore *blockchainCore;
        NodeService *nodeService;
    public:
        UnconfirmedTransactionsSearcher(NetCoreConfiguration *netCoreConfiguration, BlockchainCore *blockchainCore, NodeService *nodeService);
    public:
        void start();
    private:
        void searchUnconfirmedTransactions();
    };

}


#endif //HELLOWORLD_BLOCKCHAIN_CPP_UNCONFIRMEDTRANSACTIONSSEARCHER_H
