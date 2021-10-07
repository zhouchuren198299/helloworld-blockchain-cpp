//
// Created by 40906 on 2021/9/20.
//

#include "Incentive.h"
#include "tool/BlockTool.h"
#include "../util/LogUtil.h"
#include "../setting/IncentiveSetting.h"

namespace core{

    uint64_t getMinerSubsidy(Block *block);

    /**
     *
     * 矿工挖矿成功后，奖励给矿工的激励金额。
     */
    uint64_t Incentive::incentiveValue(BlockchainDatabase *blockchainDatabase, Block *block) {
        //给予矿工的挖矿津贴
        uint64_t minerSubsidy = getMinerSubsidy(block);
        //给予矿工的交易手续费
        uint64_t minerFee = BlockTool::getBlockFee(block);
        //总的激励
        return minerSubsidy + minerFee;
    }

    /**
     * 校验激励。
     */
    bool Incentive::checkIncentive(BlockchainDatabase *blockchainDatabase, Block *block) {
        uint64_t writeIncentiveValue = BlockTool::getWritedIncentiveValue(block);
        uint64_t targetIncentiveValue = incentiveValue(blockchainDatabase,block);
        if(writeIncentiveValue != targetIncentiveValue){
            LogUtil::debug("区块数据异常，挖矿奖励数据异常。");
            return false;
        }
        return true;
    }

    uint64_t getMinerSubsidy(Block *block) {
        uint64_t initCoin = IncentiveSetting::BLOCK_INIT_INCENTIVE;
        uint64_t multiple = (block->height - 1L) / IncentiveSetting::INCENTIVE_HALVING_INTERVAL;
        while (multiple > 0){
            initCoin = initCoin / 2L;
            --multiple;
        }
        return initCoin;
    }
}