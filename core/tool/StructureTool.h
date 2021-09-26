//
// Created by 40906 on 2021/9/20.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_STRUCTURETOOL_H
#define HELLOWORLD_BLOCKCHAIN_CPP_STRUCTURETOOL_H


#include "../model/model.h"

using namespace model;

namespace StructureTool {
    /**
      * 校验区块的结构
      */
    bool checkBlockStructure(Block *block) ;
    /**
     * 校验交易的结构
     */
    bool checkTransactionStructure(Transaction *transaction) ;
};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_STRUCTURETOOL_H
