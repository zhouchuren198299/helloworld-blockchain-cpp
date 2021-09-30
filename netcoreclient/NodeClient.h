//
// Created by XINGKAICHUN on 2021/9/30.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_NODECLIENT_H
#define HELLOWORLD_BLOCKCHAIN_CPP_NODECLIENT_H

#include "../dto/dto.h"

using namespace dto;

namespace netcoreclient{

    class NodeClient {
    private:
        string ip;
    private:
        string getUrl(string api);
    public:
        NodeClient(string ip);
    public:
        /**
         * 提交交易至节点
         */
        PostTransactionResponse postTransaction(PostTransactionRequest request);

        /**
         * Ping节点
         */
        PingResponse pingNode(PingRequest request);

        /**
         * 根据区块高度，获取区块
         */
        GetBlockResponse getBlock(GetBlockRequest request);

        /**
         * 获取节点列表
         */
        GetNodesResponse getNodes(GetNodesRequest request);

        /**
         * 提交区块至节点
         */
        PostBlockResponse postBlock(PostBlockRequest request);

        /**
         * 提交区块链高度至节点
         */
        PostBlockchainHeightResponse postBlockchainHeight(PostBlockchainHeightRequest request);

        /**
         * 获取区块链高度
         */
        GetBlockchainHeightResponse getBlockchainHeight(GetBlockchainHeightRequest request);

        /**
         * 根据交易高度，获取交易
         */
        GetUnconfirmedTransactionsResponse getUnconfirmedTransactions(GetUnconfirmedTransactionsRequest getUnconfirmedTransactionsRequest);
    };
}



#endif //HELLOWORLD_BLOCKCHAIN_CPP_NODECLIENT_H
