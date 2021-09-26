//
// Created by 40906 on 2021/9/20.
//

#include <set>
#include "Miner.h"
#include "../util/ThreadUtil.h"
#include "../util/TimeUtil.h"
#include "../util/ByteUtil.h"
#include "../util/LogUtil.h"
#include "../util/StringUtil.h"
#include "tool/Model2DtoTool.h"
#include "tool/NullTool.h"
#include "tool/ScriptTool.h"
#include "../setting/BlockSetting.h"
#include "tool/SizeTool.h"
#include "tool/TransactionTool.h"
#include "../setting/GenesisBlockSetting.h"
#include "tool/BlockTool.h"
#include "tool/TransactionDtoTool.h"


namespace core{

    Miner::Miner(CoreConfiguration *coreConfiguration, Wallet *wallet, BlockchainDatabase *blockchainDatabase,
                 UnconfirmedTransactionDatabase *unconfirmedTransactionDatabase) {
        this->coreConfiguration= coreConfiguration;
        this->wallet = wallet;
        this->blockchainDatabase = blockchainDatabase;
        this->unconfirmedTransactionDatabase = unconfirmedTransactionDatabase;
    }


    void Miner::start() {
        while (true){
            //防止空跑，浪费CPU
            ThreadUtil::millisecondSleep(10);
            if(!isActive()){
                continue;
            }

            uint64_t blockChainHeight = blockchainDatabase->queryBlockchainHeight();
            //'当前区块链的高度'是否大于'矿工最大被允许的挖矿高度'
            if(blockChainHeight >= coreConfiguration->getMinerMineMaxBlockHeight()){
                continue;
            }

            //创建一个账户用于存放挖矿成功后发放的激励金额
            Account minerAccount = wallet->createAccount();
            Block block = buildMiningBlock(blockchainDatabase,unconfirmedTransactionDatabase,&minerAccount);
            uint64_t startTimestamp = TimeUtil::millisecondTimestamp();
            while (true){
                if(!isActive()){
                    break;
                }
                //在挖矿的期间，可能收集到新的交易。每隔一定的时间，重新组装挖矿中的区块，这样新收集到交易就可以被放进挖矿中的区块了。
                if(TimeUtil::millisecondTimestamp()-startTimestamp > coreConfiguration->getMinerMineTimeInterval()){
                    break;
                }
                //随机数
                block.nonce = ByteUtil::bytesToHexString(ByteUtil::random32Bytes());
                //计算区块哈希
                block.hash = BlockTool::calculateBlockHash(&block);
                //判断共识是否达成(即挖矿是否成功)
                if(blockchainDatabase->getConsensus()->checkConsensus(blockchainDatabase,&block)){
                    //挖到矿了，账户里有挖矿成功发放的激励金额，将账户放入钱包。
                    wallet->saveAccount(minerAccount);
                    LogUtil::debug("祝贺您！挖矿成功！！！区块高度:"+StringUtil::valueOfUint64(block.height)+",区块哈希:"+block.hash);
                    //业务模型转换
                    BlockDto blockDto = Model2DtoTool::block2BlockDto(&block);
                    //将矿放入区块链
                    bool isAddBlockToBlockchainSuccess = blockchainDatabase->addBlockDto(&blockDto);
                    if(!isAddBlockToBlockchainSuccess){
                        LogUtil::debug("挖矿成功，但是区块放入区块链失败。");
                    }
                    break;
                }
            }
        }
    }


    void Miner::deactive() {
        coreConfiguration->deactiveMiner();
    }


    void Miner::active() {
        coreConfiguration->activeMiner();
    }


    bool Miner::isActive() {
        return coreConfiguration->isMinerActive();
    }



    void Miner::setMinerMineMaxBlockHeight(uint64_t maxHeight) {
        coreConfiguration->setMinerMineMaxBlockHeight(maxHeight);
    }


    uint64_t Miner::getMinerMineMaxBlockHeight( ) {
        return coreConfiguration->getMinerMineMaxBlockHeight();
    }

