//
// Created by 40906 on 2021/9/20.
//

#include "IncentiveSetting.h"
#include <cstdint>

namespace IncentiveSetting {

    //挖出一个区块的期望耗时时间(单位：毫秒)
    uint64_t BLOCK_TIME = 1000 * 60 * 10;
    //一个挖矿难度周期内的区块数量
    uint64_t INTERVAL_BLOCK_COUNT = 6 * 24 * 7 * 2;
    //一个挖矿周期内的期望周期耗时时间(单位：毫秒)
    uint64_t INTERVAL_TIME = BLOCK_TIME * INTERVAL_BLOCK_COUNT;
    //区块初始奖励
    uint64_t BLOCK_INIT_INCENTIVE = uint64_t(50) * uint64_t(100000000);
    //激励金额减半周期：每210000个区块激励减半
    uint64_t INCENTIVE_HALVING_INTERVAL = 210000L;
};