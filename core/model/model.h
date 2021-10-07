//
// Created by 40906 on 2021/9/16.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_MODEL_H
#define HELLOWORLD_BLOCKCHAIN_CPP_MODEL_H

#include <string>
#include <cstdint>
#include <vector>
#include <stack>
#include "../../netcoredto/netcoredto.h"

using namespace std;
using namespace dto;

namespace model {

    namespace BlockchainActionEnum
    {
        typedef bool BlockchainAction;
        extern BlockchainAction ADD_BLOCK;
        extern BlockchainAction DELETE_BLOCK;
    }




    namespace BooleanCodeEnum
    {
        struct BooleanCode{
            vector<unsigned char> code;
            string name;
        };
        extern BooleanCode FALSE00;
        extern BooleanCode TRUE00;
    }
    namespace OperationCodeEnum
    {
        struct OperationCode{
            vector<unsigned char> code;
            string name;
        };
        extern OperationCode OP_PUSHDATA;
        extern OperationCode OP_DUP;
        extern OperationCode OP_HASH160;
        extern OperationCode OP_EQUALVERIFY;
        extern OperationCode OP_CHECKSIG;
    }
    typedef vector<string> Script;
    typedef Script InputScript;
    typedef Script OutputScript;
    typedef stack<string> ScriptExecuteResult;



    struct TransactionOutput;
    struct TransactionInput;
    struct Transaction;
    namespace TransactionTypeEnum
    {
        typedef string TransactionType;
        extern TransactionType GENESIS_TRANSACTION;
        extern TransactionType STANDARD_TRANSACTION;
    }
    struct Transaction{
        string transactionHash;
        TransactionTypeEnum::TransactionType transactionType;
        vector<TransactionInput> inputs;
        vector<TransactionOutput> outputs;
        uint64_t transactionIndex;
        uint64_t transactionHeight;
        uint64_t blockHeight;
    };
    struct TransactionOutput{
        uint64_t value;
        OutputScript outputScript;
        string transactionHash;
        uint64_t transactionOutputIndex;
        string address;
        uint64_t blockHeight;
        string blockHash;
        uint64_t transactionHeight;
        uint64_t transactionIndex;
        uint64_t transactionOutputHeight;
    };
    struct TransactionInput{
        TransactionOutput unspentTransactionOutput;
        InputScript inputScript;
    };
    struct Block{
        uint64_t timestamp;
        uint64_t height;
        string previousHash;
        vector<Transaction> transactions;
        string merkleTreeRoot;
        string nonce;
        string hash;
        string difficulty;
        uint64_t transactionCount;
        uint64_t previousTransactionHeight;
    };


    struct Payer{
        string privateKey;
        string transactionHash;
        uint64_t transactionOutputIndex;
        uint64_t value;
        string address;
    };
    struct Payee{
        string address;
        uint64_t value;
    };
    struct AutoBuildTransactionRequest{
        vector<Payee> nonChangePayees;
    };
    struct AutoBuildTransactionResponse{
        bool buildTransactionSuccess;
        string message;
        string transactionHash;
        uint64_t fee;
        vector<Payer> payers;
        vector<Payee> nonChangePayees;
        Payee changePayee;
        vector<Payee> payees;
        TransactionDto transaction;
    };
    namespace PayAlert {
        const string PAYEE_CAN_NOT_EMPTY = "payee_can_not_empty";
        const string PAYEE_ADDRESS_CAN_NOT_EMPTY = "payee_address_can_not_empty";
        const string PAYEE_VALUE_CAN_NOT_LESS_EQUAL_THAN_ZERO = "payee_value_can_not_less_equal_than_zero";
        const string NOT_ENOUGH_MONEY_TO_PAY = "not_enough_money_to_pay";
        const string BUILD_TRANSACTION_SUCCESS = "build_transaction_success";
    }

    void to_json(json& j, const TransactionOutput& p);
    void from_json(const json& j, TransactionOutput& p);

    void to_json(json& j, const TransactionInput& p);
    void from_json(const json& j, TransactionInput& p);

    void to_json(json& j, const Transaction& p);
    void from_json(const json& j, Transaction& p);

    void to_json(json& j, const Block& p);
    void from_json(const json& j, Block& p);
};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_MODEL_H
