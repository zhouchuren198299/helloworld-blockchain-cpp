//
// Created by 40906 on 2021/10/5.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_BLOCKCHAINBROWSERAPPLICATIONSERVICE_H
#define HELLOWORLD_BLOCKCHAIN_CPP_BLOCKCHAINBROWSERAPPLICATIONSERVICE_H

#include "../vo/transactionvo.h"
#include "../vo/walletvo.h"
#include "../../netcore/BlockchainNetCore.h"
#include "../vo/blockvo.h"

using namespace vo;

using namespace netcore;



namespace service {


    class BlockchainBrowserApplicationService {
    private:
        BlockchainNetCore *blockchainNetCore;
    public:
        BlockchainBrowserApplicationService(BlockchainNetCore *blockchainNetCore);
    public:
        TransactionOutputVo3 queryTransactionOutputByTransactionOutputId(string transactionHash, uint64_t transactionOutputIndex);
        TransactionOutputVo3 queryTransactionOutputByAddress(string address);
        TransactionVo queryTransactionByTransactionHash(string transactionHash);
        vector<TransactionVo> queryTransactionListByBlockHashTransactionHeight(string blockHash, uint64_t from, uint64_t size);
        BlockVo queryBlockViewByBlockHeight(uint64_t blockHeight);
        UnconfirmedTransactionVo queryUnconfirmedTransactionByTransactionHash(string transactionHash);
    };
};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_BLOCKCHAINBROWSERAPPLICATIONSERVICE_H
