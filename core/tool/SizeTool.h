//
// Created by 40906 on 2021/9/20.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_SIZETOOL_H
#define HELLOWORLD_BLOCKCHAIN_CPP_SIZETOOL_H


#include "../model/model.h"

using namespace model;


namespace SizeTool {
    //region 校验大小
    /**
     * 校验区块大小。用来限制区块的大小。
     * 注意：校验区块的大小，不仅要校验区块的大小
     * ，还要校验区块内部各个属性(时间戳、前哈希、随机数、交易)的大小。
     */
    bool checkBlockSize(Block *block) ;
    /**
     * 校验交易的大小：用来限制交易的大小。
     * 注意：校验交易的大小，不仅要校验交易的大小
     * ，还要校验交易内部各个属性(交易输入、交易输出)的大小。
     */
    bool checkTransactionSize(Transaction *transaction) ;
    //endregion



    //region 计算大小
    uint64_t calculateBlockSize(Block *block) ;
    uint64_t calculateTransactionSize(Transaction *transaction) ;
};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_SIZETOOL_H
