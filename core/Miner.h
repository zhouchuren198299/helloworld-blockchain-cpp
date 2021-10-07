//
// Created by 40906 on 2021/9/20.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_MINER_H
#define HELLOWORLD_BLOCKCHAIN_CPP_MINER_H

#include "Wallet.h"
#include "UnconfirmedTransactionDatabase.h"
#include "CoreConfiguration.h"
#include "BlockchainDatabase.h"

namespace core{

    class Miner {
    private:
        CoreConfiguration *coreConfiguration;
        Wallet *wallet;
        BlockchainDatabase *blockchainDatabase;
        UnconfirmedTransactionDatabase *unconfirmedTransactionDatabase;
    public:
        Miner(CoreConfiguration *coreConfiguration, Wallet *wallet, BlockchainDatabase *blockchainDatabase,
              UnconfirmedTransactionDatabase *unconfirmedTransactionDatabase);

    public:
        /**
         * 启用矿工。
         * 矿工有两种状态：活动状态与非活动状态。
         * 若矿工处于活动作态，矿工会进行挖矿劳作。
         * 若矿工处于非活动状态，矿工不会进行任何工作。
         */
        void start() ;

        /**
         * 矿工是否处于活动状态。
         */
        bool isActive() ;

        /**
         * 激活矿工：设置矿工为活动状态。
         */
        void active() ;

        /**
         * 停用矿工：设置矿工为非活动状态。
         */
        void deactive() ;

        /**
         * 设置矿工可挖掘的最高区块高度
         */
        void setMinerMineMaxBlockHeight(uint64_t maxHeight) ;
        /**
         * 获取矿工可挖掘的最高区块高度
         */
        uint64_t getMinerMineMaxBlockHeight( ) ;
        //endregion

    public:
        //region get set
        Wallet * getWallet() ;

        BlockchainDatabase * getBlockchainDatabase() ;

        UnconfirmedTransactionDatabase * getUnconfirmedTransactionDatabase() ;

        CoreConfiguration * getCoreConfiguration();
        //endregion
    private:
        Block Miner::buildMiningBlock(BlockchainDatabase *blockchainDatabase, UnconfirmedTransactionDatabase *unconfirmedTransactionDatabase, Account *minerAccount);
        Transaction Miner::buildIncentiveTransaction(string address,uint64_t incentiveValue);
        vector<Transaction> Miner::packingTransactions(BlockchainDatabase *blockchainDatabase, UnconfirmedTransactionDatabase *unconfirmedTransactionDatabase);
    };
}


#endif //HELLOWORLD_BLOCKCHAIN_CPP_MINER_H
