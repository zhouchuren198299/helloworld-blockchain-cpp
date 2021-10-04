//
// Created by XINGKAICHUN on 2021/9/30.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_NULLUTIL_H
#define HELLOWORLD_BLOCKCHAIN_CPP_NULLUTIL_H


#include "../dto/dto.h"
#include "../core/model/model.h"
#include "../netcore/netcorepo/netcorepo.h"
#include "../netcore/netcoremodel/netcoremodel.h"
using namespace dto;
using namespace model;
using namespace netcorepo;
using namespace netcoremodel;

namespace NullUtil {



    TransactionOutput newNullTransactionOutput();
    bool isNullTransactionOutput(TransactionOutput transactionOutput);

    Transaction newNullTransaction();
    bool isNullTransaction(Transaction transaction);

    Block newNullBlock();
    bool isNullBlock(Block block);


    TransactionDto newNullTransactionDto();
    bool isNullTransactionDto(TransactionDto transactionDto);

    Payee newNullPayee();
    bool isNullPayee(Payee payee);











    NodePo newNullNodePo();
    bool isNullNodePo(NodePo nodePo);

    Node newNullNode();
    bool isNullNode(Node node);









    BlockDto newNullBlockDto();
    bool isNullBlockDto(BlockDto blockDto);

    GetBlockResponse newNullGetBlockResponse();
    bool isNullBlockGetBlockResponse(GetBlockResponse getBlockResponse);

    GetNodesResponse newNullGetNodesResponse();
    bool isNullGetNodesResponse(GetNodesResponse getNodesResponse);

    PingResponse newNullPingResponse();
    bool isNullPingResponse(PingResponse pingResponse);

    GetBlockchainHeightResponse newNullGetBlockchainHeightResponse();
    bool isNullGetBlockchainHeightResponse(GetBlockchainHeightResponse getBlockchainHeightResponse);

    GetUnconfirmedTransactionsResponse newNullGetUnconfirmedTransactionsResponse();
    bool isNullGetUnconfirmedTransactionsResponse(GetUnconfirmedTransactionsResponse getUnconfirmedTransactionsResponse);

};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_NULLUTIL_H
