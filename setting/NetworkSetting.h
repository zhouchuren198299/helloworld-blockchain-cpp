//
// Created by 40906 on 2021/9/20.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_NETWORKSETTING_H
#define HELLOWORLD_BLOCKCHAIN_CPP_NETWORKSETTING_H


#include <cstdint>
#include <string>
#include <vector>

using namespace std;

namespace NetworkSetting {

    //区块链网络使用的端口
    extern uint64_t PORT;
    //区块链网络中的种子节点
    extern vector<string> SEED_NODES;
};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_NETWORKSETTING_H
