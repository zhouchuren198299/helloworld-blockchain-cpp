//
// Created by 40906 on 2021/9/20.
//

#include "BlockchainDatabase.h"
#include "tool/StructureTool.h"
#include "tool/SizeTool.h"
#include "tool/BlockchainDatabaseKeyTool.h"
#include "tool/BlockTool.h"
#include "tool/TransactionTool.h"
#include "tool/ScriptTool.h"
#include "tool/ScriptDtoTool.h"
#include "tool/TransactionDtoTool.h"
#include "../setting/GenesisBlockSetting.h"
#include "../crypto/AccountUtil.h"
#include "../util/ByteUtil.h"
#include "../util/LogUtil.h"
#include "../util/FileUtil.h"
#include "../util/EncodeDecodeTool.h"
#include "tool/NullTool.h"

using namespace model;
using namespace BlockchainActionEnum;
using namespace TransactionTypeEnum;
namespace core{






    const string BLOCKCHAIN_DATABASE_NAME = "BlockchainDatabase";



//endregion



//region 区块增加与删除

    bool BlockchainDatabase::addBlockDto(BlockDto *blockDto) {
        mutex.lock();
        Block block = blockDto2Block(blockDto);
        bool checkBlock0 = checkBlock(&block);
        if(!checkBlock0){
            return false;
        }
        KvWriteBatch kvWriteBatch = createBlockWriteBatch(&block, ADD_BLOCK);
        KvDbUtil::write(getBlockchainDatabasePath(), kvWriteBatch);
        mutex.unlock();
        return true;
    }

    void BlockchainDatabase::deleteTailBlock() {
        mutex.lock();
        Block tailBlock = queryTailBlock();
        if(NullTool::isNullBlock(tailBlock)){
            return;
        }
        KvWriteBatch kvWriteBatch = createBlockWriteBatch(&tailBlock, DELETE_BLOCK);
        KvDbUtil::write(getBlockchainDatabasePath(),kvWriteBatch);
        mutex.unlock();
    }

    void BlockchainDatabase::deleteBlocks(uint64_t blockHeight) {
        mutex.lock();
        while (true){
            Block tailBlock = queryTailBlock();
            if(NullTool::isNullBlock(tailBlock)){
                return;
            }
            if(tailBlock.height < blockHeight){
                return;
            }
            KvWriteBatch kvWriteBatch = createBlockWriteBatch(&tailBlock, DELETE_BLOCK);
            KvDbUtil::write(getBlockchainDatabasePath(),kvWriteBatch);
        }
        mutex.unlock();
    }
//endregion



//region 校验区块、交易

    bool BlockchainDatabase::checkBlock(Block *block) {

        //校验区块的结构
        if(!StructureTool::checkBlockStructure(block)){
            LogUtil::debug("区块数据异常，请校验区块的结构。");
            return false;
        }
        //校验区块的大小
        if(!SizeTool::checkBlockSize(block)){
            LogUtil::debug("区块数据异常，请校验区块的大小。");
            return false;
        }


        //校验业务
        Block previousBlock = queryTailBlock();
        //校验区块高度的连贯性
        if(!BlockTool::checkBlockHeight(&previousBlock,block)){
            LogUtil::debug("区块写入的区块高度出错。");
            return false;
        }
        //校验区块的前区块哈希
        if(!BlockTool::checkPreviousBlockHash(&previousBlock,block)){
            LogUtil::debug("区块写入的前区块哈希出错。");
            return false;
        }
        //校验区块时间
        if(!BlockTool::checkBlockTimestamp(&previousBlock,block)){
            LogUtil::debug("区块生成的时间太滞后。");
            return false;
        }
        //校验新产生的哈希
        if(!checkBlockNewHash(block)){
            LogUtil::debug("区块数据异常，区块中新产生的哈希异常。");
            return false;
        }
        //校验新产生的地址
        if(!checkBlockNewAddress(block)){
            LogUtil::debug("区块数据异常，区块中新产生的哈希异常。");
            return false;
        }
        //校验双花
        if(!checkBlockDoubleSpend(block)){
            LogUtil::debug("区块数据异常，检测到双花攻击。");
            return false;
        }
        //校验共识
        if(!consensus->checkConsensus(this,block)){
            LogUtil::debug("区块数据异常，未满足共识规则。");
            return false;
        }
        //校验激励
        if(!incentive->checkIncentive(this,block)){
            LogUtil::debug("区块数据异常，激励校验失败。");
            return false;
        }

        //从交易角度校验每一笔交易
        for(Transaction transaction : block->transactions){
            bool transactionCanAddToNextBlock = checkTransaction(&transaction);
            if(!transactionCanAddToNextBlock){
                LogUtil::debug("区块数据异常，交易异常。");
                return false;
            }
        }
        return true;
    }


    bool BlockchainDatabase::checkTransaction(Transaction *transaction) {
        //校验交易的结构
        if(!StructureTool::checkTransactionStructure(transaction)){
            LogUtil::debug("交易数据异常，请校验交易的结构。");
            return false;
        }
        //校验交易的大小
        if(!SizeTool::checkTransactionSize(transaction)){
            LogUtil::debug("交易数据异常，请校验交易的大小。");
            return false;
        }


        //校验交易中的地址是否是P2PKH地址
        if(!TransactionTool::checkPayToPublicKeyHashAddress(transaction)){
            return false;
        }
        //校验交易中的脚本是否是P2PKH脚本
        if(!TransactionTool::checkPayToPublicKeyHashScript(transaction)){
            return false;
        }

        //业务校验
        //校验新产生的哈希
        if(!checkTransactionNewHash(transaction)){
            LogUtil::debug("区块数据异常，区块中新产生的哈希异常。");
            return false;
        }
        //校验新产生的地址
        if(!checkTransactionNewAddress(transaction)){
            LogUtil::debug("区块数据异常，区块中新产生的哈希异常。");
            return false;
        }
        //校验交易金额
        if(!TransactionTool::checkTransactionValue(transaction)){
            LogUtil::debug("交易金额不合法");
            return false;
        }
        //校验双花
        if(!checkTransactionDoubleSpend(transaction)){
            LogUtil::debug("交易数据异常，检测到双花攻击。");
            return false;
        }
        //校验脚本
        if(!checkTransactionScript(transaction)) {
            LogUtil::debug("交易校验失败：交易[输入脚本]解锁交易[输出脚本]异常。");
            return false;
        }
        return true;
    }
//endregion



//region 普通查询

