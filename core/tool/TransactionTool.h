//
// Created by 40906 on 2021/9/21.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_TRANSACTIONTOOL_H
#define HELLOWORLD_BLOCKCHAIN_CPP_TRANSACTIONTOOL_H


#include "../model/model.h"

using namespace model;

namespace TransactionTool {
    /**
      * 交易输入总额
      */
    uint64_t getInputValue(Transaction *transaction) ;



    /**
     * 交易输出总额
     */
    uint64_t getOutputValue(Transaction *transaction) ;


    /**
     * 交易手续费（创世交易交易手续费是0）
     */
    uint64_t getTransactionFee(Transaction *transaction) ;
    /**
     * 交易费率（创世交易交易手续费率是0）
     */
    uint64_t getTransactionFeeRate(Transaction *transaction) ;


    /**
     * 获取待签名数据
     */
    string signatureHashAll(Transaction *transaction) ;
    /**
     * 交易签名
     */
    string signature(string privateKey, Transaction *transaction) ;

    /**
     * 计算交易哈希
     */
    string calculateTransactionHash(Transaction *transaction) ;






    /**
     * 交易中的金额是否符合系统的约束
     */
    bool checkTransactionValue(Transaction *transaction) ;

    /**
     * 校验交易中的地址是否是P2PKH地址
     */
    bool checkPayToPublicKeyHashAddress(Transaction *transaction)  ;

    /**
     * 交易中是否存在重复的[未花费交易输出]
     */
    bool isExistDuplicateUtxo(Transaction *transaction)  ;
    /**
     * 区块新产生的地址是否存在重复
     */
    bool isExistDuplicateNewAddress(Transaction *transaction)  ;

    uint64_t getTransactionInputCount(Transaction *transaction)  ;

    uint64_t getTransactionOutputCount(Transaction *transaction)  ;

    uint64_t calculateTransactionFee(Transaction *transaction)  ;

    /**
     * 按照费率(每字符的手续费)从大到小排序交易
     */
    void sortByTransactionFeeRateDescend(vector<Transaction> *transactions)  ;

    /**
     * 校验交易中的脚本是否是P2PKH脚本
     */
    bool checkPayToPublicKeyHashScript(Transaction *transaction)  ;

    string getTransactionOutputId(TransactionOutput *transactionOutput)  ;

    /**
     * 验证签名
     */
    bool verifySignature(Transaction transaction, string publicKey, vector<unsigned char> *bytesSignature)  ;

    /**
     * 校验交易金额是否是一个合法的交易金额：这里用于限制交易金额的最大值、最小值、小数保留位等
     */
    bool checkValue(uint64_t transactionAmount) ;
};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_TRANSACTIONTOOL_H