    /**
     * 构建挖矿区块
     */
    Block Miner::buildMiningBlock(BlockchainDatabase *blockchainDatabase, UnconfirmedTransactionDatabase *unconfirmedTransactionDatabase, Account *minerAccount) {
        uint64_t timestamp = TimeUtil::millisecondTimestamp();

        Block tailBlock = blockchainDatabase->queryTailBlock();
        Block nonNonceBlock;
        //这个挖矿时间不需要特别精确，没必要非要挖出矿的前一霎那时间。
        nonNonceBlock.timestamp=timestamp;

        if(NullTool::isNullBlock(tailBlock)){
            nonNonceBlock.height=GenesisBlockSetting::HEIGHT +1;
            nonNonceBlock.previousHash=GenesisBlockSetting::HASH;
        } else {
            nonNonceBlock.height=tailBlock.height+1;
            nonNonceBlock.previousHash=tailBlock.hash;
        }
        vector<Transaction> packingTransactions0 = packingTransactions(blockchainDatabase,unconfirmedTransactionDatabase);
        nonNonceBlock.transactions=packingTransactions0;

        //创建挖矿奖励交易
        //激励金额
        Incentive *incentive = blockchainDatabase->getIncentive();
        uint64_t incentiveValue = incentive->incentiveValue(blockchainDatabase,&nonNonceBlock);
        //激励交易
        Transaction mineAwardTransaction = buildIncentiveTransaction(minerAccount->address,incentiveValue);
        nonNonceBlock.transactions.insert(nonNonceBlock.transactions.begin(),mineAwardTransaction);

        string merkleTreeRoot = BlockTool::calculateBlockMerkleTreeRoot(&nonNonceBlock);
        nonNonceBlock.merkleTreeRoot=merkleTreeRoot;

        //计算挖矿难度
        nonNonceBlock.difficulty=blockchainDatabase->getConsensus()->calculateDifficult(blockchainDatabase,&nonNonceBlock);
        return nonNonceBlock;
    }
    /**
     * 构建区块的挖矿奖励交易。
     */
    Transaction Miner::buildIncentiveTransaction(string address,uint64_t incentiveValue) {
        Transaction transaction;
        transaction.transactionType=TransactionTypeEnum::GENESIS_TRANSACTION;

        vector<TransactionOutput> outputs;
        TransactionOutput output;
        output.address=address;
        output.value=incentiveValue;
        output.outputScript=ScriptTool::createPayToPublicKeyHashOutputScript(address);
        outputs.push_back(output);

        transaction.outputs=outputs;
        transaction.transactionHash=TransactionTool::calculateTransactionHash(&transaction);
        return transaction;
    }
    /**
     * 确认打包哪些'未确认交易'
     */
    vector<Transaction> Miner::packingTransactions(BlockchainDatabase *blockchainDatabase, UnconfirmedTransactionDatabase *unconfirmedTransactionDatabase) {
        //获取一部分未确认交易，最优的方式是获取所有未确认的交易进行处理，但是数据处理起来会很复杂，因为项目是helloworld的，所以简单的拿一部分数据即可。
        vector<TransactionDto> forMineBlockTransactionDtos = unconfirmedTransactionDatabase->selectTransactions(1,10000);

        vector<Transaction> transactions;
        vector<Transaction> backupTransactions;

        if(!forMineBlockTransactionDtos.empty()){
            for(TransactionDto transactionDto:forMineBlockTransactionDtos){
                try {
                    Transaction transaction = blockchainDatabase->transactionDto2Transaction(&transactionDto);
                    transactions.push_back(transaction);
                } catch (exception e) {
                    string transactionHash = TransactionDtoTool::calculateTransactionHash(transactionDto);
                    LogUtil::error("类型转换异常,将从挖矿交易数据库中删除该交易["+transactionHash+"]。",e);
                    unconfirmedTransactionDatabase->deleteByTransactionHash(transactionHash);
                }
            }
        }

        backupTransactions.clear();
        backupTransactions=transactions;
        transactions.clear();
        for(Transaction &transaction : backupTransactions){
            bool checkTransaction = blockchainDatabase->checkTransaction(&transaction);
            if(checkTransaction){
                transactions.push_back(transaction);
            }else {
                string transactionHash = TransactionTool::calculateTransactionHash(&transaction);
                LogUtil::debug("交易不能被挖矿,将从挖矿交易数据库中删除该交易。交易哈希"+transactionHash);
                unconfirmedTransactionDatabase->deleteByTransactionHash(transactionHash);
            }
        }

        backupTransactions.clear();
        backupTransactions=transactions;
        transactions.clear();
        //防止双花
        set<string> transactionOutputIdSet;
        for(Transaction transaction : backupTransactions){
            vector<TransactionInput> inputs = transaction.inputs;
            if(!inputs.empty()){
                bool canAdd = true;
                for(TransactionInput transactionInput : inputs) {
                    TransactionOutput unspentTransactionOutput = transactionInput.unspentTransactionOutput;
                    string transactionOutputId = TransactionTool::getTransactionOutputId(&unspentTransactionOutput);
                    if(transactionOutputIdSet.count(transactionOutputId)>0){
                        canAdd = false;
                        break;
                    }else {
                        transactionOutputIdSet.insert(transactionOutputId);
                    }
                }
                if(canAdd){
                    transactions.push_back(transaction);
                }else {
                    string transactionHash = TransactionTool::calculateTransactionHash(&transaction);
                    LogUtil::debug("交易不能被挖矿,将从挖矿交易数据库中删除该交易。交易哈希"+transactionHash);
                    unconfirmedTransactionDatabase->deleteByTransactionHash(transactionHash);
                }
            }
        }



        backupTransactions.clear();
        backupTransactions=transactions;
        transactions.clear();
        //防止一个地址被用多次
        set<string> addressSet;
        for(Transaction transaction : backupTransactions){
            vector<TransactionOutput> outputs = transaction.outputs;
            if(!outputs.empty()){
                bool canAdd = true;
                for (TransactionOutput output:outputs){
                    string address = output.address;
                    if(addressSet.count(address)>0){
                        canAdd = false;
                        break;
                    }else {
                        addressSet.insert(address);
                    }
                }
                if(canAdd){
                    transactions.push_back(transaction);
                }else {
                    string transactionHash = TransactionTool::calculateTransactionHash(&transaction);
                    LogUtil::debug("交易不能被挖矿,将从挖矿交易数据库中删除该交易。交易哈希"+transactionHash);
                    unconfirmedTransactionDatabase->deleteByTransactionHash(transactionHash);
                }
            }
        }


        //按照费率(每字符的手续费)从大到小排序交易
        TransactionTool::sortByTransactionFeeRateDescend(&transactions);


        backupTransactions.clear();
        backupTransactions=transactions;
        transactions.clear();
        //到此时，剩余交易都是经过验证的了，且按照交易费率从大到小排列了。
        //尽可能多的获取交易
        uint64_t size = 0;
        for(int i=0; i<backupTransactions.size(); i++){
            //序号从0开始，加一。
            //留给挖矿交易一个位置，减一。
            if(i+1 > BlockSetting::BLOCK_MAX_TRANSACTION_COUNT-1){
                break;
            }
            Transaction transaction = backupTransactions[i];
            size += SizeTool::calculateTransactionSize(&transaction);
            if(size > BlockSetting::BLOCK_MAX_CHARACTER_COUNT){
                break;
            }
            transactions.push_back(transaction);
        }
        return transactions;
    }


    //region get set
    Wallet * Miner::getWallet() {
        return this->wallet;
    }
    BlockchainDatabase * Miner::getBlockchainDatabase() {
        return this->blockchainDatabase;
    }
    UnconfirmedTransactionDatabase * Miner::getUnconfirmedTransactionDatabase() {
        return this->unconfirmedTransactionDatabase;
    }
    CoreConfiguration * Miner::getCoreConfiguration() {
        return this->coreConfiguration;
    }
    //endregion
}