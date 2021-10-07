//
// Created by 40906 on 2021/9/18.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_TRANSACTIONDTOTOOL_H
#define HELLOWORLD_BLOCKCHAIN_CPP_TRANSACTIONDTOTOOL_H

#include "../../netcoredto/netcoredto.h"

using namespace std;
using namespace dto;

namespace TransactionDtoTool {
    /**
     * 获取待签名数据
     */
    string signatureHashAll(TransactionDto &transactionDto);

    /**
     * 交易签名
     */
    string signature(string &privateKey, TransactionDto &transactionDto);


    string calculateTransactionHash(TransactionDto &transactionDto);

    //region 序列化与反序列化
    /**
     * 序列化。将交易转换为字节数组，要求生成的字节数组反过来能还原为原始交易。
     */
    vector<unsigned char> bytesTransaction(TransactionDto &transactionDto, bool &omitInputScript);
    bool verifySignature(TransactionDto &transaction, string &publicKey, vector<unsigned char> &bytesSignature) ;
    //endregion
}

    



#endif //HELLOWORLD_BLOCKCHAIN_CPP_TRANSACTIONDTOTOOL_H