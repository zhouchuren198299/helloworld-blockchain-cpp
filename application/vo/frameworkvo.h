//
// Created by 40906 on 2021/10/4.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_FRAMEWORKVO_H
#define HELLOWORLD_BLOCKCHAIN_CPP_FRAMEWORKVO_H

#include <cstdint>
#include <string>
#include <vector>
#include "../../thirdpart/nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;



namespace vo {

    struct PageCondition {
         uint64_t from;
         uint64_t size;
    };

    void to_json(json& j, const PageCondition& p);
    void from_json(const json& j, PageCondition& p);
};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_FRAMEWORKVO_H
