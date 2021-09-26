//
// Created by 40906 on 2021/9/20.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_INCENTIVESETTING_H
#define HELLOWORLD_BLOCKCHAIN_CPP_INCENTIVESETTING_H


#include <cstdint>

namespace IncentiveSetting {

    //挖出一个区块的期望耗时时间(单位：毫秒)
    extern uint64_t BLOCK_TIME;
    //一个挖矿难度周期内的区块数量
    extern uint64_t INTERVAL_BLOCK_COUNT;
    //一个挖矿周期内的期望周期耗时时间(单位：毫秒)
    extern uint64_t INTERVAL_TIME;
    //区块初始奖励
    extern uint64_t BLOCK_INIT_INCENTIVE;
    //激励金额减半周期：每210000个区块激励减半
    extern uint64_t INCENTIVE_HALVING_INTERVAL;
};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_INCENTIVESETTING_H
