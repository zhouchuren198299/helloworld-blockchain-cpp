//
// Created by 40906 on 2021/9/21.
//

#include "TransactionTool.h"
#include "ScriptTool.h"
#include "Model2DtoTool.h"
#include "BlockchainDatabaseKeyTool.h"
#include "../../crypto/AccountUtil.h"
#include "../../util/LogUtil.h"
#include "TransactionDtoTool.h"
#include "../../util/SystemUtil.h"
#include "SizeTool.h"
#include "../../util/StringsUtil.h"


namespace TransactionTool{

    /**
      * 交易输入总额
      */
    uint64_t getInputValue(Transaction *transaction) {
        vector<TransactionInput> inputs = transaction->inputs;
        uint64_t total = 0;
        if(!inputs.empty()){
            for(TransactionInput input : inputs) {
                total += input.unspentTransactionOutput.value;
            }
        }
        return total;
    }



    /**
     * 交易输出总额
     */
    uint64_t getOutputValue(Transaction *transaction) {
        vector<TransactionOutput> outputs = transaction->outputs;
        uint64_t total = 0;
        if(!outputs.empty()){
            for(TransactionOutput output : outputs) {
                total += output.value;
            }
        }
        return total;
    }


    /**
     * 交易手续费（创世交易交易手续费是0）
     */
    uint64_t getTransactionFee(Transaction *transaction) {
        if(transaction->transactionType == TransactionTypeEnum::STANDARD_TRANSACTION){
            uint64_t transactionFee = getInputValue(transaction) - getOutputValue(transaction);
            return transactionFee;
        }else if(transaction->transactionType == TransactionTypeEnum::GENESIS_TRANSACTION){
            return 0L;
        }else{
            throw exception();
        }
    }
    /**
     * 交易费率（创世交易交易手续费率是0）
     */
    uint64_t getTransactionFeeRate(Transaction *transaction) {
        if(transaction->transactionType == TransactionTypeEnum::STANDARD_TRANSACTION){
            return getTransactionFee(transaction)/SizeTool::calculateTransactionSize(transaction);
        }else if(transaction->transactionType == TransactionTypeEnum::GENESIS_TRANSACTION){
            return 0L;
        }else {
            throw exception();
        }
    }


    /**
     * 获取待签名数据
     */
    string signatureHashAll(Transaction *transaction) {
        TransactionDto transactionDto = Model2DtoTool::transaction2TransactionDto(transaction);
        return TransactionDtoTool::signatureHashAll(transactionDto);
    }

    /**
     * 交易签名
     */
    string signature(string privateKey, Transaction *transaction) {
        TransactionDto transactionDto = Model2DtoTool::transaction2TransactionDto(transaction);
        return TransactionDtoTool::signature(privateKey,transactionDto);
    }

    /**
     * 计算交易哈希
     */
    string calculateTransactionHash(Transaction *transaction){
        TransactionDto transactionDto = Model2DtoTool::transaction2TransactionDto(transaction);
        return TransactionDtoTool::calculateTransactionHash(transactionDto);
    }






    /**
     * 交易中的金额是否符合系统的约束
     */
    bool checkTransactionValue(Transaction *transaction) {
        vector<TransactionInput> inputs = transaction->inputs;
        if(!inputs.empty()){
            //校验交易输入的金额
            for(TransactionInput input:inputs){
                if(!checkValue(input.unspentTransactionOutput.value)){
                    LogUtil::debug("交易金额不合法");
                    return false;
                }
            }
        }

        vector<TransactionOutput> outputs = transaction->outputs;
        if(!outputs.empty()){
            //校验交易输出的金额
            for(TransactionOutput output:outputs){
                if(!checkValue(output.value)){
                    LogUtil::debug("交易金额不合法");
                    return false;
                }
            }
        }

        //根据交易类型，做进一步的校验
        if(transaction->transactionType == TransactionTypeEnum::GENESIS_TRANSACTION){
            //没有需要校验的，跳过。
        } else if(transaction->transactionType == TransactionTypeEnum::STANDARD_TRANSACTION){
            //交易输入必须要大于等于交易输出
            uint64_t inputsValue = getInputValue(transaction);
            uint64_t outputsValue = getOutputValue(transaction);
            if(inputsValue < outputsValue) {
                LogUtil::debug("交易校验失败：交易的输入必须大于等于交易的输出。不合法的交易。");
                return false;
            }
            return true;
        } else {
            throw exception();
        }
        return true;
    }

    /**
     * 校验交易中的地址是否是P2PKH地址
     */
    bool checkPayToPublicKeyHashAddress(Transaction *transaction) {
        vector<TransactionOutput> outputs = transaction->outputs;
        if(!outputs.empty()){
            for(TransactionOutput output:outputs){
                if(!AccountUtil::isPayToPublicKeyHashAddress(output.address)){
                    LogUtil::debug("交易地址不合法");
                    return false;
                }
            }
        }
        return true;
    }

