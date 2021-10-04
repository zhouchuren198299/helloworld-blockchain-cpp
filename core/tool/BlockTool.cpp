//
// Created by 40906 on 2021/9/18.
//

#include "BlockTool.h"
#include "BlockDtoTool.h"
#include "../../util/StringsUtil.h"
#include "../../util/StringUtil.h"
#include "../../util/TimeUtil.h"
#include "../../util/NullUtil.h"

#include "../../setting/GenesisBlockSetting.h"
#include "Model2DtoTool.h"
#include "TransactionTool.h"

using namespace dto;

namespace BlockTool {

    /**
     * 计算区块的Hash值
     */
    string calculateBlockHash(Block *block) {
        BlockDto blockDto = Model2DtoTool::block2BlockDto(block);
        return BlockDtoTool::calculateBlockHash(blockDto);
    }

    /**
     * 计算区块的默克尔树根值
     */
    string calculateBlockMerkleTreeRoot(Block *block) {
        BlockDto blockDto = Model2DtoTool::block2BlockDto(block);
        return BlockDtoTool::calculateBlockMerkleTreeRoot(blockDto);
    }

    /**
     * 区块新产生的哈希是否存在重复
     */
    bool isExistDuplicateNewHash(Block *block) {
        vector<string> newHashs;
        string blockHash = block->hash;
        newHashs.push_back(blockHash);
        vector<Transaction> transactions = block->transactions;
        if(!transactions.empty()){
            for(Transaction transaction : transactions){
                string transactionHash = transaction.transactionHash;
                newHashs.push_back(transactionHash);
            }
        }
        return StringsUtil::hasDuplicateElement(newHashs);
    }
    /**
     * 区块新产生的地址是否存在重复
     */
    bool isExistDuplicateNewAddress(Block *block) {
        vector<string> newAddresss;
        vector<Transaction> transactions = block->transactions;
        if(!transactions.empty()){
            for(Transaction transaction : transactions){
                vector<TransactionOutput> outputs = transaction.outputs;
                if(!outputs.empty()){
                    for (TransactionOutput output:outputs){
                        string address = output.address;
                        newAddresss.push_back(address);
                    }
                }
            }
        }
        return StringsUtil::hasDuplicateElement(newAddresss);
    }
    /**
     * 区块中是否存在重复的[未花费交易输出]
     */
    bool isExistDuplicateUtxo(Block *block) {
        vector<string> utxoIds;
        vector<Transaction> transactions = block->transactions;
        if(!transactions.empty()) {
            for(Transaction transaction : transactions){
                vector<TransactionInput> inputs = transaction.inputs;
                if(!inputs.empty()){
                    for(TransactionInput transactionInput : inputs) {
                        TransactionOutput unspentTransactionOutput = transactionInput.unspentTransactionOutput;
                        string utxoId = TransactionTool::getTransactionOutputId(&unspentTransactionOutput);
                        utxoIds.push_back(utxoId);
                    }
                }
            }
        }
        return StringsUtil::hasDuplicateElement(utxoIds);
    }

    /**
     * 校验区块的前区块哈希
     */
    bool checkPreviousBlockHash(Block *previousBlock, Block *currentBlock) {
        if(NullUtil::isNullBlock(*previousBlock)){
            return StringUtil::equals(GenesisBlockSetting::HASH, currentBlock->previousHash);
        } else {
            return StringUtil::equals(previousBlock->hash, currentBlock->previousHash);
        }
    }

    /**
     * 校验区块高度的连贯性
     */
    bool checkBlockHeight(Block *previousBlock, Block *currentBlock) {
        if(NullUtil::isNullBlock(*previousBlock)){
            return (GenesisBlockSetting::HEIGHT +1) == currentBlock->height;
        } else {
            return (previousBlock->height+1) == currentBlock->height;
        }
    }

    /**
     * 校验区块的时间
     * 区块时间戳一定要比当前时间戳小。挖矿是个技术活，默认矿工有能力将自己机器的时间调整正确，所以矿工不应该穿越到未来挖矿。
     * 区块时间戳一定要比前一个区块的时间戳大。
     */
    bool checkBlockTimestamp(Block *previousBlock, Block *currentBlock) {
        if(currentBlock->timestamp > TimeUtil::millisecondTimestamp()){
            return false;
        }
        if(NullUtil::isNullBlock(*previousBlock)){
            return true;
        } else {
            return currentBlock->timestamp > previousBlock->timestamp;
        }
    }

    /**
     * 获取区块中交易的数量
     */
    uint64_t getTransactionCount(Block *block) {
        vector<Transaction> transactions = block->transactions;
        return transactions.empty()?0:transactions.size();
    }

    /**
     * 简单的校验两个区块是否相等
     * 注意：这里没有严格校验,例如没有校验区块中的交易是否完全一样
     * ，所以即使这里认为两个区块相等，实际上这两个区块还是有可能不相等的。
     */
    bool isBlockEquals(Block *block1, Block *block2) {
        return StringUtil::equals(block1->hash, block2->hash);
    }

    /**
     * 获取写入的激励金额
     */
    uint64_t getWritedIncentiveValue(Block *block) {
        return ((block->transactions)[0].outputs)[0].value;
    }

    /**
     * 格式化难度
     * 前置填零，返回[长度为64位][十六进制字符串形式的]难度
     */
    string formatDifficulty(string difficulty) {
        //难度长度是256bit，64位十六进制的字符串数，如果传入的难度长度不够，这里进行前置补充零操作。
        return StringUtil::prefixPadding(difficulty,64,"0");
    }

    /**
     * 获取交易输出数量
     */
    uint64_t getTransactionOutputCount(Block *block) {
        uint64_t transactionOutputCount = 0;
        vector<Transaction> transactions = block->transactions;
        if(!transactions.empty()){
            for(Transaction transaction:transactions){
                transactionOutputCount += TransactionTool::getTransactionOutputCount(&transaction);
            }
        }
        return transactionOutputCount;
    }

    /**
     * 区块总交易手续费
     */
    uint64_t getBlockFee(Block *block) {
        uint64_t blockFee = 0;
        vector<Transaction> transactions = block->transactions;
        if(!transactions.empty()){
            for(Transaction transaction:transactions){
                if(transaction.transactionType == TransactionTypeEnum::GENESIS_TRANSACTION){
                    continue;
                }else if(transaction.transactionType == TransactionTypeEnum::STANDARD_TRANSACTION){
                    uint64_t fee = TransactionTool::getTransactionFee(&transaction);
                    blockFee += fee;
                }else{
                    throw new exception();
                }
            }
        }
        return blockFee;
    }
    /**
     * 获取下一个区块的高度
     */
    uint64_t getNextBlockHeight(Block *currentBlock) {
        uint64_t nextBlockHeight = NullUtil::isNullBlock(*currentBlock) ? GenesisBlockSetting::HEIGHT + 1 : currentBlock->height + 1;
        return nextBlockHeight;
    }
    
}