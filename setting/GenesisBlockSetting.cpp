//
// Created by 40906 on 2021/9/20.
//

#include "GenesisBlockSetting.h"
#include <cstdint>
#include <string>

using namespace std;

namespace GenesisBlockSetting {
    //创世区块的高度
    uint64_t HEIGHT = 0;
    //创世区块的哈希
    string HASH = "0000000000000000000000000000000000000000000000000000000000000000";
    //创世区块的挖矿难度
    string DIFFICULTY = "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff";
};