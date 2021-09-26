//
// Created by 40906 on 2021/9/20.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_BLOCKCHAINCOREFACTORY_H
#define HELLOWORLD_BLOCKCHAIN_CPP_BLOCKCHAINCOREFACTORY_H


#include "BlockchainCore.h"

namespace core{
    namespace BlockchainCoreFactory {
        BlockchainCore* createDefaultBlockchainCore();
        BlockchainCore* createBlockchainCore(string corePath);
    };
}


#endif //HELLOWORLD_BLOCKCHAIN_CPP_BLOCKCHAINCOREFACTORY_H
