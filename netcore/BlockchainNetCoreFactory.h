//
// Created by XINGKAICHUN on 2021/9/30.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_BLOCKCHAINNETCOREFACTORY_H
#define HELLOWORLD_BLOCKCHAIN_CPP_BLOCKCHAINNETCOREFACTORY_H

#include "BlockchainNetCore.h"



namespace netcore {


    namespace BlockchainNetCoreFactory{

        BlockchainNetCore *createDefaultBlockchainNetCore();
        BlockchainNetCore *createBlockchainNetCore(string netcorePath);
    }
};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_BLOCKCHAINNETCOREFACTORY_H
