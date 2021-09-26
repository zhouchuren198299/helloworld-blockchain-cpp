//
// Created by 40906 on 2021/9/20.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_BLOCKCHAINCORE_H
#define HELLOWORLD_BLOCKCHAIN_CPP_BLOCKCHAINCORE_H


#include "BlockchainDatabase.h"
#include "UnconfirmedTransactionDatabase.h"
#include "Miner.h"

namespace core{
    class BlockchainCore {
    private:
        CoreConfiguration *coreConfiguration;
        BlockchainDatabase *blockchainDatabase;
        UnconfirmedTransactionDatabase *unconfirmedTransactionDatabase;
        Wallet *wallet;
        Miner *miner;
    public:
        BlockchainCore(CoreConfiguration *coreConfiguration, BlockchainDatabase *blockchainDatabase,
                       UnconfirmedTransactionDatabase *unconfirmedTransactionDatabase, Wallet *wallet, Miner *miner);

        //TODO 删除
        //void BlockchainCore::startBlockchainCore();

        /**
         * 激活区块链核心。包含激活矿工等操作。
         */
        void start();



        /**
         * 将一个区块添加到区块链
         */
        bool addBlockDto(BlockDto *blockDto);
        /**
         * 将一个区块添加到区块链
         */
        bool addBlock(Block *block);
        /**
         * 删除区块链的尾巴区块(最后一个区块)
         */
        void deleteTailBlock();
        /**
         * 删除区块高度大于等于@blockHeight@的区块
         */
        void deleteBlocks(uint64_t blockHeight) ;
        /**
         * 获取区块链高度
         */
        uint64_t queryBlockchainHeight() ;
        /**
         * 根据区块高度查询区块
         */
        Block queryBlockByBlockHeight(uint64_t blockHeight);
        /**
         * 根据区块哈希查询区块
         */
        Block queryBlockByBlockHash(string blockHash);
        /**
         * 查询区块链尾巴区块
         */
        Block queryTailBlock();




        /**
         * 根据交易哈希获取交易
         */
        Transaction queryTransactionByTransactionHash(string transactionHash) ;
        /**
         * 根据交易高度获取交易
         * @param transactionHeight 交易高度。注意：区块高度从1开始。
         */
        Transaction queryTransactionByTransactionHeight(uint64_t transactionHeight) ;




        /**
         * 根据地址获取[交易输出]。
         */
        TransactionOutput queryTransactionOutputByAddress(string address) ;
        /**
         * 根据地址获取[未花费交易输出]。
         */
        TransactionOutput queryUnspentTransactionOutputByAddress(string address) ;
        /**
         * 根据地址获取[已花费交易输出]。
         */
        TransactionOutput querySpentTransactionOutputByAddress(string address) ;




        /**
         * 构建交易。
         */
        AutoBuildTransactionResponse autoBuildTransaction(AutoBuildTransactionRequest *request) ;
        /**
         * 提交交易到区块链
         */
        void postTransaction(TransactionDto *transactionDto) ;
        /**
         * 查询未确认的交易
         */
        vector<TransactionDto> queryUnconfirmedTransactions(uint64_t from, uint64_t size) ;
        /**
         * 根据交易哈希查询未确认交易
         */
        TransactionDto queryUnconfirmedTransactionByTransactionHash(string transactionHash) ;



        //region
        /**
         * block dto to block model
         */
        Block blockDto2Block(BlockDto *blockDto) ;
        /**
         * transaction dto to transaction model
         */
        Transaction transactionDto2Transaction(TransactionDto *transactionDto) ;
        //endregion


        //region get set
        BlockchainDatabase * getBlockchainDatabase() ;

        UnconfirmedTransactionDatabase * getUnconfirmedTransactionDatabase()  ;

        Miner * getMiner() ;

        Wallet * getWallet()  ;

        CoreConfiguration * getCoreConfiguration() ;
        //endregion
    };
}


#endif //HELLOWORLD_BLOCKCHAIN_CPP_BLOCKCHAINCORE_H
