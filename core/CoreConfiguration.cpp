//
// Created by 40906 on 2021/9/20.
//

#include "CoreConfiguration.h"
#include "../util/FileUtil.h"
#include "../util/ByteUtil.h"
#include "../util/KvDbUtil.h"

namespace core{

    //配置数据库名字
    const string  CONFIGURATION_DATABASE_NAME = "ConfigurationDatabase";

    //'矿工是否是激活状态'存入到数据库时的主键
    const string MINE_OPTION_KEY = "IS_MINER_ACTIVE";

    //'矿工可挖的最高区块高度'存入到数据库时的主键
    const string MINER_MINE_MAX_BLOCK_HEIGHT_KEY = "MINER_MINE_MAX_BLOCK_HEIGHT";



    //'矿工是否是激活状态'的默认值
    const bool MINE_OPTION_DEFAULT_VALUE = false;

    //这个时间间隔更新一次正在被挖矿的区块的交易。如果时间太长，可能导致新提交的交易延迟被确认。
    const uint64_t MINE_TIMESTAMP_PER_ROUND = 1000 * 10;




    string CoreConfiguration::getCorePath() {
        return this->corePath;
    }


    bool CoreConfiguration::isMinerActive() {
        vector<unsigned char> mineOption = getConfigurationValue(ByteUtil::stringToUtf8Bytes(MINE_OPTION_KEY));
        if(mineOption.empty()){
            return MINE_OPTION_DEFAULT_VALUE;
        }
        return ByteUtil::utf8BytesToBoolean(mineOption);
    }


    void CoreConfiguration::activeMiner() {
        addOrUpdateConfiguration(ByteUtil::stringToUtf8Bytes(MINE_OPTION_KEY),ByteUtil::booleanToUtf8Bytes(true));
    }


    void CoreConfiguration::deactiveMiner() {
        addOrUpdateConfiguration(ByteUtil::stringToUtf8Bytes(MINE_OPTION_KEY),ByteUtil::booleanToUtf8Bytes(false));
    }

    void CoreConfiguration::setMinerMineMaxBlockHeight(uint64_t maxHeight) {
        addOrUpdateConfiguration(ByteUtil::stringToUtf8Bytes(MINER_MINE_MAX_BLOCK_HEIGHT_KEY),ByteUtil::uint64ToBytes(maxHeight));
    }

    uint64_t CoreConfiguration::getMinerMineMaxBlockHeight() {
        vector<unsigned char> bytesMineMaxBlockHeight = getConfigurationValue(ByteUtil::stringToUtf8Bytes(MINER_MINE_MAX_BLOCK_HEIGHT_KEY));
        if(bytesMineMaxBlockHeight.empty()){
            //设置默认值，这是一个十分巨大的数字，矿工永远挖不到的高度
            return 10000000000000000L;
        }
        return ByteUtil::bytesToUint64(bytesMineMaxBlockHeight);
    }

    uint64_t CoreConfiguration::getMinerMineTimeInterval() {
        return MINE_TIMESTAMP_PER_ROUND;
    }







    string CoreConfiguration::getConfigurationDatabasePath(){
        return FileUtil::newPath(corePath, CONFIGURATION_DATABASE_NAME);
    }
    vector<unsigned char> CoreConfiguration::getConfigurationValue(vector<unsigned char> configurationKey) {
        return KvDbUtil::get(getConfigurationDatabasePath(), configurationKey);
    }
    void CoreConfiguration::addOrUpdateConfiguration(vector<unsigned char> configurationKey, vector<unsigned char> configurationValue) {
        KvDbUtil::put(getConfigurationDatabasePath(), configurationKey, configurationValue);
    }

    CoreConfiguration::CoreConfiguration(string corePath) {
        this->corePath = corePath;
    }
}