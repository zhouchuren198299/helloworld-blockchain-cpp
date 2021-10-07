//
// Created by 40906 on 2021/9/20.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_DTOSIZETOOL_H
#define HELLOWORLD_BLOCKCHAIN_CPP_DTOSIZETOOL_H


#include "../../netcoredto/netcoredto.h"

using namespace dto;

namespace DtoSizeTool {
    //region 校验大小
    /**
     * 校验区块大小。用来限制区块的大小。
     * 注意：校验区块的大小，不仅要校验区块的大小
     * ，还要校验区块内部各个属性(时间戳、前哈希、随机数、交易)的大小。
     */
    bool checkBlockSize(BlockDto *blockDto) ;
    /**
     * 校验交易的大小：用来限制交易的大小。
     * 注意：校验交易的大小，不仅要校验交易的大小
     * ，还要校验交易内部各个属性(交易输入、交易输出)的大小。
     */
    bool checkTransactionSize(TransactionDto *transactionDto) ;

    /**
     * 校验输出脚本的大小
     */
    bool checkOutputScriptSize(OutputScriptDto *outputScriptDto) ;

    /**
     * 校验脚本的大小
     */
    bool checkScriptSize(ScriptDto *scriptDto) ;
    //endregion

    uint64_t calculateBlockSize(BlockDto *blockDto) ;
    uint64_t calculateTransactionSize(TransactionDto *transactionDto) ;
};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_DTOSIZETOOL_H
