//
// Created by 40906 on 2021/10/5.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_WALLETAPPLICATIONSERVICE_H
#define HELLOWORLD_BLOCKCHAIN_CPP_WALLETAPPLICATIONSERVICE_H


#include "../vo/transactionvo.h"
#include "../vo/walletvo.h"
#include "../../netcore/BlockchainNetCore.h"

using namespace vo;
using namespace netcore;

namespace service {

    class WalletApplicationService{
    private:
        BlockchainNetCore *blockchainNetCore;
    public:
        WalletApplicationService::WalletApplicationService(BlockchainNetCore *blockchainNetCore);

    private:
        string payAlert2PayAlertVo(string message);
        vector<Payee> payeeVos2payees(vector<PayeeVo> payeeVos);
        Payee payeeVo2payee(PayeeVo payeeVo);
        vector<PayerVo> payers2payerVos(vector<Payer> payers);
        PayerVo payer2payerVo(Payer payer);
        vector<PayeeVo> payees2payeeVos(vector<Payee> payees);
        PayeeVo payee2payeeVo(Payee payee);
    public:
        SubmitTransactionToBlockchainNetworkResponse submitTransactionToBlockchainNetwork(SubmitTransactionToBlockchainNetworkRequest request);
        AutomaticBuildTransactionResponse automaticBuildTransaction(AutomaticBuildTransactionRequest request);

    };
};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_WALLETAPPLICATIONSERVICE_H
