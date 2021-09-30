//
// Created by 40906 on 2021/9/28.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_NETCORENULLTOOL_H
#define HELLOWORLD_BLOCKCHAIN_CPP_NETCORENULLTOOL_H


#include "../netcorepo/netcorepo.h"
#include "../netcoremodel/netcoremodel.h"
#include "../../dto/dto.h"

using namespace netcorepo;
using namespace netcoremodel;
using namespace dto;

namespace NetcoreNullTool {
    NodePo newNullNodePo();
    bool isNullNodePo(NodePo nodePo);

    Node newNullNode();
    bool isNullNode(Node node);



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


#endif //HELLOWORLD_BLOCKCHAIN_CPP_NETCORENULLTOOL_H
