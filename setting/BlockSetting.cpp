//
// Created by 40906 on 2021/9/20.
//

#include "BlockSetting.h"
#include <cstdint>

namespace BlockSetting {
    //区块最多含有的交易数量
    uint64_t BLOCK_MAX_TRANSACTION_COUNT = 600;
    //区块的最大字符数量：用于限制区块的大小
    uint64_t BLOCK_MAX_CHARACTER_COUNT = 1024 * 1024;
    //随机数的字符数量：用于限制随机数的大小只能是64个字符。
    uint64_t NONCE_CHARACTER_COUNT = 64;
};
