//
// Created by 40906 on 2021/10/5.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_WALLETAPPLICATIONCONTROLLER_H
#define HELLOWORLD_BLOCKCHAIN_CPP_WALLETAPPLICATIONCONTROLLER_H


#include "../../thirdpart/httplib/httplib.h"
#include "../../netcore/BlockchainNetCore.h"
#include "../service/BlockchainBrowserApplicationService.h"
#include "../service/WalletApplicationService.h"

using namespace netcore;
using namespace service;

namespace controller{




    class WalletApplicationController {
    private:
        BlockchainNetCore *blockchainNetCore;
        WalletApplicationService *walletApplicationService;
    public:
        WalletApplicationController(BlockchainNetCore *blockchainNetCore,WalletApplicationService *walletApplicationService);

    public:
        void createAccount(const httplib::Request &req, httplib::Response &res);
        void createAndSaveAccount(const httplib::Request &req, httplib::Response &res);
        void saveAccount(const httplib::Request &req, httplib::Response &res);
        void deleteAccount(const httplib::Request &req, httplib::Response &res);
        void queryAllAccounts(const httplib::Request &req, httplib::Response &res);
        void automaticBuildTransaction(const httplib::Request &req, httplib::Response &res);
        void submitTransactionToBlockchainNetwork(const httplib::Request &req, httplib::Response &res);
    };
}


#endif //HELLOWORLD_BLOCKCHAIN_CPP_WALLETAPPLICATIONCONTROLLER_H
