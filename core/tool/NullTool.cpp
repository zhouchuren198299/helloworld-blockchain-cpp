//
// Created by 40906 on 2021/9/25.
//

#include "NullTool.h"

namespace NullTool{

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
}