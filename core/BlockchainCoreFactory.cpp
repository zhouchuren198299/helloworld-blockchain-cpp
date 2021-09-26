//
// Created by 40906 on 2021/9/20.
//

#include "BlockchainCoreFactory.h"
#include "tool/ResourcePathTool.h"
using namespace core;

namespace core{
    namespace BlockchainCoreFactory {
        BlockchainCore* createDefaultBlockchainCore(){
            return createBlockchainCore(ResourcePathTool::getDataRootPath());
        }
        BlockchainCore* createBlockchainCore(string corePath){
            CoreConfiguration *coreConfiguration = new CoreConfiguration(corePath);
            Incentive *incentive = new Incentive();
            Consensus *consensus = new Consensus();
            VirtualMachine *virtualMachine = new VirtualMachine();
            BlockchainDatabase *blockchainDatabase = new BlockchainDatabase(coreConfiguration,incentive,consensus,virtualMachine);


            UnconfirmedTransactionDatabase *unconfirmedTransactionDatabase = new UnconfirmedTransactionDatabase(coreConfiguration);
            Wallet *wallet = new Wallet(coreConfiguration,blockchainDatabase);
            Miner *miner = new Miner(coreConfiguration,wallet,blockchainDatabase,unconfirmedTransactionDatabase);
            BlockchainCore *blockchainCore = new BlockchainCore(coreConfiguration,blockchainDatabase,unconfirmedTransactionDatabase,wallet,miner);
            return blockchainCore;
        }
    };
}
