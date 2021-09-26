//
// Created by 40906 on 2021/9/25.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_NULLTOOL_H
#define HELLOWORLD_BLOCKCHAIN_CPP_NULLTOOL_H


#include "../model/model.h"
using namespace model;

namespace NullTool {

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
};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_NULLTOOL_H
