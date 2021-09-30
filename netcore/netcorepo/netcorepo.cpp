//
// Created by 40906 on 2021/9/28.
//

#include "netcorepo.h"


namespace netcorepo {

    void to_json(json& j, const NodePo& p){
        j = json{
                { "ip", p.ip },
                { "blockchainHeight", p.blockchainHeight }
        };
    }
    void from_json(const json& j, NodePo& p){
        j.at("ip").get_to(p.ip);
        j.at("blockchainHeight").get_to(p.blockchainHeight);
    }
};

