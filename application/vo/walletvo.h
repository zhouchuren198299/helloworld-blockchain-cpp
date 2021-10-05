//
// Created by 40906 on 2021/10/4.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_WALLETVO_H
#define HELLOWORLD_BLOCKCHAIN_CPP_WALLETVO_H

#include <cstdint>
#include <string>
#include <vector>
#include "../../dto/dto.h"
#include "../../thirdpart/nlohmann/json.hpp"

using namespace std;
using namespace dto;
using json = nlohmann::json;



namespace vo {

    struct PayerVo {
        string privateKey;
        string transactionHash;
        uint64_t transactionOutputIndex;
        uint64_t value;
        string address;
    };
    struct PayeeVo {
        string address;
        uint64_t value;
    };

    struct AutomaticBuildTransactionRequest {
        vector<PayeeVo> nonChangePayees;
    };
    struct AutomaticBuildTransactionResponse {
        bool buildTransactionSuccess;
        string message;
        string transactionHash;
        uint64_t fee;
        vector<PayerVo> payers;
        vector<PayeeVo> nonChangePayees;
        PayeeVo changePayee;
        vector<PayeeVo> payees;
        TransactionDto transaction;
    };

    //PayAlert
    const string PAYEE_CAN_NOT_EMPTY = "payee_can_not_empty";
    const string PAYEE_ADDRESS_CAN_NOT_EMPTY = "payee_address_can_not_empty";
    const string PAYEE_VALUE_CAN_NOT_LESS_EQUAL_THAN_ZERO = "payee_value_can_not_less_equal_than_zero";
    const string NOT_ENOUGH_MONEY_TO_PAY = "not_enough_money_to_pay";
    const string BUILD_TRANSACTION_SUCCESS = "build_transaction_success";


    void to_json(json& j, const PayerVo& p);
    void from_json(const json& j, PayerVo& p);

    void to_json(json& j, const PayeeVo& p);
    void from_json(const json& j, PayeeVo& p);

    void to_json(json& j, const AutomaticBuildTransactionRequest& p);
    void from_json(const json& j, AutomaticBuildTransactionRequest& p);
    void to_json(json& j, const AutomaticBuildTransactionResponse& p);
    void from_json(const json& j, AutomaticBuildTransactionResponse& p);
};

#endif //HELLOWORLD_BLOCKCHAIN_CPP_WALLETVO_H