//
// Created by 40906 on 2021/9/20.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_GENESISBLOCKSETTING_H
#define HELLOWORLD_BLOCKCHAIN_CPP_GENESISBLOCKSETTING_H


#include <cstdint>
#include <string>

using namespace std;

namespace GenesisBlockSetting {
    //创世区块的高度
    extern uint64_t HEIGHT;
    //创世区块的哈希
    extern string HASH;
    //创世区块的挖矿难度
    extern string DIFFICULTY;
};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_GENESISBLOCKSETTING_H
