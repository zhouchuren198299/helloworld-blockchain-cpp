//
// Created by 40906 on 2021/10/5.
//

#include "WalletApplicationService.h"
#include "../../netcoreclient/NodeClient.h"
#include "../../util/StringUtil.h"

using namespace netcoreclient;

namespace service {


    WalletApplicationService::WalletApplicationService(BlockchainNetCore *blockchainNetCore){
        this->blockchainNetCore = blockchainNetCore;
    }

    SubmitTransactionToBlockchainNetworkResponse WalletApplicationService::submitTransactionToBlockchainNetwork(SubmitTransactionToBlockchainNetworkRequest request){
        TransactionDto &transactionDto = request.transaction;
        //将交易提交到本地区块链
        blockchainNetCore->getBlockchainCore()->postTransaction(&transactionDto);
        //提交交易到网络
        vector<Node> nodes = blockchainNetCore->getNodeService()->queryAllNodes();
        vector<string> successSubmitNodes;
        vector<string> failedSubmitNodes;
        if(!nodes.empty()){
            for(Node node:nodes){
                PostTransactionRequest postTransactionRequest;
                postTransactionRequest.transaction=transactionDto;
                NodeClient nodeClient(node.ip);
                unique_ptr<PostTransactionResponse> postTransactionResponse = nodeClient.postTransaction(postTransactionRequest);
                if(postTransactionResponse.get()){
                    successSubmitNodes.push_back(node.ip);
                } else {
                    failedSubmitNodes.push_back(node.ip);
                }
            }
        }

        SubmitTransactionToBlockchainNetworkResponse response;
        response.transaction=transactionDto;
        response.successSubmitNodes=successSubmitNodes;
        response.failedSubmitNodes=failedSubmitNodes;
        return response;
    }
    AutomaticBuildTransactionResponse WalletApplicationService::automaticBuildTransaction(AutomaticBuildTransactionRequest request){
        AutoBuildTransactionRequest autoBuildTransactionRequest;
        autoBuildTransactionRequest.nonChangePayees=payeeVos2payees(request.nonChangePayees);

        AutoBuildTransactionResponse autoBuildTransactionResponse = blockchainNetCore->getBlockchainCore()->autoBuildTransaction(&autoBuildTransactionRequest);

        AutomaticBuildTransactionResponse response;
        response.buildTransactionSuccess=autoBuildTransactionResponse.buildTransactionSuccess;
        response.message=payAlert2PayAlertVo(autoBuildTransactionResponse.message);
        response.transactionHash=autoBuildTransactionResponse.transactionHash;
        response.fee=autoBuildTransactionResponse.fee;
        response.transaction=autoBuildTransactionResponse.transaction;
        response.payers=payers2payerVos(autoBuildTransactionResponse.payers);
        response.payees=payees2payeeVos(autoBuildTransactionResponse.payees);
        response.changePayee=payee2payeeVo(autoBuildTransactionResponse.changePayee);
        response.nonChangePayees=payees2payeeVos(autoBuildTransactionResponse.nonChangePayees);

        return response;
    }

    string WalletApplicationService::payAlert2PayAlertVo(string message) {
        if(StringUtil::equals(vo::BUILD_TRANSACTION_SUCCESS,message)){
            return vo::BUILD_TRANSACTION_SUCCESS;
        }else if(StringUtil::equals(vo::PAYEE_CAN_NOT_EMPTY,message)){
            return vo::PAYEE_CAN_NOT_EMPTY;
        }else if(StringUtil::equals(vo::PAYEE_VALUE_CAN_NOT_LESS_EQUAL_THAN_ZERO,message)){
            return vo::PAYEE_VALUE_CAN_NOT_LESS_EQUAL_THAN_ZERO;
        }else if(StringUtil::equals(vo::NOT_ENOUGH_MONEY_TO_PAY,message)){
            return vo::NOT_ENOUGH_MONEY_TO_PAY;
        }else if(StringUtil::equals(vo::PAYEE_ADDRESS_CAN_NOT_EMPTY,message)){
            return vo::PAYEE_ADDRESS_CAN_NOT_EMPTY;
        }else {
            throw exception();
        }
    }

    vector<Payee> WalletApplicationService::payeeVos2payees(vector<PayeeVo> payeeVos){
        vector<Payee> payees;
        if(!payeeVos.empty()){
            for (PayeeVo payeeVo:payeeVos) {
                Payee payee = payeeVo2payee(payeeVo);
                payees.push_back(payee);
            }
        }
        return payees;
    }
    Payee WalletApplicationService::payeeVo2payee(PayeeVo payeeVo){
        Payee payee;
        payee.address=payeeVo.address;
        payee.value=payeeVo.value;
        return payee;
    }

    vector<PayerVo> WalletApplicationService::payers2payerVos(vector<Payer> payers){
        vector<PayerVo> payerVos;
        if(!payers.empty()){
            for (Payer payer:payers) {
                PayerVo payerVo = payer2payerVo(payer);
                payerVos.push_back(payerVo);
            }
        }
        return payerVos;
    }
    PayerVo WalletApplicationService::payer2payerVo(Payer payer){
        PayerVo payerVo;
        payerVo.address=payer.address;
        payerVo.privateKey=payer.privateKey;
        payerVo.transactionHash=payer.transactionHash;
        payerVo.transactionOutputIndex=payer.transactionOutputIndex;
        payerVo.value=payer.value;
        return payerVo;
    }
    vector<PayeeVo> WalletApplicationService::payees2payeeVos(vector<Payee> payees){
        vector<PayeeVo> payeeVos;
        if(!payees.empty()){
            for (Payee payee:payees) {
                PayeeVo payeeVo = payee2payeeVo(payee);
                payeeVos.push_back(payeeVo);
            }
        }
        return payeeVos;
    }
    PayeeVo WalletApplicationService::payee2payeeVo(Payee payee){
        PayeeVo payeeVo;
        payeeVo.address=payee.address;
        payeeVo.value=payee.value;
        return payeeVo;
    }
};