//
// Created by 40906 on 2021/9/18.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_DTO_H
#define HELLOWORLD_BLOCKCHAIN_CPP_DTO_H

#include <string>
#include <cstdint>
#include <vector>
#include <stack>
#include "../thirdpart/nlohmann/json.hpp"

using json = nlohmann::json;

using namespace std;

namespace dto {

    typedef vector<string> ScriptDto;
    typedef ScriptDto InputScriptDto;
    typedef ScriptDto OutputScriptDto;
    struct TransactionInputDto{
        string transactionHash;
        uint64_t transactionOutputIndex;
        InputScriptDto inputScript;
    };
    struct TransactionOutputDto{
        OutputScriptDto outputScript;
        uint64_t value;
    };
    struct TransactionDto{
        vector<TransactionInputDto> inputs;
        vector<TransactionOutputDto> outputs;
    };
    struct BlockDto{
        uint64_t timestamp;
        string previousHash;
        vector<TransactionDto> transactions;
        string nonce;
    };

    void to_json(json& j, const TransactionInputDto& p);
    void from_json(const json& j, TransactionInputDto& p);

    void to_json(json& j, const TransactionOutputDto& p);
    void from_json(const json& j, TransactionOutputDto& p);

    void to_json(json& j, const TransactionDto& p);
    void from_json(const json& j, TransactionDto& p);

    void to_json(json& j, const BlockDto& p);
    void from_json(const json& j, BlockDto& p);
};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_DTO_H
