//
// Created by 40906 on 2021/9/28.
//

#include "NetCoreConfiguration.h"
#include "../../util/ByteUtil.h"
#include "../../util/FileUtil.h"
#include "../../util/KvDbUtil.h"

namespace netcoreconfiguration {

    string NETCORE_CONFIGURATION_DATABASE_NAME = "NetCoreConfigurationDatabase";

    //节点搜索器"是否是自动搜索新区块"状态存入到数据库时的主键
    string AUTO_SEARCH_BLOCK_OPTION_KEY = "IS_AUTO_SEARCH_BLOCK";
    //节点搜索器"是否是自动搜索新区块"开关的默认状态
    bool AUTO_SEARCH_BLOCK_OPTION_DEFAULT_VALUE = true;

    //节点搜索器'是否自动搜索节点'状态存入到数据库时的主键
    string AUTO_SEARCH_NODE_OPTION_KEY = "IS_AUTO_SEARCH_NODE";
    //节点搜索器'是否自动搜索节点'开关的默认状态
    bool AUTO_SEARCH_NODE_OPTION_DEFAULT_VALUE = true;

    //在区块链网络中自动搜寻新的节点的间隔时间
    uint64_t SEARCH_NODE_TIME_INTERVAL = 1000 * 60 * 2;
    //在区块链网络中自动搜索节点的区块链高度
    uint64_t SEARCH_BLOCKCHAIN_HEIGHT_TIME_INTERVAL = 1000 * 60 * 2;
    //在区块链网络中自动搜寻新的区块的间隔时间。
    uint64_t SEARCH_BLOCKS_TIME_INTERVAL = 1000 * 60 * 2;
    //区块高度广播时间间隔
    uint64_t BLOCKCHAIN_HEIGHT_BROADCASTER_TIME_INTERVAL = 1000 * 20;
    //区块广播时间间隔。
    uint64_t BLOCK_BROADCASTER_TIME_INTERVAL = 1000 * 20;
    //定时将种子节点加入本地区块链网络的时间间隔。
    uint64_t ADD_SEED_NODE_TIME_INTERVAL = 1000 * 60 * 2;
    //广播自己节点的时间间隔。
    uint64_t NODE_BROADCAST_TIME_INTERVAL = 1000 * 60 * 2;
    //清理死亡节点的时间间隔。
    uint64_t NODE_CLEAN_TIME_INTERVAL = 1000 * 60 * 10;


    //两个区块链有分叉时，区块差异数量大于这个值，则硬分叉了。
    uint64_t HARD_FORK_BLOCK_COUNT = 100000000;

    //在区块链网络中搜寻未确认交易的间隔时间。
    uint64_t SEARCH_UNCONFIRMED_TRANSACTIONS_TIME_INTERVAL = 1000 * 60 * 2;


    NetCoreConfiguration::NetCoreConfiguration(string netCorePath) {
        this->netCorePath = netCorePath;
    }
    
    string NetCoreConfiguration::getNetCorePath() {
        return netCorePath;
    }

    
    bool NetCoreConfiguration::isAutoSearchBlock() {
        vector<unsigned char> bytesConfigurationValue = getConfigurationValue(ByteUtil::stringToUtf8Bytes(AUTO_SEARCH_BLOCK_OPTION_KEY));
        if(bytesConfigurationValue.empty()){
            return AUTO_SEARCH_BLOCK_OPTION_DEFAULT_VALUE;
        }
        return ByteUtil::utf8BytesToBoolean(bytesConfigurationValue);
    }

    
    void NetCoreConfiguration::activeAutoSearchBlock() {
        addOrUpdateConfiguration(ByteUtil::stringToUtf8Bytes(AUTO_SEARCH_BLOCK_OPTION_KEY),ByteUtil::booleanToUtf8Bytes(true));
    }

    
    void NetCoreConfiguration::deactiveAutoSearchBlock() {
        addOrUpdateConfiguration(ByteUtil::stringToUtf8Bytes(AUTO_SEARCH_BLOCK_OPTION_KEY),ByteUtil::booleanToUtf8Bytes(false));
    }

    
    bool NetCoreConfiguration::isAutoSearchNode() {
        vector<unsigned char> bytesConfigurationValue = getConfigurationValue(ByteUtil::stringToUtf8Bytes(AUTO_SEARCH_NODE_OPTION_KEY));
        if(bytesConfigurationValue.empty()){
            return AUTO_SEARCH_NODE_OPTION_DEFAULT_VALUE;
        }
        return ByteUtil::utf8BytesToBoolean(bytesConfigurationValue);
    }

    
    void NetCoreConfiguration::activeAutoSearchNode() {
        addOrUpdateConfiguration(ByteUtil::stringToUtf8Bytes(AUTO_SEARCH_NODE_OPTION_KEY),ByteUtil::booleanToUtf8Bytes(true));
    }

    
    void NetCoreConfiguration::deactiveAutoSearchNode() {
        addOrUpdateConfiguration(ByteUtil::stringToUtf8Bytes(AUTO_SEARCH_NODE_OPTION_KEY),ByteUtil::booleanToUtf8Bytes(false));
    }

    
    uint64_t NetCoreConfiguration::getNodeSearchTimeInterval() {
        return SEARCH_NODE_TIME_INTERVAL;
    }

    
    uint64_t NetCoreConfiguration::getBlockchainHeightSearchTimeInterval() {
        return SEARCH_BLOCKCHAIN_HEIGHT_TIME_INTERVAL;
    }

    
    uint64_t NetCoreConfiguration::getBlockSearchTimeInterval() {
        return SEARCH_BLOCKS_TIME_INTERVAL;
    }

    
    uint64_t NetCoreConfiguration::getBlockchainHeightBroadcastTimeInterval() {
        return BLOCKCHAIN_HEIGHT_BROADCASTER_TIME_INTERVAL;
    }

    
    uint64_t NetCoreConfiguration::getBlockBroadcastTimeInterval() {
        return BLOCK_BROADCASTER_TIME_INTERVAL;
    }

    
    uint64_t NetCoreConfiguration::getSeedNodeInitializeTimeInterval() {
        return ADD_SEED_NODE_TIME_INTERVAL;
    }

    
    uint64_t NetCoreConfiguration::getNodeBroadcastTimeInterval() {
        return NODE_BROADCAST_TIME_INTERVAL;
    }

    
    uint64_t NetCoreConfiguration::getHardForkBlockCount() {
        return HARD_FORK_BLOCK_COUNT;
    }

    
    uint64_t NetCoreConfiguration::getUnconfirmedTransactionsSearchTimeInterval() {
        return SEARCH_UNCONFIRMED_TRANSACTIONS_TIME_INTERVAL;
    }


    uint64_t NetCoreConfiguration::getNodeCleanTimeInterval() {
        return NODE_CLEAN_TIME_INTERVAL;
    }



    vector<unsigned char> NetCoreConfiguration::getConfigurationValue(vector<unsigned char> configurationKey) {
        vector<unsigned char> bytesConfigurationValue = KvDbUtil::get(getNetCoreConfigurationDatabasePath(), configurationKey);
        return bytesConfigurationValue;
    }

    void NetCoreConfiguration::addOrUpdateConfiguration(vector<unsigned char> configurationKey, vector<unsigned char> configurationValue) {
        KvDbUtil::put(getNetCoreConfigurationDatabasePath(), configurationKey, configurationValue);
    }

    string NetCoreConfiguration::getNetCoreConfigurationDatabasePath(){
        return FileUtil::newPath(netCorePath, NETCORE_CONFIGURATION_DATABASE_NAME);
    }
}