//
// Created by 40906 on 2021/9/16.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_MODEL_H
#define HELLOWORLD_BLOCKCHAIN_CPP_MODEL_H

#include <string>
#include <cstdint>
#include <vector>
#include <stack>
using namespace std;

namespace model {

    namespace BlockchainActionEnum
    {
        typedef string BlockchainAction;
        BlockchainAction ADD_BLOCK;
        BlockchainAction DELETE_BLOCK;
    }



    namespace BooleanCodeEnum
    {
        struct BooleanCode{
            vector<unsigned char> code;
            string name;
        };
        BooleanCode FALSE{{0x00},"false"};
        BooleanCode TRUE{{0x01},"true"};
    }
    namespace OperationCodeEnum
    {
        struct OperationCode{
            vector<unsigned char> code;
            string name;
        };
        OperationCode OP_PUSHDATA{{0x00},"OP_PUSHDATA"};
        OperationCode OP_DUP{{0x01},"OP_DUP"};
        OperationCode OP_HASH160{{0x02},"OP_HASH160"};
        OperationCode OP_EQUALVERIFY{{0x03},"OP_EQUALVERIFY"};
        OperationCode OP_CHECKSIG{{0x04},"OP_CHECKSIG"};
    }
    typedef vector<string> Script;
    typedef Script InputScript;
    typedef Script OutputScript;
    typedef stack<string> ScriptExecuteResult;



    class TransactionOutput;
    class TransactionInput;
    class Transaction;
    namespace TransactionTypeEnum
    {
        typedef string TransactionType;
        TransactionType GENESIS_TRANSACTION;
        TransactionType STANDARD_TRANSACTION;
    }
    class Transaction{
        string transactionHash;
        TransactionTypeEnum::TransactionType transactionType;
        vector<TransactionInput> inputs;
        vector<TransactionOutput> outputs;
        long transactionIndex;
        long transactionHeight;
        long blockHeight;
    };
    class TransactionInput{
        TransactionOutput *unspentTransactionOutput;
        InputScript inputScript;
    };
    class TransactionOutput{
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
    class Block{
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
};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_MODEL_H
