//
// Created by 40906 on 2021/9/28.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_NETCORECONFIGURATION_H
#define HELLOWORLD_BLOCKCHAIN_CPP_NETCORECONFIGURATION_H

#include <string>
#include <vector>
#include <cstdint>
using namespace std;

namespace netcoreconfiguration {


    class NetCoreConfiguration{
    private:
        string netCorePath;
    public:
        NetCoreConfiguration(string netCorePath);
    public:
        /**
         * BlockchainNetCore数据的存储路径
         */
        string getNetCorePath();


        /**
         * 种子节点初始化时间间隔
         */
        uint64_t getSeedNodeInitializeTimeInterval();
        /**
         * 节点搜索时间间隔
         */
        uint64_t getNodeSearchTimeInterval();
        /**
         * 节点广播时间间隔
         */
        uint64_t getNodeBroadcastTimeInterval();
        /**
         * 节点清理时间间隔
         */
        uint64_t getNodeCleanTimeInterval();


        /**
         * 是否"自动搜索新区块"
         */
        bool isAutoSearchBlock();
        /**
         * 开启"自动搜索新区块"选项
         */
        void activeAutoSearchBlock() ;
        /**
         * 关闭"自动搜索新区块"选项
         */
        void deactiveAutoSearchBlock() ;


        /**
         * 是否自动搜索节点
         */
        bool isAutoSearchNode();
        /**
         * 开启自动搜索节点
         */
        void activeAutoSearchNode();
        /**
         * 关闭自动搜索节点
         */
        void deactiveAutoSearchNode();


        /**
         * 区块搜索时间间隔
         */
        uint64_t getBlockSearchTimeInterval();
        /**
         * 区块广播时间间隔
         */
        uint64_t getBlockBroadcastTimeInterval();


        /**
         * 区块链高度搜索时间间隔
         */
        uint64_t getBlockchainHeightSearchTimeInterval();
        /**
         * 区块链高度广播时间间隔
         */
        uint64_t getBlockchainHeightBroadcastTimeInterval();


        /**
         * 硬分叉区块数量：两个区块链有分叉时，区块差异数量大于这个值，则真的分叉了。
         */
        uint64_t getHardForkBlockCount();

        /**
         * 未确认交易的搜索时间间隔
         */
        uint64_t getUnconfirmedTransactionsSearchTimeInterval();

    private:
        vector<unsigned char> getConfigurationValue(vector<unsigned char> configurationKey) ;
        void addOrUpdateConfiguration(vector<unsigned char> configurationKey, vector<unsigned char> configurationValue) ;
        string getNetCoreConfigurationDatabasePath();
    };
};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_NETCORECONFIGURATION_H
