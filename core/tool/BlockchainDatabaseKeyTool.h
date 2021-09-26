//
// Created by 40906 on 2021/9/20.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_BLOCKCHAINDATABASEKEYTOOL_H
#define HELLOWORLD_BLOCKCHAIN_CPP_BLOCKCHAINDATABASEKEYTOOL_H

#include <string>
#include <vector>
using namespace std;

namespace BlockchainDatabaseKeyTool {




    //拼装数据库Key的值
    vector<unsigned char> buildBlockchainHeightKey()  ;
    vector<unsigned char> buildHashKey(string hash)  ;
    vector<unsigned char> buildAddressKey(string address)  ;
    vector<unsigned char> buildBlockHeightToBlockKey(uint64_t blockHeight)  ;
    vector<unsigned char> buildBlockHashToBlockHeightKey(string blockHash)  ;
    vector<unsigned char> buildTransactionHashToTransactionHeightKey(string transactionHash) ;
    vector<unsigned char> buildTransactionOutputHeightToTransactionOutputKey(uint64_t transactionOutputHeight)  ;
    vector<unsigned char> buildTransactionOutputIdToTransactionOutputHeightKey(string transactionHash,uint64_t transactionOutputIndex)  ;
    vector<unsigned char> buildTransactionOutputIdToUnspentTransactionOutputHeightKey(string transactionHash,uint64_t transactionOutputIndex) ;
    vector<unsigned char> buildTransactionOutputIdToSourceTransactionHeightKey(string transactionHash,uint64_t transactionOutputIndex)  ;
    vector<unsigned char> buildTransactionOutputIdToDestinationTransactionHeightKey(string transactionHash,uint64_t transactionOutputIndex)  ;
    vector<unsigned char> buildAddressToTransactionOutputHeightKey(string address)  ;
    vector<unsigned char> buildAddressToUnspentTransactionOutputHeightKey(string address) ;
    vector<unsigned char> buildAddressToSpentTransactionOutputHeightKey(string address)  ;
    vector<unsigned char> buildBlockchainTransactionHeightKey()  ;
    vector<unsigned char> buildBlockchainTransactionOutputHeightKey()  ;
    vector<unsigned char> buildTransactionHeightToTransactionKey(uint64_t transactionHeight) ;
    vector<unsigned char> buildTransactionOutputIdToSpentTransactionOutputHeightKey(string transactionHash,uint64_t transactionOutputIndex)  ;
    string buildTransactionOutputId(string transactionHash,uint64_t transactionOutputIndex) ;
};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_BLOCKCHAINDATABASEKEYTOOL_H
