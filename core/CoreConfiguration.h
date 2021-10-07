//
// Created by 40906 on 2021/9/20.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_CORECONFIGURATION_H
#define HELLOWORLD_BLOCKCHAIN_CPP_CORECONFIGURATION_H

#include <string>
#include <vector>
using namespace std;


namespace core{
    class CoreConfiguration {
    private:
        //BlockchainCore数据存放路径
        string corePath;
    public:
        CoreConfiguration(string corePath);
    public:
        /**
         * BlockchainCore数据存放路径
         */
        string getCorePath();
        /**
         * 矿工是否处于激活状态？
         */
        bool isMinerActive();
        /**
         * 激活矿工
         */
        void activeMiner() ;
        /**
         * 停用矿工
         */
        void deactiveMiner() ;
        /**
         * 设置矿工能挖掘的最高区块高度
         */
        void setMinerMineMaxBlockHeight(uint64_t maxHeight) ;

        /**
         * 获取矿工能挖掘的最高区块高度
         */
        uint64_t getMinerMineMaxBlockHeight() ;

        /**
         * 矿工挖矿时间周期
         * 矿工在组装好矿之后，系统会分配矿工一个确定的时间(这个时间就叫矿工挖矿时间周期)来让矿工进行挖矿，
         * 在一个挖矿周期内，矿工并不一定能挖到矿，一个周期过去之后，矿工会停一停，歇一歇，然后重新组装矿再继续去挖矿。
         * 为什么要重新组装矿？因为如果矿一直不变，在上次矿形成之后，用户新提交的未确认交易，就没有办法打包进矿了
         * ，交易会很延迟才能得到确认。
         */
        uint64_t getMinerMineTimeInterval();
    private:
        string getConfigurationDatabasePath();
        vector<unsigned char> getConfigurationValue(vector<unsigned char> configurationKey);
        void addOrUpdateConfiguration(vector<unsigned char> configurationKey, vector<unsigned char> configurationValue);
    };

}

#endif //HELLOWORLD_BLOCKCHAIN_CPP_CORECONFIGURATION_H
