//
// Created by 40906 on 2021/9/28.
//

#include "NetcoreNullTool.h"


namespace NetcoreNullTool {

    string NULL_FLAG = "1234";


    NodePo newNullNodePo(){
        NodePo nodePo;
        nodePo.ip = NULL_FLAG;
        return nodePo;
    }
    bool isNullNodePo(NodePo nodePo){
        return NULL_FLAG == nodePo.ip;
    }


    Node newNullNode(){
        Node node;
        node.ip = NULL_FLAG;
        return node;
    }
    bool isNullNode(Node node){
        return NULL_FLAG == node.ip;
    }

/*    PingResponse newNullPingResponse();
    bool isNullPingResponse(PingResponse pingResponse);

    GetBlockResponse newNullGetBlockResponse();
    bool isNullGetBlockResponse(GetBlockResponse getBlockResponse);

    PostTransactionResponse newNullPostTransactionResponse();
    bool isNullPostTransactionResponse(PostTransactionResponse postTransactionResponse);

    PostBlockResponse newNullPostBlockResponse();
    bool isNullPostBlockResponse(PostBlockResponse postBlockResponse);

    GetNodesResponse newNullGetNodesResponse();
    bool isNullGetNodesResponse(GetNodesResponse getNodesResponse);

    PostBlockchainHeightResponse newNullPostBlockchainHeightResponse();
    bool isNullPostBlockchainHeightResponse(PostBlockchainHeightResponse postBlockchainHeightResponse);

    GetBlockchainHeightResponse newNullGetBlockchainHeightResponse();
    bool isNullGetBlockchainHeightResponse(GetBlockchainHeightResponse getBlockchainHeightResponse);

    GetUnconfirmedTransactionsResponse newNullGetUnconfirmedTransactionsResponse();
    bool isNullGetUnconfirmedTransactionsResponse(GetUnconfirmedTransactionsResponse getUnconfirmedTransactionsResponse);*/
};