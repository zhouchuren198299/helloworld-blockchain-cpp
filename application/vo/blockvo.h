//
// Created by 40906 on 2021/10/4.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_BLOCKVO_H
#define HELLOWORLD_BLOCKCHAIN_CPP_BLOCKVO_H

#include <cstdint>
#include <string>
#include <vector>
#include "../../thirdpart/nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;



namespace vo {

    struct BlockVo {
        uint64_t height;
        uint64_t confirmCount;
        string blockSize;
        uint64_t transactionCount;
        string time;
        uint64_t minerIncentiveValue;

        string difficulty;
        string nonce;
        string hash;
        string previousBlockHash;
        string nextBlockHash;
        string merkleTreeRoot;
    };
    struct BlockVo2 {
        uint64_t height;
        string blockSize;
        uint64_t transactionCount;
        uint64_t minerIncentiveValue;
        string time;
        string hash;
    };
    struct DeleteBlocksRequest {
        uint64_t blockHeight;
    };
    struct DeleteBlocksResponse {};

    struct QueryBlockByBlockHashRequest {
        string blockHash;
    };
    struct QueryBlockByBlockHashResponse {
        BlockVo block;
    };

    struct QueryBlockByBlockHeightRequest {
        uint64_t blockHeight;
    };
    struct QueryBlockByBlockHeightResponse {
        BlockVo block;
    };

    struct QueryTop10BlocksRequest {};
    struct QueryTop10BlocksResponse {
        vector<BlockVo2> blocks;
    };



    void to_json(json& j, const BlockVo& p);
    void from_json(const json& j, BlockVo& p);

    void to_json(json& j, const BlockVo2& p);
    void from_json(const json& j, BlockVo2& p);

    void to_json(json& j, const DeleteBlocksRequest& p);
    void from_json(const json& j, DeleteBlocksRequest& p);
    void to_json(json& j, const DeleteBlocksResponse& p);
    void from_json(const json& j, DeleteBlocksResponse& p);

    void to_json(json& j, const QueryBlockByBlockHashRequest& p);
    void from_json(const json& j, QueryBlockByBlockHashRequest& p);
    void to_json(json& j, const QueryBlockByBlockHashResponse& p);
    void from_json(const json& j, QueryBlockByBlockHashResponse& p);

    void to_json(json& j, const QueryBlockByBlockHeightRequest& p);
    void from_json(const json& j, QueryBlockByBlockHeightRequest& p);
    void to_json(json& j, const QueryBlockByBlockHeightResponse& p);
    void from_json(const json& j, QueryBlockByBlockHeightResponse& p);

    void to_json(json& j, const QueryTop10BlocksRequest& p);
    void from_json(const json& j, QueryTop10BlocksRequest& p);
    void to_json(json& j, const QueryTop10BlocksResponse& p);
    void from_json(const json& j, QueryTop10BlocksResponse& p);
};

#endif //HELLOWORLD_BLOCKCHAIN_CPP_BLOCKVO_H