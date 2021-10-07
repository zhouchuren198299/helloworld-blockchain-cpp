//
// Created by XINGKAICHUN on 2021/9/30.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_NODECLIENT_H
#define HELLOWORLD_BLOCKCHAIN_CPP_NODECLIENT_H

#include "../netcoredto/netcoredto.h"

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
        unique_ptr<PostTransactionResponse> postTransaction(PostTransactionRequest request);

        /**
         * Ping节点
         */
        unique_ptr<PingResponse> pingNode(PingRequest request);

        /**
         * 根据区块高度，获取区块
         */
        unique_ptr<GetBlockResponse> getBlock(GetBlockRequest request);

        /**
         * 获取节点列表
         */
        unique_ptr<GetNodesResponse> getNodes(GetNodesRequest request);

        /**
         * 提交区块至节点
         */
        unique_ptr<PostBlockResponse> postBlock(PostBlockRequest request);

        /**
         * 提交区块链高度至节点
         */
        unique_ptr<PostBlockchainHeightResponse> postBlockchainHeight(PostBlockchainHeightRequest request);

        /**
         * 获取区块链高度
         */
        unique_ptr<GetBlockchainHeightResponse> getBlockchainHeight(GetBlockchainHeightRequest request);

        /**
         * 根据交易高度，获取交易
         */
        unique_ptr<GetUnconfirmedTransactionsResponse> getUnconfirmedTransactions(GetUnconfirmedTransactionsRequest getUnconfirmedTransactionsRequest);
    };
}



#endif //HELLOWORLD_BLOCKCHAIN_CPP_NODECLIENT_H
