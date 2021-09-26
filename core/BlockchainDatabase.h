//
// Created by 40906 on 2021/9/20.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_BLOCKCHAINDATABASE_H
#define HELLOWORLD_BLOCKCHAIN_CPP_BLOCKCHAINDATABASE_H


#include "Incentive.h"
#include "Consensus.h"
#include "VirtualMachine.h"
#include "CoreConfiguration.h"
#include "../util/KvDbUtil.h"
#include <mutex>

using namespace BlockchainActionEnum;
using namespace TransactionTypeEnum;
using namespace KvDbUtil;

namespace core{
    class Incentive;
    class Consensus;
    class BlockchainDatabase {
    private:
        //区块共识
        Consensus *consensus ;
        //矿工激励
        Incentive *incentive ;
        //虚拟机
        VirtualMachine *virtualMachine;
        CoreConfiguration *coreConfiguration;
    public:
        //region 区块增加与删除
        /**
         * 将一个区块添加到区块链的尾部。
         */
        bool addBlockDto(BlockDto *blockDto);

        /**
         * 删除区块链的尾巴区块(最后一个区块)
         */
        void deleteTailBlock();

        /**
         * 删除区块高度大于等于@blockHeight@的区块
         */
        void deleteBlocks(uint64_t blockHeight);
        //endregion



        //region 校验区块、交易
        /**
         * 检测区块是否可以被添加到区块链上
         * 只有一种情况，区块可以被添加到区块链，即: 区块是区块链上的下一个区块。
         */
        bool checkBlock(Block *block);

        /**
         * 校验交易是否可以被添加进下一个区块之中。
         * 注意，如果是创世交易，则会跳过激励金额的校验，但除了不校验激励金额外，仍然会校验创世交易的方方面面，如交易大小、交易的结构等。
         * 为什么会跳过创世交易的激励金额的校验？
         * 因为激励金额的校验需要整个区块的信息，因此激励校验是区块层面的校验，而不是在交易层面校验激励金额。
         */
        bool checkTransaction(Transaction *transaction);
        //endregion



        //region 区块链查询
        /**
         * 查询区块链的长度
         */
        uint64_t queryBlockchainHeight();

        /**
         * 查询区块链中总的交易数量
         */
        uint64_t queryBlockchainTransactionHeight();

        /**
         * 查询区块链中总的交易输出数量
         */
        uint64_t queryBlockchainTransactionOutputHeight();
        //endregion



        //region 区块查询
        /**
         * 查询区块链上的最后一个区块
         */
        Block queryTailBlock();

        /**
         * 在区块链中根据区块高度查找区块
         */
        Block queryBlockByBlockHeight(uint64_t blockHeight);

        /**
         * 在区块链中根据区块哈希查找区块
         */
        Block queryBlockByBlockHash(string blockHash);
        //endregion



        //region 交易查询
        /**
         * 根据交易高度查询交易。交易高度从1开始。
         */
        Transaction queryTransactionByTransactionHeight(uint64_t transactionHeight);

        /**
         * 在区块链中根据交易哈希查找交易
         */
        Transaction queryTransactionByTransactionHash(string transactionHash);

        /**
         * 查询来源交易：查询交易输出产生于的那笔交易
         */
        Transaction
        querySourceTransactionByTransactionOutputId(string transactionHash, uint64_t transactionOutputIndex);

        /**
         * 查询去向交易：查询使用交易输出的那笔交易
         */
        Transaction
        queryDestinationTransactionByTransactionOutputId(string transactionHash, uint64_t transactionOutputIndex);
        //endregion



        //region 交易输出查询
        /**
         * 根据 交易输出高度 查找 交易输出
         */
        TransactionOutput queryTransactionOutputByTransactionOutputHeight(uint64_t transactionOutputHeight);

        /**
         * 根据 交易输出ID 查找 交易输出
         */
        TransactionOutput
        queryTransactionOutputByTransactionOutputId(string transactionHash, uint64_t transactionOutputIndex);

