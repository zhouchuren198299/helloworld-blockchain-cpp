//
// Created by 40906 on 2021/9/28.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_NODESERVER_H
#define HELLOWORLD_BLOCKCHAIN_CPP_NODESERVER_H

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "../../thirdpart/httplib/httplib.h"
#include "../../dto/dto.h"
#include "../../core/BlockchainCore.h"
#include "../netcoreservice/NodeService.h"

using namespace dto;
using namespace core;
using namespace service;

namespace server{

    class NodeServer {
    private:
        BlockchainCore *blockchainCore;
        NodeService *nodeService;
        NetCoreConfiguration *netCoreConfiguration;
    public:
        //region 挖矿相关
        NodeServer(NetCoreConfiguration *netCoreConfiguration,BlockchainCore *blockchainCore, NodeService *nodeService);
    public:
        void start() ;
    private:
        PingResponse ping(string requestIp, PingRequest request);
        GetBlockResponse getBlock(GetBlockRequest request);
        PostTransactionResponse postTransaction(PostTransactionRequest request);
        PostBlockResponse postBlock(PostBlockRequest request);
        GetNodesResponse getNodes(GetNodesRequest request);
        PostBlockchainHeightResponse postBlockchainHeight(string requestIp, PostBlockchainHeightRequest request);
        GetBlockchainHeightResponse getBlockchainHeight(GetBlockchainHeightRequest request);
        GetUnconfirmedTransactionsResponse getUnconfirmedTransactions(GetUnconfirmedTransactionsRequest request);
    };

}




#endif //HELLOWORLD_BLOCKCHAIN_CPP_NODESERVER_H
