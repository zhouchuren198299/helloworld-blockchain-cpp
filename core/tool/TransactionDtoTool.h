//
// Created by 40906 on 2021/9/18.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_TRANSACTIONDTOTOOL_H
#define HELLOWORLD_BLOCKCHAIN_CPP_TRANSACTIONDTOTOOL_H

#include "../../dto/dto.h"

using namespace std;

namespace TransactionDtoTool {
    string calculateTransactionHash(dto::TransactionDto transactionDto);
};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_TRANSACTIONDTOTOOL_H