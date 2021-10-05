//
// Created by 40906 on 2021/10/5.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_BLOCKCHAINBROWSERAPPLICATIONCONTROLLER_H
#define HELLOWORLD_BLOCKCHAIN_CPP_BLOCKCHAINBROWSERAPPLICATIONCONTROLLER_H

#include "../../thirdpart/httplib/httplib.h"
#include "../../netcore/BlockchainNetCore.h"
#include "../service/BlockchainBrowserApplicationService.h"
#include "../service/WalletApplicationService.h"

using namespace netcore;
using namespace service;



namespace controller{



    class BlockchainBrowserApplicationController {
    private:
        BlockchainNetCore *blockchainNetCore;
        BlockchainBrowserApplicationService *blockchainBrowserApplicationService;
    public:
        BlockchainBrowserApplicationController(BlockchainNetCore *blockchainNetCore,BlockchainBrowserApplicationService *blockchainBrowserApplicationService);
    public:
        void queryTransactionByTransactionHash(const httplib::Request &req, httplib::Response &res);
        void queryTransactionsByBlockHashTransactionHeight(const httplib::Request &req, httplib::Response &res);
        void queryTransactionOutputByAddress(const httplib::Request &req, httplib::Response &res);
        void queryTransactionOutputByTransactionOutputId(const httplib::Request &req, httplib::Response &res);
        void queryBlockchainHeight(const httplib::Request &req, httplib::Response &res);
        void queryUnconfirmedTransactionByTransactionHash(const httplib::Request &req, httplib::Response &res);
        void queryUnconfirmedTransactions(const httplib::Request &req, httplib::Response &res);
        void queryBlockByBlockHeight(const httplib::Request &req, httplib::Response &res);
        void queryBlockByBlockHash(const httplib::Request &req, httplib::Response &res);
        void queryTop10Blocks(const httplib::Request &req, httplib::Response &res);
    };
}


#endif //HELLOWORLD_BLOCKCHAIN_CPP_BLOCKCHAINBROWSERAPPLICATIONCONTROLLER_H