    uint64_t BlockchainDatabase::queryBlockchainHeight() {
        vector<unsigned char> bytesBlockchainHeight = KvDbUtil::get(getBlockchainDatabasePath(), BlockchainDatabaseKeyTool::buildBlockchainHeightKey());
        if(bytesBlockchainHeight.empty()){
            return GenesisBlockSetting::HEIGHT;
        }
        return ByteUtil::bytesToUint64(bytesBlockchainHeight);
    }


    uint64_t BlockchainDatabase::queryBlockchainTransactionHeight() {
        vector<unsigned char> byteTotalTransactionCount = KvDbUtil::get(getBlockchainDatabasePath(), BlockchainDatabaseKeyTool::buildBlockchainTransactionHeightKey());
        if(byteTotalTransactionCount.empty()){
            return 0;
        }
        return ByteUtil::bytesToUint64(byteTotalTransactionCount);
    }

    uint64_t BlockchainDatabase::queryBlockchainTransactionOutputHeight() {
        vector<unsigned char> byteTotalTransactionCount = KvDbUtil::get(getBlockchainDatabasePath(), BlockchainDatabaseKeyTool::buildBlockchainTransactionOutputHeightKey());
        if(byteTotalTransactionCount.empty()){
            return 0;
        }
        return ByteUtil::bytesToUint64(byteTotalTransactionCount);
    }
//endregion



//region 区块查询

    Block BlockchainDatabase::queryTailBlock() {
        uint64_t blockchainHeight = queryBlockchainHeight();
        if(blockchainHeight <= GenesisBlockSetting::HEIGHT){
            return NullTool::newNullBlock();
        }
        return queryBlockByBlockHeight(blockchainHeight);
    }

    Block BlockchainDatabase::queryBlockByBlockHeight(uint64_t blockHeight) {
        vector<unsigned char> bytesBlock = KvDbUtil::get(getBlockchainDatabasePath(), BlockchainDatabaseKeyTool::buildBlockHeightToBlockKey(blockHeight));
        if(bytesBlock.empty()){
            return NullTool::newNullBlock();
        }
        return EncodeDecodeTool::decode(bytesBlock,Block{});
    }

    Block BlockchainDatabase::queryBlockByBlockHash(string blockHash) {
        vector<unsigned char> bytesBlockHeight = KvDbUtil::get(getBlockchainDatabasePath(), BlockchainDatabaseKeyTool::buildBlockHashToBlockHeightKey(blockHash));
        if(bytesBlockHeight.empty()){
            return NullTool::newNullBlock();
        }
        return queryBlockByBlockHeight(ByteUtil::bytesToUint64(bytesBlockHeight));
    }
//endregion



//region 交易查询

    Transaction BlockchainDatabase::queryTransactionByTransactionHash(string transactionHash) {
        vector<unsigned char> transactionHeight = KvDbUtil::get(getBlockchainDatabasePath(), BlockchainDatabaseKeyTool::buildTransactionHashToTransactionHeightKey(transactionHash));
        if(transactionHeight.empty()){
            return NullTool::newNullTransaction();
        }
        return queryTransactionByTransactionHeight(ByteUtil::bytesToUint64(transactionHeight));
    }


    Transaction BlockchainDatabase::querySourceTransactionByTransactionOutputId(string transactionHash,uint64_t transactionOutputIndex) {
        vector<unsigned char> sourceTransactionHeight = KvDbUtil::get(getBlockchainDatabasePath(), BlockchainDatabaseKeyTool::buildTransactionOutputIdToSourceTransactionHeightKey(transactionHash,transactionOutputIndex));
        if(sourceTransactionHeight.empty()){
            return NullTool::newNullTransaction();
        }
        return queryTransactionByTransactionHeight(ByteUtil::bytesToUint64(sourceTransactionHeight));
    }


    Transaction BlockchainDatabase::queryDestinationTransactionByTransactionOutputId(string transactionHash,uint64_t transactionOutputIndex) {
        vector<unsigned char> destinationTransactionHeight = KvDbUtil::get(getBlockchainDatabasePath(), BlockchainDatabaseKeyTool::buildTransactionOutputIdToDestinationTransactionHeightKey(transactionHash,transactionOutputIndex));
        if(destinationTransactionHeight.empty()){
            return NullTool::newNullTransaction();
        }
        return queryTransactionByTransactionHeight(ByteUtil::bytesToUint64(destinationTransactionHeight));
    }


    TransactionOutput BlockchainDatabase::queryTransactionOutputByTransactionOutputHeight(uint64_t transactionOutputHeight) {
        vector<unsigned char> bytesTransactionOutput = KvDbUtil::get(getBlockchainDatabasePath(), BlockchainDatabaseKeyTool::buildTransactionOutputHeightToTransactionOutputKey(transactionOutputHeight));
        if(bytesTransactionOutput.empty()){
            return NullTool::newNullTransactionOutput();
        }
        return EncodeDecodeTool::decode(bytesTransactionOutput,TransactionOutput{});
    }


    Transaction BlockchainDatabase::queryTransactionByTransactionHeight(uint64_t transactionHeight) {
        vector<unsigned char> byteTransaction = KvDbUtil::get(getBlockchainDatabasePath(), BlockchainDatabaseKeyTool::buildTransactionHeightToTransactionKey(transactionHeight));
        if(byteTransaction.empty()){
            return NullTool::newNullTransaction();
        }
        return EncodeDecodeTool::decode(byteTransaction,Transaction{});
    }
//endregion



//region 交易输出查询

    TransactionOutput BlockchainDatabase::queryTransactionOutputByTransactionOutputId(string transactionHash,uint64_t transactionOutputIndex) {
        vector<unsigned char> bytesTransactionOutputHeight = KvDbUtil::get(getBlockchainDatabasePath(), BlockchainDatabaseKeyTool::buildTransactionOutputIdToTransactionOutputHeightKey(transactionHash,transactionOutputIndex));
        if(bytesTransactionOutputHeight.empty()){
            return NullTool::newNullTransactionOutput();
        }
        return queryTransactionOutputByTransactionOutputHeight(ByteUtil::bytesToUint64(bytesTransactionOutputHeight));
    }


    TransactionOutput BlockchainDatabase::queryUnspentTransactionOutputByTransactionOutputId(string transactionHash,uint64_t transactionOutputIndex) {
        vector<unsigned char> bytesTransactionOutputHeight = KvDbUtil::get(getBlockchainDatabasePath(), BlockchainDatabaseKeyTool::buildTransactionOutputIdToUnspentTransactionOutputHeightKey(transactionHash,transactionOutputIndex));
        if(bytesTransactionOutputHeight.empty()){
            return NullTool::newNullTransactionOutput();
        }
        return queryTransactionOutputByTransactionOutputHeight(ByteUtil::bytesToUint64(bytesTransactionOutputHeight));
    }


