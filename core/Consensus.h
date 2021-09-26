//
// Created by 40906 on 2021/9/20.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_CONSENSUS_H
#define HELLOWORLD_BLOCKCHAIN_CPP_CONSENSUS_H


#include "BlockchainDatabase.h"
#include "model/model.h"

using namespace model;

namespace core{
    class BlockchainDatabase;
    class Consensus {
    public:
        /**
         * 校验区块是否满足共识
         * 如果区块满足共识的要求，这个区块就可能(为什么说是可能呢？因为还要进一步校验区块的结构、大小等信息)是一个合格的区块
         * ，如果进一步校验通过的话，那么这个区块就被允许添加进区块链了。
         * 如果区块不满足共识的要求，那么这个区块一定是一个非法的区块，非法的区块一定不能被添加进区块链。
         */
        bool checkConsensus(BlockchainDatabase *blockchainDatabase, Block *block) ;

        /**
         * 计算目标区块的挖矿难度
         */
        string calculateDifficult(BlockchainDatabase *blockchainDatabase, Block *block) ;
    };
}


#endif //HELLOWORLD_BLOCKCHAIN_CPP_CONSENSUS_H
