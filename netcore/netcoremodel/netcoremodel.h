//
// Created by 40906 on 2021/9/28.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_NETCOREMODEL_H
#define HELLOWORLD_BLOCKCHAIN_CPP_NETCOREMODEL_H

#include <string>
#include <cstdint>
using namespace std;

namespace netcoremodel {
    struct Node {
        string ip;
        uint64_t blockchainHeight;
    };
};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_NETCOREMODEL_H