    TransactionOutput BlockchainDatabase::querySpentTransactionOutputByTransactionOutputId(string transactionHash,uint64_t transactionOutputIndex) {
        vector<unsigned char> bytesTransactionOutputHeight = KvDbUtil::get(getBlockchainDatabasePath(), BlockchainDatabaseKeyTool::buildTransactionOutputIdToSpentTransactionOutputHeightKey(transactionHash,transactionOutputIndex));
        if(bytesTransactionOutputHeight.empty()){
            return NullTool::newNullTransactionOutput();
        }
        return queryTransactionOutputByTransactionOutputHeight(ByteUtil::bytesToUint64(bytesTransactionOutputHeight));
    }


    TransactionOutput BlockchainDatabase::queryTransactionOutputByAddress(string address) {
        vector<unsigned char> bytesTransactionOutputHeight = KvDbUtil::get(getBlockchainDatabasePath(), BlockchainDatabaseKeyTool::buildAddressToTransactionOutputHeightKey(address));
        if(bytesTransactionOutputHeight.empty()){
            return NullTool::newNullTransactionOutput();
        }
        return queryTransactionOutputByTransactionOutputHeight(ByteUtil::bytesToUint64(bytesTransactionOutputHeight));
    }


    TransactionOutput BlockchainDatabase::queryUnspentTransactionOutputByAddress(string address) {
        vector<unsigned char> bytesTransactionOutputHeight = KvDbUtil::get(getBlockchainDatabasePath(), BlockchainDatabaseKeyTool::buildAddressToUnspentTransactionOutputHeightKey(address));
        if(bytesTransactionOutputHeight.empty()){
            return NullTool::newNullTransactionOutput();
        }
        return queryTransactionOutputByTransactionOutputHeight(ByteUtil::bytesToUint64(bytesTransactionOutputHeight));
    }


    TransactionOutput BlockchainDatabase::querySpentTransactionOutputByAddress(string address) {
        vector<unsigned char> bytesTransactionOutputHeight = KvDbUtil::get(getBlockchainDatabasePath(), BlockchainDatabaseKeyTool::buildAddressToSpentTransactionOutputHeightKey(address));
        if(bytesTransactionOutputHeight.empty()){
            return NullTool::newNullTransactionOutput();
        }
        return queryTransactionOutputByTransactionOutputHeight(ByteUtil::bytesToUint64(bytesTransactionOutputHeight));
    }


    //endregion

    Incentive* BlockchainDatabase::getIncentive() {
        return incentive;
    }

    Consensus* BlockchainDatabase::getConsensus() {
        return consensus;
    }












































//region 拼装WriteBatch
/**
 * 根据区块信息组装WriteBatch对象
 */
    KvWriteBatch BlockchainDatabase::createBlockWriteBatch(Block *block, BlockchainAction blockchainAction) {
        KvWriteBatch kvWriteBatch;

        storeHash(&kvWriteBatch,block,blockchainAction);
        storeAddress(&kvWriteBatch,block,blockchainAction);

        storeBlockchainHeight(&kvWriteBatch,block,blockchainAction);
        storeBlockchainTransactionHeight(&kvWriteBatch,block,blockchainAction);
        storeBlockchainTransactionOutputHeight(&kvWriteBatch,block,blockchainAction);

        storeBlockHeightToBlock(&kvWriteBatch,block,blockchainAction);
        storeBlockHashToBlockHeight(&kvWriteBatch,block,blockchainAction);

        storeTransactionHeightToTransaction(&kvWriteBatch,block,blockchainAction);
        storeTransactionHashToTransactionHeight(&kvWriteBatch,block,blockchainAction);

        storeTransactionOutputHeightToTransactionOutput(&kvWriteBatch,block,blockchainAction);
        storeTransactionOutputIdToTransactionOutputHeight(&kvWriteBatch,block,blockchainAction);
        storeTransactionOutputIdToUnspentTransactionOutputHeight(&kvWriteBatch,block,blockchainAction);
        storeTransactionOutputIdToSpentTransactionOutputHeight(&kvWriteBatch,block,blockchainAction);
        storeTransactionOutputIdToSourceTransactionHeight(&kvWriteBatch,block,blockchainAction);
        storeTransactionOutputIdToDestinationTransactionHeight(&kvWriteBatch,block,blockchainAction);

        //TODO 将地址改为交易输出脚本
        storeAddressToTransactionOutputHeight(&kvWriteBatch,block,blockchainAction);
        storeAddressToUnspentTransactionOutputHeight(&kvWriteBatch,block,blockchainAction);
        storeAddressToSpentTransactionOutputHeight(&kvWriteBatch,block,blockchainAction);
        return kvWriteBatch;
    };


