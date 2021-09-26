//
// Created by 40906 on 2021/9/20.
//

#include "UnconfirmedTransactionDatabase.h"
#include "CoreConfiguration.h"
#include "../util/FileUtil.h"
#include "../util/ByteUtil.h"
#include "tool/TransactionDtoTool.h"
#include "../util/EncodeDecodeTool.h"
#include "../util/KvDbUtil.h"
#include "tool/NullTool.h"

namespace core{

    const string UNCONFIRMED_TRANSACTION_DATABASE_NAME = "UnconfirmedTransactionDatabase";

    bool UnconfirmedTransactionDatabase::insertTransaction(TransactionDto transaction) {
        string transactionHash = TransactionDtoTool::calculateTransactionHash(transaction);
        KvDbUtil::put(getUnconfirmedTransactionDatabasePath(), getKey(transactionHash), EncodeDecodeTool::encode(transaction));
        return true;
    }
    vector<TransactionDto> UnconfirmedTransactionDatabase::selectTransactions(uint64_t from, uint64_t size) {
        vector<TransactionDto> transactionDtos;
        vector<vector<unsigned char>> bytesTransactionDtos = KvDbUtil::gets(getUnconfirmedTransactionDatabasePath(),from,size);
        if(!bytesTransactionDtos.empty()){
            for(vector<unsigned char> bytesTransactionDto:bytesTransactionDtos){
                TransactionDto transactionDto = EncodeDecodeTool::decode(bytesTransactionDto,TransactionDto{});
                transactionDtos.push_back(transactionDto);
            }
        }
        return transactionDtos;
    }
    void UnconfirmedTransactionDatabase::deleteByTransactionHash(string transactionHash) {
        KvDbUtil::delete0(getUnconfirmedTransactionDatabasePath(), getKey(transactionHash));
    }
    TransactionDto UnconfirmedTransactionDatabase::selectTransactionByTransactionHash(string transactionHash){
        vector<unsigned char> byteTransactionDto = KvDbUtil::get(getUnconfirmedTransactionDatabasePath(), getKey(transactionHash));
        if(byteTransactionDto.empty()){
            return NullTool::newNullTransactionDto();
        }
        return EncodeDecodeTool::decode(byteTransactionDto,TransactionDto{});
    }




    string UnconfirmedTransactionDatabase::getUnconfirmedTransactionDatabasePath() {
        return FileUtil::newPath(coreConfiguration->getCorePath(), UNCONFIRMED_TRANSACTION_DATABASE_NAME);
    }
    vector<unsigned char> UnconfirmedTransactionDatabase::getKey(string transactionHash){
        return ByteUtil::hexStringToBytes(transactionHash);
    }

    UnconfirmedTransactionDatabase::UnconfirmedTransactionDatabase(CoreConfiguration *coreConfiguration) {
        this->coreConfiguration = coreConfiguration;
    }
}