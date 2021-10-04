//
// Created by XINGKAICHUN on 2021/9/30.
//

#include "NullUtil.h"


namespace NullUtil {


    string NULL_FLAG = "1234";




    TransactionOutput newNullTransactionOutput(){
        TransactionOutput transactionOutput;
        transactionOutput.blockHash=NULL_FLAG;
        return transactionOutput;
    }
    bool isNullTransactionOutput(TransactionOutput transactionOutput){
        return NULL_FLAG == transactionOutput.blockHash;
    }

    Transaction newNullTransaction(){
        Transaction transaction;
        transaction.transactionHash=NULL_FLAG;
        return transaction;
    }
    bool isNullTransaction(Transaction transaction){
        return NULL_FLAG == transaction.transactionHash;
    }

    Block newNullBlock(){
        Block block;
        block.hash=NULL_FLAG;
        return block;
    }
    bool isNullBlock(Block block){
        return NULL_FLAG == block.hash;
    }


    TransactionDto newNullTransactionDto(){
        TransactionDto transactionDto;
        return transactionDto;
    }
    bool isNullTransactionDto(TransactionDto transactionDto){
        return transactionDto.inputs.empty() && transactionDto.outputs.empty();
    }

    Payee newNullPayee(){
        Payee payee;
        payee.address = NULL_FLAG;
        return payee;
    }
    bool isNullPayee(Payee payee){
        return NULL_FLAG == payee.address;
    }









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















    BlockDto newNullBlockDto(){
        BlockDto blockDto{};
        blockDto.previousHash = NULL_FLAG;
        return blockDto;
    }
    bool isNullBlockDto(BlockDto blockDto){
        return blockDto.previousHash == NULL_FLAG;
    }

    GetBlockResponse newNullGetBlockResponse(){
        GetBlockResponse getBlockResponse{};
        getBlockResponse.block.previousHash = NULL_FLAG;
        return getBlockResponse;
    }
    bool isNullBlockGetBlockResponse(GetBlockResponse getBlockResponse){
        return getBlockResponse.block.previousHash == NULL_FLAG;
    }

    GetNodesResponse newNullGetNodesResponse(){
        GetNodesResponse getNodesResponse{};
        return getNodesResponse;
    }
    //TODO
    bool isNullGetNodesResponse(GetNodesResponse getNodesResponse){
        return getNodesResponse.nodes.empty();
    }
    PingResponse newNullPingResponse(){
        PingResponse pingResponse{};
        return pingResponse;
    }
    //TODO
    bool isNullPingResponse(PingResponse pingResponse){
        return false;
    }
    GetBlockchainHeightResponse newNullGetBlockchainHeightResponse(){
        GetBlockchainHeightResponse getBlockchainHeightResponse{};
        return getBlockchainHeightResponse;
    }
    bool isNullGetBlockchainHeightResponse(GetBlockchainHeightResponse getBlockchainHeightResponse){
        return false;
    }
    GetUnconfirmedTransactionsResponse newNullGetUnconfirmedTransactionsResponse(){
        GetUnconfirmedTransactionsResponse getUnconfirmedTransactionsResponse{};
        return getUnconfirmedTransactionsResponse;
    }
    bool isNullGetUnconfirmedTransactionsResponse(GetUnconfirmedTransactionsResponse getUnconfirmedTransactionsResponse){
        return false;
    }
};