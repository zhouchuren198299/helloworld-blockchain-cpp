//
// Created by 40906 on 2021/10/4.
//

#include "frameworkvo.h"



namespace vo {


    void to_json(json& j, const PageCondition& p){
        j = json{
                { "from", p.from },
                { "size", p.size }
        };
    }
    void from_json(const json& j, PageCondition& p){
        j.at("from").get_to(p.from);
        j.at("size").get_to(p.size);
    }
}