    /**
     * 交易中是否存在重复的[未花费交易输出]
     */
    bool isExistDuplicateUtxo(Transaction *transaction) {
        vector<string> utxoIds;
        vector<TransactionInput> inputs = transaction->inputs;
        if(!inputs.empty()){
            for(TransactionInput transactionInput : inputs) {
                TransactionOutput unspentTransactionOutput = transactionInput.unspentTransactionOutput;
                string utxoId = getTransactionOutputId(&unspentTransactionOutput);
                utxoIds.push_back(utxoId);
            }
        }
        return StringsUtil::hasDuplicateElement(utxoIds);
    }
    /**
     * 区块新产生的地址是否存在重复
     */
    bool isExistDuplicateNewAddress(Transaction *transaction) {
        vector<string> newAddresss;
        vector<TransactionOutput> outputs = transaction->outputs;
        if(!outputs.empty()){
            for (TransactionOutput output:outputs){
                string address = output.address;
                newAddresss.push_back(address);
            }
        }
        return StringsUtil::hasDuplicateElement(newAddresss);
    }

    uint64_t getTransactionInputCount(Transaction *transaction) {
        vector<TransactionInput> inputs = transaction->inputs;
        uint64_t transactionInputCount = inputs.empty()?0:inputs.size();
        return transactionInputCount;
    }

    uint64_t getTransactionOutputCount(Transaction *transaction) {
        vector<TransactionOutput> outputs = transaction->outputs;
        uint64_t transactionOutputCount = outputs.empty()?0:outputs.size();
        return transactionOutputCount;
    }

    uint64_t calculateTransactionFee(Transaction *transaction) {
        if(transaction->transactionType == TransactionTypeEnum::GENESIS_TRANSACTION){
            //创世交易没有交易手续费
            return 0;
        }else if(transaction->transactionType == TransactionTypeEnum::STANDARD_TRANSACTION){
            uint64_t inputsValue = getInputValue(transaction);
            uint64_t outputsValue = getOutputValue(transaction);
            return inputsValue - outputsValue;
        }else {
            throw exception();
        }
    }

    /**
     * 按照费率(每字符的手续费)从大到小排序交易
     */
    void sortByTransactionFeeRateDescend(vector<Transaction> *transactions) {
        if(transactions == nullptr){
            return;
        }
        //TODO
        /*transactions.sort((transaction1, transaction2) -> {
            uint64_t transaction1FeeRate = TransactionTool.getTransactionFeeRate(transaction1);
            uint64_t transaction2FeeRate = TransactionTool.getTransactionFeeRate(transaction2);
            uint64_t diffFeeRate = transaction1FeeRate - transaction2FeeRate;
            if(diffFeeRate>0){
                return -1;
            }else if(diffFeeRate==0){
                return 0;
            }else {
                return 1;
            }
        });*/
    }

    /**
     * 校验交易中的脚本是否是P2PKH脚本
     */
    bool checkPayToPublicKeyHashScript(Transaction *transaction) {
        vector<TransactionInput> inputs = transaction->inputs;
        if(!inputs.empty()){
            for(TransactionInput input:inputs){
                if(!ScriptTool::isPayToPublicKeyHashInputScript(&input.inputScript)){
                    return false;
                }
            }
        }
        vector<TransactionOutput> outputs = transaction->outputs;
        if(!outputs.empty()){
            for (TransactionOutput output:outputs) {
                if(!ScriptTool::isPayToPublicKeyHashOutputScript(&output.outputScript)){
                    return false;
                }
            }
        }
        return true;
    }

    string getTransactionOutputId(TransactionOutput *transactionOutput) {
        return BlockchainDatabaseKeyTool::buildTransactionOutputId(transactionOutput->transactionHash,transactionOutput->transactionOutputIndex);
    }

    /**
     * 验证签名
     */
    bool verifySignature(Transaction transaction, string publicKey, vector<unsigned char> *bytesSignature) {
        TransactionDto transactionDto = Model2DtoTool::transaction2TransactionDto(&transaction);
        return TransactionDtoTool::verifySignature(transactionDto,publicKey,*bytesSignature);
    }

    /**
     * 校验交易金额是否是一个合法的交易金额：这里用于限制交易金额的最大值、最小值、小数保留位等
     */
    bool checkValue(uint64_t transactionAmount) {
        //交易金额不能小于等于0
        if(transactionAmount <= 0){
            return false;
        }
        //最大值是2^64
        //小数保留位是0位
        return true;
    }
}