    /**
     * [交易输出ID]到[来源交易高度]的映射
     */
    void BlockchainDatabase::storeTransactionOutputIdToSourceTransactionHeight(KvWriteBatch *kvWriteBatch, Block *block, BlockchainAction blockchainAction) {
        vector<Transaction> transactions = block->transactions;
        if(!transactions.empty()){
            for(Transaction transaction:transactions){
                vector<TransactionOutput> outputs = transaction.outputs;
                if(!outputs.empty()){
                    for(TransactionOutput transactionOutput:outputs){
                        vector<unsigned char> transactionOutputIdToToSourceTransactionHeightKey = BlockchainDatabaseKeyTool::buildTransactionOutputIdToSourceTransactionHeightKey(transactionOutput.transactionHash,transactionOutput.transactionOutputIndex);
                        if(ADD_BLOCK == blockchainAction){
                            kvWriteBatch->put(transactionOutputIdToToSourceTransactionHeightKey, ByteUtil::uint64ToBytes(transaction.transactionHeight));
                        } else {
                            kvWriteBatch->delete0(transactionOutputIdToToSourceTransactionHeightKey);
                        }
                    }
                }
            }
        }
    }
/**
 * [已花费交易输出ID]到[去向交易高度]的映射
 */
    void BlockchainDatabase::storeTransactionOutputIdToDestinationTransactionHeight(KvWriteBatch *kvWriteBatch, Block *block, BlockchainAction blockchainAction) {
        vector<Transaction> transactions = block->transactions;
        if(!transactions.empty()){
            for(Transaction transaction:transactions){
                vector<TransactionInput> inputs = transaction.inputs;
                if(!inputs.empty()){
                    for(TransactionInput transactionInput:inputs){
                        TransactionOutput unspentTransactionOutput = transactionInput.unspentTransactionOutput;
                        vector<unsigned char> transactionOutputIdToToDestinationTransactionHeightKey = BlockchainDatabaseKeyTool::buildTransactionOutputIdToDestinationTransactionHeightKey(unspentTransactionOutput.transactionHash,unspentTransactionOutput.transactionOutputIndex);
                        if(ADD_BLOCK == blockchainAction){
                            kvWriteBatch->put(transactionOutputIdToToDestinationTransactionHeightKey, ByteUtil::uint64ToBytes(transaction.transactionHeight));
                        } else {
                            kvWriteBatch->delete0(transactionOutputIdToToDestinationTransactionHeightKey);
                        }
                    }
                }
            }
        }
    }
/**
 * [交易输出ID]到[交易输出]的映射
 */
    void BlockchainDatabase::storeTransactionOutputIdToTransactionOutputHeight(KvWriteBatch *kvWriteBatch, Block *block, BlockchainAction blockchainAction) {
        vector<Transaction> transactions = block->transactions;
        if(!transactions.empty()){
            for(Transaction transaction:transactions){
                vector<TransactionOutput> outputs = transaction.outputs;
                if(!outputs.empty()){
                    for(TransactionOutput output:outputs){
                        vector<unsigned char> transactionOutputIdToTransactionOutputHeightKey = BlockchainDatabaseKeyTool::buildTransactionOutputIdToTransactionOutputHeightKey(output.transactionHash,output.transactionOutputIndex);
                        if(ADD_BLOCK == blockchainAction){
                            kvWriteBatch->put(transactionOutputIdToTransactionOutputHeightKey, ByteUtil::uint64ToBytes(output.transactionOutputHeight));
                        } else {
                            kvWriteBatch->delete0(transactionOutputIdToTransactionOutputHeightKey);
                        }
                    }
                }
            }
        }
    }
/**
 * [交易输出高度]到[交易输出]的映射
 */
    void BlockchainDatabase::storeTransactionOutputHeightToTransactionOutput(KvWriteBatch *kvWriteBatch, Block *block, BlockchainAction blockchainAction) {
        vector<Transaction> transactions = block->transactions;
        if(!transactions.empty()){
            for(Transaction transaction:transactions){
                vector<TransactionOutput> outputs = transaction.outputs;
                if(!outputs.empty()){
                    for(TransactionOutput output:outputs){
                        vector<unsigned char> transactionOutputHeightToTransactionOutputKey = BlockchainDatabaseKeyTool::buildTransactionOutputHeightToTransactionOutputKey(output.transactionOutputHeight);
                        if(ADD_BLOCK == blockchainAction){
                            kvWriteBatch->put(transactionOutputHeightToTransactionOutputKey, EncodeDecodeTool::encode(output));
                        } else {
                            kvWriteBatch->delete0(transactionOutputHeightToTransactionOutputKey);
                        }
                    }
                }
            }
        }
    }
/**
 * 存储未花费交易输出ID到未花费交易输出的映射
 */
    void BlockchainDatabase::storeTransactionOutputIdToUnspentTransactionOutputHeight(KvWriteBatch *kvWriteBatch, Block *block, BlockchainAction blockchainAction) {
        vector<Transaction> transactions = block->transactions;
        if(!transactions.empty()){
            for(Transaction transaction:transactions){
                vector<TransactionInput> inputs = transaction.inputs;
                if(!inputs.empty()){
                    for(TransactionInput transactionInput:inputs){
                        TransactionOutput unspentTransactionOutput = transactionInput.unspentTransactionOutput;
                        vector<unsigned char> transactionOutputIdToUnspentTransactionOutputHeightKey = BlockchainDatabaseKeyTool::buildTransactionOutputIdToUnspentTransactionOutputHeightKey(unspentTransactionOutput.transactionHash,unspentTransactionOutput.transactionOutputIndex);
                        if(ADD_BLOCK == blockchainAction){
                            kvWriteBatch->delete0(transactionOutputIdToUnspentTransactionOutputHeightKey);
                        } else {
                            kvWriteBatch->put(transactionOutputIdToUnspentTransactionOutputHeightKey, ByteUtil::uint64ToBytes(unspentTransactionOutput.transactionOutputHeight));
                        }
                    }
                }
                vector<TransactionOutput> outputs = transaction.outputs;
                if(!outputs.empty()){
                    for(TransactionOutput output:outputs){
                        vector<unsigned char> transactionOutputIdToUnspentTransactionOutputHeightKey = BlockchainDatabaseKeyTool::buildTransactionOutputIdToUnspentTransactionOutputHeightKey(output.transactionHash,output.transactionOutputIndex);
                        if(ADD_BLOCK == blockchainAction){
                            kvWriteBatch->put(transactionOutputIdToUnspentTransactionOutputHeightKey, ByteUtil::uint64ToBytes(output.transactionOutputHeight));
                        } else {
                            kvWriteBatch->delete0(transactionOutputIdToUnspentTransactionOutputHeightKey);
                        }
                    }
                }
            }
        }
    }
/**
 * 存储已花费交易输出ID到已花费交易输出的映射
 */
    void BlockchainDatabase::storeTransactionOutputIdToSpentTransactionOutputHeight(KvWriteBatch *kvWriteBatch, Block *block, BlockchainAction blockchainAction) {
        vector<Transaction> transactions = block->transactions;
        if(!transactions.empty()){
            for(Transaction transaction:transactions){
                vector<TransactionInput> inputs = transaction.inputs;
                if(!inputs.empty()){
                    for(TransactionInput transactionInput:inputs){
                        TransactionOutput unspentTransactionOutput = transactionInput.unspentTransactionOutput;
                        vector<unsigned char> transactionOutputIdToSpentTransactionOutputHeightKey = BlockchainDatabaseKeyTool::buildTransactionOutputIdToSpentTransactionOutputHeightKey(unspentTransactionOutput.transactionHash,unspentTransactionOutput.transactionOutputIndex);
                        if(ADD_BLOCK == blockchainAction){
                            kvWriteBatch->put(transactionOutputIdToSpentTransactionOutputHeightKey, ByteUtil::uint64ToBytes(unspentTransactionOutput.transactionOutputHeight));
                        } else {
                            kvWriteBatch->delete0(transactionOutputIdToSpentTransactionOutputHeightKey);
                        }
                    }
                }
                vector<TransactionOutput> outputs = transaction.outputs;
                if(!outputs.empty()){
                    for(TransactionOutput output:outputs){
                        vector<unsigned char> transactionOutputIdToSpentTransactionOutputHeightKey = BlockchainDatabaseKeyTool::buildTransactionOutputIdToSpentTransactionOutputHeightKey(output.transactionHash,output.transactionOutputIndex);
                        if(ADD_BLOCK == blockchainAction){
                            kvWriteBatch->delete0(transactionOutputIdToSpentTransactionOutputHeightKey);
                        } else {
                            kvWriteBatch->put(transactionOutputIdToSpentTransactionOutputHeightKey, ByteUtil::uint64ToBytes(output.transactionOutputHeight));
                        }
                    }
                }
            }
        }
    }
/**
 * 存储交易高度到交易的映射
 */
    void BlockchainDatabase::storeTransactionHeightToTransaction(KvWriteBatch *kvWriteBatch, Block *block, BlockchainAction blockchainAction) {
        vector<Transaction> transactions = block->transactions;
        if(!transactions.empty()){
            for(Transaction transaction:transactions){
//更新区块链中的交易序列号数据
                vector<unsigned char> transactionHeightToTransactionKey = BlockchainDatabaseKeyTool::buildTransactionHeightToTransactionKey(transaction.transactionHeight);
                if(ADD_BLOCK == blockchainAction){
                    kvWriteBatch->put(transactionHeightToTransactionKey, EncodeDecodeTool::encode(transaction));
                } else {
                    kvWriteBatch->delete0(transactionHeightToTransactionKey);
                }
            }
        }
    }
/**
 * 存储交易哈希到交易高度的映射
 */
    void BlockchainDatabase::storeTransactionHashToTransactionHeight(KvWriteBatch *kvWriteBatch, Block *block, BlockchainAction blockchainAction) {
        vector<Transaction> transactions = block->transactions;
        if(!transactions.empty()){
            for(Transaction transaction:transactions){
                vector<unsigned char> transactionHashToTransactionHeightKey = BlockchainDatabaseKeyTool::buildTransactionHashToTransactionHeightKey(transaction.transactionHash);
                if(ADD_BLOCK == blockchainAction){
                    kvWriteBatch->put(transactionHashToTransactionHeightKey, ByteUtil::uint64ToBytes(transaction.transactionHeight));
                } else {
                    kvWriteBatch->delete0(transactionHashToTransactionHeightKey);
                }
            }
        }
    }
/**
 * 存储区块链的高度
 */
    void BlockchainDatabase::storeBlockchainHeight(KvWriteBatch *kvWriteBatch, Block *block, BlockchainAction blockchainAction) {
        vector<unsigned char> blockchainHeightKey = BlockchainDatabaseKeyTool::buildBlockchainHeightKey();
        if(ADD_BLOCK == blockchainAction){
            kvWriteBatch->put(blockchainHeightKey, ByteUtil::uint64ToBytes(block->height));
        }else{
            kvWriteBatch->put(blockchainHeightKey, ByteUtil::uint64ToBytes(block->height-1));
        }
    }
/**
 * 存储区块哈希到区块高度的映射
 */
    void BlockchainDatabase::storeBlockHashToBlockHeight(KvWriteBatch *kvWriteBatch, Block *block, BlockchainAction blockchainAction) {
        vector<unsigned char> blockHashBlockHeightKey = BlockchainDatabaseKeyTool::buildBlockHashToBlockHeightKey(block->hash);
        if(ADD_BLOCK == blockchainAction){
            kvWriteBatch->put(blockHashBlockHeightKey, ByteUtil::uint64ToBytes(block->height));
        }else{
            kvWriteBatch->delete0(blockHashBlockHeightKey);
        }
    }
/**
 * 存储区块链中总的交易高度
 */
    void BlockchainDatabase::storeBlockchainTransactionHeight(KvWriteBatch *kvWriteBatch, Block *block, BlockchainAction blockchainAction) {
        uint64_t transactionCount = queryBlockchainTransactionHeight();
        vector<unsigned char> bytesBlockchainTransactionCountKey = BlockchainDatabaseKeyTool::buildBlockchainTransactionHeightKey();
        if(ADD_BLOCK == blockchainAction){
            kvWriteBatch->put(bytesBlockchainTransactionCountKey, ByteUtil::uint64ToBytes(transactionCount + BlockTool::getTransactionCount(block)));
        }else{
            kvWriteBatch->put(bytesBlockchainTransactionCountKey, ByteUtil::uint64ToBytes(transactionCount - BlockTool::getTransactionCount(block)));
        }
    }
/**
 * 存储区块链中总的交易数量
 */
    void BlockchainDatabase::storeBlockchainTransactionOutputHeight(KvWriteBatch *kvWriteBatch, Block *block, BlockchainAction blockchainAction) {
        uint64_t transactionOutputCount = queryBlockchainTransactionOutputHeight();
        vector<unsigned char> bytesBlockchainTransactionOutputHeightKey = BlockchainDatabaseKeyTool::buildBlockchainTransactionOutputHeightKey();
        if(ADD_BLOCK == blockchainAction){
            kvWriteBatch->put(bytesBlockchainTransactionOutputHeightKey, ByteUtil::uint64ToBytes(transactionOutputCount + BlockTool::getTransactionOutputCount(block)));
        }else{
            kvWriteBatch->put(bytesBlockchainTransactionOutputHeightKey, ByteUtil::uint64ToBytes(transactionOutputCount - BlockTool::getTransactionOutputCount(block)));
        }
    }
/**
 * 存储区块链高度到区块的映射
 */
    void BlockchainDatabase::storeBlockHeightToBlock(KvWriteBatch *kvWriteBatch, Block *block, BlockchainAction blockchainAction) {
        vector<unsigned char> blockHeightKey = BlockchainDatabaseKeyTool::buildBlockHeightToBlockKey(block->height);
        if(ADD_BLOCK == blockchainAction){
            kvWriteBatch->put(blockHeightKey, EncodeDecodeTool::encode(*block));
        }else{
            kvWriteBatch->delete0(blockHeightKey);
        }
    }

/**
 * 存储已使用的哈希
 */
    void BlockchainDatabase::storeHash(KvWriteBatch *kvWriteBatch, Block *block, BlockchainAction blockchainAction) {
        vector<unsigned char> blockHashKey = BlockchainDatabaseKeyTool::buildHashKey(block->hash);
        if(ADD_BLOCK == blockchainAction){
            kvWriteBatch->put(blockHashKey, blockHashKey);
        } else {
            kvWriteBatch->delete0(blockHashKey);
        }
        vector<Transaction> transactions = block->transactions;
        if(!transactions.empty()){
            for(Transaction transaction:transactions){
                vector<unsigned char> transactionHashKey = BlockchainDatabaseKeyTool::buildHashKey(transaction.transactionHash);
                if(ADD_BLOCK == blockchainAction){
                    kvWriteBatch->put(transactionHashKey, transactionHashKey);
                } else {
                    kvWriteBatch->delete0(transactionHashKey);
                }
            }
        }
    }

/**
 * 存储已使用的地址
 */
    void BlockchainDatabase::storeAddress(KvWriteBatch *kvWriteBatch, Block *block, BlockchainAction blockchainAction) {
        vector<Transaction> transactions = block->transactions;
        if(!transactions.empty()){
            for(Transaction transaction:transactions){
                vector<TransactionOutput> outputs = transaction.outputs;
                if(!outputs.empty()){
                    for(TransactionOutput output:outputs){
                        vector<unsigned char> addressKey = BlockchainDatabaseKeyTool::buildAddressKey(output.address);
                        if(ADD_BLOCK == blockchainAction){
                            kvWriteBatch->put(addressKey, addressKey);
                        } else {
                            kvWriteBatch->delete0(addressKey);
                        }
                    }
                }
            }
        }
    }
/**
 * 存储地址到未花费交易输出列表
 */
    void BlockchainDatabase::storeAddressToUnspentTransactionOutputHeight(KvWriteBatch *kvWriteBatch, Block *block, BlockchainAction blockchainAction) {
        vector<Transaction> transactions = block->transactions;
        if(transactions.empty()){
            return;
        }
        for(Transaction transaction : transactions){
            vector<TransactionInput> inputs = transaction.inputs;
            if(!inputs.empty()){
                for (TransactionInput transactionInput:inputs){
                    TransactionOutput utxo = transactionInput.unspentTransactionOutput;
                    vector<unsigned char> addressToUnspentTransactionOutputHeightKey = BlockchainDatabaseKeyTool::buildAddressToUnspentTransactionOutputHeightKey(utxo.address);
                    if(blockchainAction == ADD_BLOCK){
                        kvWriteBatch->delete0(addressToUnspentTransactionOutputHeightKey);
                    }else{
                        kvWriteBatch->put(addressToUnspentTransactionOutputHeightKey, ByteUtil::uint64ToBytes(utxo.transactionOutputHeight));
                    }
                }
            }
            vector<TransactionOutput> outputs = transaction.outputs;
            if(!outputs.empty()){
                for (TransactionOutput transactionOutput:outputs){
                    vector<unsigned char> addressToUnspentTransactionOutputHeightKey = BlockchainDatabaseKeyTool::buildAddressToUnspentTransactionOutputHeightKey(transactionOutput.address);
                    if(blockchainAction == ADD_BLOCK){
                        kvWriteBatch->put(addressToUnspentTransactionOutputHeightKey, ByteUtil::uint64ToBytes(transactionOutput.transactionOutputHeight));
                    }else{
                        kvWriteBatch->delete0(addressToUnspentTransactionOutputHeightKey);
                    }
                }
            }
        }
    }
/**
 * 存储地址到交易输出
 */
    void BlockchainDatabase::storeAddressToTransactionOutputHeight(KvWriteBatch *kvWriteBatch, Block *block, BlockchainAction blockchainAction) {
        vector<Transaction> transactions = block->transactions;
        if(!transactions.empty()){
            return;
        }
        for(Transaction transaction : transactions){
            vector<TransactionOutput> outputs = transaction.outputs;
            if(!outputs.empty()){
                for (TransactionOutput transactionOutput:outputs){
                    vector<unsigned char> addressToTransactionOutputHeightKey = BlockchainDatabaseKeyTool::buildAddressToTransactionOutputHeightKey(transactionOutput.address);
                    if(blockchainAction == ADD_BLOCK){
                        kvWriteBatch->put(addressToTransactionOutputHeightKey, ByteUtil::uint64ToBytes(transactionOutput.transactionOutputHeight));
                    }else{
                        kvWriteBatch->delete0(addressToTransactionOutputHeightKey);
                    }
                }
            }
        }
    }
/**
 * 存储地址到交易输出高度
 */
    void BlockchainDatabase::storeAddressToSpentTransactionOutputHeight(KvWriteBatch *kvWriteBatch, Block *block, BlockchainAction blockchainAction) {
        vector<Transaction> transactions = block->transactions;
        if(!transactions.empty()){
            return;
        }
        for(Transaction transaction : transactions){
            vector<TransactionInput> inputs = transaction.inputs;
            if(!inputs.empty()){
                for (TransactionInput transactionInput:inputs){
                    TransactionOutput utxo = transactionInput.unspentTransactionOutput;
                    vector<unsigned char> addressToSpentTransactionOutputHeightKey = BlockchainDatabaseKeyTool::buildAddressToSpentTransactionOutputHeightKey(utxo.address);
                    if(blockchainAction == ADD_BLOCK){
                        kvWriteBatch->put(addressToSpentTransactionOutputHeightKey, ByteUtil::uint64ToBytes(utxo.transactionOutputHeight));
                    }else{
                        kvWriteBatch->delete0(addressToSpentTransactionOutputHeightKey);
                    }
                }
            }
        }
    }
//endregion

