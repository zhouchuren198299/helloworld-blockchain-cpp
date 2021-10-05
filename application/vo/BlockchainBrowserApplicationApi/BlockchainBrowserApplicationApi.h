//
// Created by 40906 on 2021/10/4.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_BLOCKCHAINBROWSERAPPLICATIONAPI_H
#define HELLOWORLD_BLOCKCHAIN_CPP_BLOCKCHAINBROWSERAPPLICATIONAPI_H

#include <string>
using namespace std;


namespace BlockchainBrowserApplicationApi {




//查询区块链高度
    const string QUERY_BLOCKCHAIN_HEIGHT = "/Api/BlockchainBrowserApplication/QueryBlockchainHeight";

//根据交易哈希查询交易
    const string QUERY_TRANSACTION_BY_TRANSACTION_HASH = "/Api/BlockchainBrowserApplication/QueryTransactionByTransactionHash";

//根据区块哈希与交易高度查询交易列表
    const string QUERY_TRANSACTIONS_BY_BLOCK_HASH_TRANSACTION_HEIGHT = "/Api/BlockchainBrowserApplication/QueryTransactionsByBlockHashTransactionHeight";

//根据地址获取交易输出
    const string QUERY_TRANSACTION_OUTPUT_BY_ADDRESS = "/Api/BlockchainBrowserApplication/QueryTransactionOutputByAddress";

//根据交易输出ID获取交易输出
    const string QUERY_TRANSACTION_OUTPUT_BY_TRANSACTION_OUTPUT_ID = "/Api/BlockchainBrowserApplication/QueryTransactionOutputByTransactionOutputId";

//查询未确认交易
    const string QUERY_UNCONFIRMED_TRANSACTIONS = "/Api/BlockchainBrowserApplication/QueryUnconfirmedTransactions";

//根据交易哈希查询未确认交易
    const string QUERY_UNCONFIRMED_TRANSACTION_BY_TRANSACTION_HASH = "/Api/BlockchainBrowserApplication/QueryUnconfirmedTransactionByTransactionHash";

//根据区块高度查询区块
    const string QUERY_BLOCK_BY_BLOCK_HEIGHT = "/Api/BlockchainBrowserApplication/QueryBlockByBlockHeight";

//根据区块哈希查询区块
    const string QUERY_BLOCK_BY_BLOCK_HASH = "/Api/BlockchainBrowserApplication/QueryBlockByBlockHash";

//查询最近的10个区块
    const string QUERY_TOP10_BLOCKS = "/Api/BlockchainBrowserApplication/QueryTop10Blocks";

};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_BLOCKCHAINBROWSERAPPLICATIONAPI_H
