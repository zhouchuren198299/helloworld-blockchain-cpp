//
// Created by 40906 on 2021/9/20.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_INCENTIVE_H
#define HELLOWORLD_BLOCKCHAIN_CPP_INCENTIVE_H


#include "BlockchainDatabase.h"
#include "model/model.h"
using namespace model;

namespace core{
    class BlockchainDatabase;
    class Incentive {
    public:
        /**
         *
         * 矿工挖矿成功后，奖励给矿工的激励金额。
         */
        uint64_t incentiveValue(BlockchainDatabase *blockchainDatabase, Block *block) ;

        /**
         * 校验激励。
         */
        bool checkIncentive(BlockchainDatabase *blockchainDatabase, Block *block) ;
    };
}


#endif //HELLOWORLD_BLOCKCHAIN_CPP_INCENTIVE_H