    string BlockchainDatabase::getBlockchainDatabasePath(){
        return FileUtil::newPath(coreConfiguration->getCorePath(), BLOCKCHAIN_DATABASE_NAME);
    }

//region 新产生的哈希相关
/**
 * 校验区块新产生的哈希
 */
    bool BlockchainDatabase::checkBlockNewHash(Block *block) {
        //校验哈希作为主键的正确性
        //新产生的哈希不能有重复
        if(BlockTool::isExistDuplicateNewHash(block)){
            LogUtil::debug("区块数据异常，区块中新产生的哈希有重复。");
            return false;
        }

        //新产生的哈希不能被区块链使用过了
        //校验区块Hash是否已经被使用了
        string blockHash = block->hash;
        if(isHashUsed(blockHash)){
            LogUtil::debug("区块数据异常，区块Hash已经被使用了。");
            return false;
        }
        //校验每一笔交易新产生的Hash是否正确
        vector<Transaction> blockTransactions = block->transactions;
        if(!blockTransactions.empty()){
            for(Transaction transaction:blockTransactions){
                if(!checkTransactionNewHash(&transaction)){
                    return false;
                }
            }
        }
        return true;
    }
/**
 * 区块中校验新产生的哈希
 */
    bool BlockchainDatabase::checkTransactionNewHash(Transaction *transaction) {
        //校验哈希作为主键的正确性
        //校验交易Hash是否已经被使用了
        string transactionHash = transaction->transactionHash;
        if(isHashUsed(transactionHash)){
            LogUtil::debug("交易数据异常，交易Hash已经被使用了。");
            return false;
        }
        return true;
    }
/**
 * 哈希是否已经被区块链系统使用了？
 */
    bool BlockchainDatabase::isHashUsed(string hash){
        vector<unsigned char> bytesHash = KvDbUtil::get(getBlockchainDatabasePath(), BlockchainDatabaseKeyTool::buildHashKey(hash));
        return !bytesHash.empty();
    }
//endregion

//region 新产生的地址相关
/**
 * 校验区块新产生的地址
 */
    bool BlockchainDatabase::checkBlockNewAddress(Block *block) {
        //校验地址作为主键的正确性
        //新产生的地址不能有重复
        if(BlockTool::isExistDuplicateNewAddress(block)){
            LogUtil::debug("区块数据异常，区块中新产生的地址有重复。");
            return false;
        }
        vector<Transaction> transactions = block->transactions;
        if(!transactions.empty()){
            for(Transaction transaction:transactions){
                if(!checkTransactionNewAddress(&transaction)){
                    return false;
                }
            }
        }
        return true;
    }
/**
 * 区块中校验新产生的哈希
 */
    bool BlockchainDatabase::checkTransactionNewAddress(Transaction *transaction) {
        //区块新产生的地址不能有重复
        if(TransactionTool::isExistDuplicateNewAddress(transaction)){
            return false;
        }
        //区块新产生的地址不能被使用过了
        vector<TransactionOutput> outputs = transaction->outputs;
        if(!outputs.empty()){
            for (TransactionOutput output:outputs){
                string address = output.address;
                if(isAddressUsed(address)){
                    LogUtil::debug("区块数据异常，地址["+address+"]重复。");
                    return false;
                }
            }
        }
        return true;
    }
    bool BlockchainDatabase::isAddressUsed(string address) {
        vector<unsigned char> bytesAddress = KvDbUtil::get(getBlockchainDatabasePath(), BlockchainDatabaseKeyTool::buildAddressKey(address));
        return !bytesAddress.empty();
    }
//endregion


//region 双花攻击
/**
 * 校验双花
 * 双花指的是同一笔UTXO被花费两次或多次。
 */
    bool BlockchainDatabase::checkBlockDoubleSpend(Block *block) {
        //双花交易：区块内部存在重复的[未花费交易输出]
        if(BlockTool::isExistDuplicateUtxo(block)){
            LogUtil::debug("区块数据异常：发生双花交易。");
            return false;
        }
        vector<Transaction> transactions = block->transactions;
        if(!transactions.empty()){
            for(Transaction transaction:transactions){
                if(!checkTransactionDoubleSpend(&transaction)){
                    LogUtil::debug("区块数据异常：发生双花交易。");
                    return false;
                }
            }
        }
        return true;
    }
/**
 * 校验双花
 */
    bool BlockchainDatabase::checkTransactionDoubleSpend(Transaction *transaction) {
        //双花交易：交易内部存在重复的[未花费交易输出]
        if(TransactionTool::isExistDuplicateUtxo(transaction)){
            LogUtil::debug("交易数据异常，检测到双花攻击。");
            return false;
        }
        //双花交易：交易内部使用了[已经花费的[未花费交易输出]]
        if(!checkStxoIsUtxo(transaction)){
            LogUtil::debug("交易数据异常：发生双花交易。");
            return false;
        }
        return true;
    }
/**
 * 检查[花费的交易输出]是否都是[未花费的交易输出]
 */
    bool BlockchainDatabase::checkStxoIsUtxo(Transaction *transaction) {
        vector<TransactionInput> inputs = transaction->inputs;
        if(!inputs.empty()){
            for(TransactionInput transactionInput : inputs) {
                TransactionOutput unspentTransactionOutput = transactionInput.unspentTransactionOutput;
                TransactionOutput transactionOutput = queryUnspentTransactionOutputByTransactionOutputId(unspentTransactionOutput.transactionHash,unspentTransactionOutput.transactionOutputIndex);
                if(NullTool::isNullTransactionOutput(transactionOutput)){
                    LogUtil::debug("交易数据异常：交易输入不是未花费交易输出。");
                    return false;
                }
            }
        }
        return true;
    }
    bool BlockchainDatabase::checkTransactionScript(Transaction *transaction){
        try{
            vector<TransactionInput> inputs = transaction->inputs;
            if(!inputs.empty()){
                for(TransactionInput transactionInput:inputs){
                    //锁(交易输出脚本)
                    OutputScript outputScript = transactionInput.unspentTransactionOutput.outputScript;
                    //钥匙(交易输入脚本)
                    InputScript inputScript = transactionInput.inputScript;
                    //完整脚本
                    Script script = ScriptTool::createScript(inputScript,outputScript);
                    //执行脚本
                    ScriptExecuteResult scriptExecuteResult = virtualMachine->executeScript(*transaction,script);
                    //脚本执行结果是个栈，如果栈有且只有一个元素，且这个元素是0x01，则解锁成功。
                    bool executeSuccess = scriptExecuteResult.size()==1 && ByteUtil::equals(BooleanCodeEnum::TRUE.code,ByteUtil::hexStringToBytes(scriptExecuteResult.top()));
                    if(!executeSuccess){
                        return false;
                    }
                }
            }
        }catch (exception e){
            LogUtil::error("交易校验失败：交易[输入脚本]解锁交易[输出脚本]异常。",e);
            return false;
        }
        return true;
    }
//endregion





//region dto to model
    Block BlockchainDatabase::blockDto2Block(BlockDto *blockDto) {
        string previousBlockHash = blockDto->previousHash;
        Block previousBlock = queryBlockByBlockHash(previousBlockHash);

        Block block;
        block.timestamp=blockDto->timestamp;
        block.previousHash=previousBlockHash;
        block.nonce=blockDto->nonce;

        long blockHeight = BlockTool::getNextBlockHeight(&previousBlock);
        block.height=blockHeight;

        vector<Transaction> transactions = transactionDtos2Transactions(&blockDto->transactions);
        block.transactions=transactions;

        string merkleTreeRoot = BlockTool::calculateBlockMerkleTreeRoot(&block);
        block.merkleTreeRoot=merkleTreeRoot;

        string blockHash = BlockTool::calculateBlockHash(&block);
        block.hash=blockHash;

        string difficult = getConsensus()->calculateDifficult(this,&block);
        block.difficulty=difficult;

        fillBlockProperty(&block);
        /*
         * 预先校验区块工作量共识。伪造工作量共识是一件十分耗费资源的事情，因此预先校验工作量共识可以抵消绝大部分的攻击。
         * 也可以选择跳过此处预检，后续业务有完整的校验检测。
         * 此处预检，只是想预先抵消绝大部分的攻击。
         */
        if(!getConsensus()->checkConsensus(this,&block)){
            throw new exception("区块预检失败。");
        }
        return block;
    }
    vector<Transaction> BlockchainDatabase::transactionDtos2Transactions(vector<TransactionDto> *transactionDtos) {
        vector<Transaction> transactions;
        if(transactionDtos != nullptr){
            for(TransactionDto &transactionDto:*transactionDtos){
                Transaction transaction = transactionDto2Transaction(&transactionDto);
                transactions.push_back(transaction);
            }
        }
        return transactions;
    }
    Transaction BlockchainDatabase::transactionDto2Transaction(TransactionDto *transactionDto) {
        vector<TransactionInput> inputs;
        vector<TransactionInputDto> transactionInputDtos = transactionDto->inputs;
        if(!transactionInputDtos.empty()){
            for (TransactionInputDto transactionInputDto:transactionInputDtos){
                TransactionOutput unspentTransactionOutput = queryUnspentTransactionOutputByTransactionOutputId(transactionInputDto.transactionHash,transactionInputDto.transactionOutputIndex);
                if(NullTool::isNullTransactionOutput(unspentTransactionOutput)){
                    throw new exception("非法交易。交易输入并不是一笔未花费交易输出。");
                }
                TransactionInput transactionInput;
                transactionInput.unspentTransactionOutput = unspentTransactionOutput;
                InputScript inputScript = inputScriptDto2InputScript(&transactionInputDto.inputScript);
                transactionInput.inputScript = inputScript;
                inputs.push_back(transactionInput);
            }
        }

        vector<TransactionOutput> outputs;
        vector<TransactionOutputDto> transactionOutputDtos = transactionDto->outputs;
        if(!transactionOutputDtos.empty()){
            for(TransactionOutputDto transactionOutputDto:transactionOutputDtos){
                TransactionOutput transactionOutput = transactionOutputDto2TransactionOutput(&transactionOutputDto);
                outputs.push_back(transactionOutput);
            }
        }

        Transaction transaction;
        TransactionType transactionType = obtainTransactionDto(transactionDto);
        transaction.transactionType=transactionType;
        transaction.transactionHash=TransactionDtoTool::calculateTransactionHash(*transactionDto);
        transaction.inputs=inputs;
        transaction.outputs=outputs;
        return transaction;
    }
    TransactionOutput BlockchainDatabase::transactionOutputDto2TransactionOutput(TransactionOutputDto *transactionOutputDto) {
        TransactionOutput transactionOutput;
        string publicKeyHash = ScriptDtoTool::getPublicKeyHashFromPayToPublicKeyHashOutputScript(transactionOutputDto->outputScript);
        string address = AccountUtil::addressFromPublicKeyHash(publicKeyHash);
        transactionOutput.address=address;
        transactionOutput.value=transactionOutputDto->value;
        OutputScript outputScript = outputScriptDto2OutputScript(&transactionOutputDto->outputScript);
        transactionOutput.outputScript=outputScript;
        return transactionOutput;
    }
    TransactionType BlockchainDatabase::obtainTransactionDto(TransactionDto *transactionDto) {
        if(transactionDto->inputs.empty()){
            return TransactionTypeEnum::GENESIS_TRANSACTION;
        }
        return TransactionTypeEnum::STANDARD_TRANSACTION;
    }
    OutputScript BlockchainDatabase::outputScriptDto2OutputScript(OutputScriptDto *outputScriptDto) {
        if(outputScriptDto == nullptr){
            return OutputScript{};
        }
        OutputScript outputScript;
        for (string s:*outputScriptDto) {
            outputScript.push_back(s);
        }
        return outputScript;
    }
    InputScript BlockchainDatabase::inputScriptDto2InputScript(InputScriptDto *inputScriptDto) {
        if(inputScriptDto == nullptr){
            return InputScript{};
        }
        InputScript inputScript;
        for (string s:*inputScriptDto) {
            inputScript.push_back(s);
        }
        return inputScript;
    }
/**
 * 补充区块的属性
 */
    void BlockchainDatabase::fillBlockProperty(Block *block) {
        uint64_t transactionIndex = 0;
        uint64_t transactionHeight = queryBlockchainTransactionHeight();
        uint64_t transactionOutputHeight = queryBlockchainTransactionOutputHeight();
        uint64_t blockHeight = block->height;
        string blockHash = block->hash;
        vector<Transaction> &transactions = block->transactions;
        uint64_t transactionCount = BlockTool::getTransactionCount(block);
        block->transactionCount=transactionCount;
        block->previousTransactionHeight=transactionHeight;
        if(!transactions.empty()){
            for(Transaction &transaction:transactions){
                transactionIndex++;
                transactionHeight++;
                transaction.blockHeight=blockHeight;
                transaction.transactionIndex=transactionIndex;
                transaction.transactionHeight=transactionHeight;

                vector<TransactionOutput> &outputs = transaction.outputs;
                if(!outputs.empty()){
                    for (int i=0; i <outputs.size(); i++){
                        transactionOutputHeight++;
                        TransactionOutput &output = outputs[i];
                        output.blockHeight=blockHeight;
                        output.blockHash=blockHash;
                        output.transactionHeight=transactionHeight;
                        output.transactionHash=transaction.transactionHash;
                        output.transactionOutputIndex=i+1;
                        output.transactionIndex=transaction.transactionIndex;
                        output.transactionOutputHeight=transactionOutputHeight;
                    }
                }
            }
        }
    }

    BlockchainDatabase::BlockchainDatabase(CoreConfiguration *coreConfiguration, Incentive *incentive,
                                           Consensus *consensus, VirtualMachine *virtualMachine) {
        this->coreConfiguration = coreConfiguration;
        this->incentive = incentive;
        this->consensus = consensus;
        this->virtualMachine = virtualMachine;
    }
//endregion
}