        /**
         * 根据 交易输出ID 查找 未花费交易输出
         */
        TransactionOutput
        queryUnspentTransactionOutputByTransactionOutputId(string transactionHash, uint64_t transactionOutputIndex);

        /**
         * 根据 交易输出ID 查找 已花费交易输出
         */
        TransactionOutput
        querySpentTransactionOutputByTransactionOutputId(string transactionHash, uint64_t transactionOutputIndex);
        //endregion



        //region 地址查询
        /**
         * 根据 地址 查询 交易输出
         */
        TransactionOutput queryTransactionOutputByAddress(string address);

        /**
         * 根据 地址 查询 未花费交易输出
         */
        TransactionOutput queryUnspentTransactionOutputByAddress(string address);

        /**
         * 根据 地址 查询 已花费交易输出
         */
        TransactionOutput querySpentTransactionOutputByAddress(string address);
        //endregion

        //region
        /**
         * block dto to block model
         */
        Block blockDto2Block(BlockDto *blockDto);

        /**
         * transaction dto to transaction model
         */
        Transaction transactionDto2Transaction(TransactionDto *transactionDto);
        //endregion


        //region get set
        Incentive* getIncentive();

        Consensus* getConsensus();

        BlockchainDatabase(CoreConfiguration *coreConfiguration, Incentive *incentive, Consensus *consensus,
                           VirtualMachine *virtualMachine);

    private:
        mutex mutex;

    private:
        /**
         * 根据区块信息组装WriteBatch对象
         */
        KvWriteBatch createBlockWriteBatch(Block *block, BlockchainAction blockchainAction);

        /**
         * [交易输出ID]到[来源交易高度]的映射
         */
        void storeTransactionOutputIdToSourceTransactionHeight(KvWriteBatch *kvWriteBatch, Block *block,
                                                               BlockchainAction blockchainAction);

        /**
         * [已花费交易输出ID]到[去向交易高度]的映射
         */
        void storeTransactionOutputIdToDestinationTransactionHeight(KvWriteBatch *kvWriteBatch, Block *block,
                                                                    BlockchainAction blockchainAction);

        /**
         * [交易输出ID]到[交易输出]的映射
         */
        void storeTransactionOutputIdToTransactionOutputHeight(KvWriteBatch *kvWriteBatch, Block *block,
                                                               BlockchainAction blockchainAction);

        /**
         * [交易输出高度]到[交易输出]的映射
         */
        void storeTransactionOutputHeightToTransactionOutput(KvWriteBatch *kvWriteBatch, Block *block,
                                                             BlockchainAction blockchainAction);

        /**
         * 存储未花费交易输出ID到未花费交易输出的映射
         */
        void storeTransactionOutputIdToUnspentTransactionOutputHeight(KvWriteBatch *kvWriteBatch, Block *block,
                                                                      BlockchainAction blockchainAction);

        /**
         * 存储已花费交易输出ID到已花费交易输出的映射
         */
        void storeTransactionOutputIdToSpentTransactionOutputHeight(KvWriteBatch *kvWriteBatch, Block *block,
                                                                    BlockchainAction blockchainAction);

        /**
         * 存储交易高度到交易的映射
         */
        void storeTransactionHeightToTransaction(KvWriteBatch *kvWriteBatch, Block *block,
                                                 BlockchainAction blockchainAction);

        /**
         * 存储交易哈希到交易高度的映射
         */
        void storeTransactionHashToTransactionHeight(KvWriteBatch *kvWriteBatch, Block *block,
                                                     BlockchainAction blockchainAction);

        /**
         * 存储区块链的高度
         */
        void storeBlockchainHeight(KvWriteBatch *kvWriteBatch, Block *block, BlockchainAction blockchainAction);

        /**
         * 存储区块哈希到区块高度的映射
         */
        void
        storeBlockHashToBlockHeight(KvWriteBatch *kvWriteBatch, Block *block, BlockchainAction blockchainAction);

        /**
         * 存储区块链中总的交易高度
         */
        void storeBlockchainTransactionHeight(KvWriteBatch *kvWriteBatch, Block *block,
                                              BlockchainAction blockchainAction);

