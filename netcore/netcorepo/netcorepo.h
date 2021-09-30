//
// Created by 40906 on 2021/9/28.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_NETCOREPO_H
#define HELLOWORLD_BLOCKCHAIN_CPP_NETCOREPO_H

#include <string>
#include <cstdint>
#include "../../thirdpart/nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

namespace netcorepo {
    struct NodePo {
        string ip;
        uint64_t blockchainHeight;
    };

    void to_json(json& j, const NodePo& p);
    void from_json(const json& j, NodePo& p);
};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_NETCOREPO_H
