//
// Created by 40906 on 2021/9/20.
//

#include "Consensus.h"
#include "../util/StringUtil.h"
#include "tool/BlockTool.h"
#include "../setting/IncentiveSetting.h"
#include "../setting/GenesisBlockSetting.h"
#include "../util/ByteUtil.h"

namespace core{


    struct BigInteger{

    };

    /**
     * 校验区块是否满足共识
     * 如果区块满足共识的要求，这个区块就可能(为什么说是可能呢？因为还要进一步校验区块的结构、大小等信息)是一个合格的区块
     * ，如果进一步校验通过的话，那么这个区块就被允许添加进区块链了。
     * 如果区块不满足共识的要求，那么这个区块一定是一个非法的区块，非法的区块一定不能被添加进区块链。
     */
    bool Consensus::
    checkConsensus(BlockchainDatabase *blockchainDatabase, Block *block) {
        string difficulty = block->difficulty;
        if(StringUtil::isEmpty(difficulty)){
            difficulty = calculateDifficult(blockchainDatabase,block);
        }

        string hash = block->hash;
        if(StringUtil::isEmpty(hash)){
            hash = BlockTool::calculateBlockHash(block);
        }
        return ByteUtil::greatThan(difficulty,hash);
    }

    /**
     * 计算目标区块的挖矿难度
     */
    string Consensus::calculateDifficult(BlockchainDatabase *blockchainDatabase, Block *targetBlock) {
        // 目标难度
        string targetDifficult;
        // 目标区块高度
        uint64_t targetBlockHeight = targetBlock->height;
        // 目标区块高度在第一周期、第二周期内
        if(targetBlockHeight <= IncentiveSetting::INTERVAL_BLOCK_COUNT * 2){
            targetDifficult = GenesisBlockSetting::DIFFICULTY;
            return targetDifficult;
        }

        // 目标区块的上一个区块
        unique_ptr<Block> targetBlockPreviousBlock = blockchainDatabase->queryBlockByBlockHeight(targetBlockHeight-1);
        /*
         * 目标区块的上一个区块如果不是一个周期的末尾，说明一个周期尚未结束
         * ，说明目标区块和[目标区块的上一个区块]位于同一个周期，此时目标区块难度和[目标区块的上一个区块]的难度相同。
         */
        if (targetBlockPreviousBlock->height % IncentiveSetting::INTERVAL_BLOCK_COUNT != 0){
            targetDifficult = targetBlockPreviousBlock->difficulty;
            return targetDifficult;
        }

        // *** 计算新周期难度值 ***
        // 上个周期的最后一个区块，此时，targetBlockPreviousBlock是上一个周期的最后一个区块，这里仅仅是重新命名了一个更为准确的变量名称。
        unique_ptr<Block> &previousIntervalLastBlock = targetBlockPreviousBlock;
        // 上上个周期最后一个区块
        unique_ptr<Block> previousPreviousIntervalLastBlock = blockchainDatabase->queryBlockByBlockHeight(previousIntervalLastBlock->height- IncentiveSetting::INTERVAL_BLOCK_COUNT);
        // 上个周期出块实际耗时
        uint64_t previousIntervalActualTimespan = previousIntervalLastBlock->timestamp - previousPreviousIntervalLastBlock->timestamp;


        string hexPreviousIntervalActualTimespan = ByteUtil::bytesToHexString(ByteUtil::uint64ToBytes(previousIntervalActualTimespan));
        string hexIntervalTime = ByteUtil::bytesToHexString(ByteUtil::uint64ToBytes(IncentiveSetting::INTERVAL_TIME));
        return ByteUtil::divide(ByteUtil::multiply(previousIntervalLastBlock->difficulty,hexPreviousIntervalActualTimespan),hexIntervalTime);
    }
}