        /**
         * 存储区块链中总的交易数量
         */
        void storeBlockchainTransactionOutputHeight(KvWriteBatch *kvWriteBatch, Block *block,
                                                    BlockchainAction blockchainAction);

        /**
         * 存储区块链高度到区块的映射
         */
        void storeBlockHeightToBlock(KvWriteBatch *kvWriteBatch, Block *block, BlockchainAction blockchainAction);

        /**
         * 存储已使用的哈希
         */
        void storeHash(KvWriteBatch *kvWriteBatch, Block *block, BlockchainAction blockchainAction);

        /**
         * 存储已使用的地址
         */
        void storeAddress(KvWriteBatch *kvWriteBatch, Block *block, BlockchainAction blockchainAction);

        /**
         * 存储地址到未花费交易输出列表
         */
        void storeAddressToUnspentTransactionOutputHeight(KvWriteBatch *kvWriteBatch, Block *block,
                                                          BlockchainAction blockchainAction);

        /**
         * 存储地址到交易输出
         */
        void storeAddressToTransactionOutputHeight(KvWriteBatch *kvWriteBatch, Block *block,
                                                   BlockchainAction blockchainAction);

        /**
         * 存储地址到交易输出高度
         */
        void storeAddressToSpentTransactionOutputHeight(KvWriteBatch *kvWriteBatch, Block *block,
                                                        BlockchainAction blockchainAction);
//endregion

        string getBlockchainDatabasePath();

//region 新产生的哈希相关
        /**
         * 校验区块新产生的哈希
         */
        bool checkBlockNewHash(Block *block);

        /**
         * 区块中校验新产生的哈希
         */
        bool checkTransactionNewHash(Transaction *transaction);

        /**
         * 哈希是否已经被区块链系统使用了？
         */
        bool isHashUsed(string hash);
//endregion

//region 新产生的地址相关
        /**
         * 校验区块新产生的地址
         */
        bool checkBlockNewAddress(Block *block);

        /**
         * 区块中校验新产生的哈希
         */
        bool checkTransactionNewAddress(Transaction *transaction);

        bool isAddressUsed(string address);
//endregion


//region 双花攻击
        /**
         * 校验双花
         * 双花指的是同一笔UTXO被花费两次或多次。
         */
        bool checkBlockDoubleSpend(Block *block);

        /**
         * 校验双花
         */
        bool checkTransactionDoubleSpend(Transaction *transaction);

        /**
         * 检查[花费的交易输出]是否都是[未花费的交易输出]
         */
        bool checkStxoIsUtxo(Transaction *transaction);
//endregion

        /**
         * 补充区块的属性
         */
        void fillBlockProperty(Block *block);

        /**
         * 检验交易脚本，即校验交易输入能解锁交易输出吗？即用户花费的是自己的钱吗？
         * 校验用户花费的是自己的钱吗，用户只可以花费自己的钱。专业点的说法，校验UTXO所有权，用户只可以花费自己拥有的UTXO。
         * 用户如何能证明自己拥有这个UTXO，只要用户能创建出一个能解锁(该UTXO对应的交易输出脚本)的交易输入脚本，就证明了用户拥有该UTXO。
         * 这是因为锁(交易输出脚本)是用户创建的，自然只有该用户有对应的钥匙(交易输入脚本)，自然意味着有钥匙的用户拥有这把锁的所有权。
         */
        bool checkTransactionScript(Transaction *transaction);

        vector<Transaction> transactionDtos2Transactions(vector<TransactionDto> *transactionDtos) ;
        TransactionOutput transactionOutputDto2TransactionOutput(TransactionOutputDto *transactionOutputDto) ;
        TransactionType obtainTransactionDto(TransactionDto *transactionDto) ;
        OutputScript outputScriptDto2OutputScript(OutputScriptDto *outputScriptDto) ;
        InputScript inputScriptDto2InputScript(InputScriptDto *inputScriptDto) ;
    };

}


#endif //HELLOWORLD_BLOCKCHAIN_CPP_BLOCKCHAINDATABASE_H
