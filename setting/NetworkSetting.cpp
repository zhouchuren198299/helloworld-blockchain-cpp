//
// Created by 40906 on 2021/9/20.
//

#include "NetworkSetting.h"

#include <cstdint>
#include <string>
#include <vector>

using namespace std;

namespace NetworkSetting {

    //区块链网络使用的端口
    uint64_t PORT = 8888;
    //区块链网络中的种子节点
    vector<string> SEED_NODES{"139.9.125.122","119.3.57.171"};
};
