//
// Created by XINGKAICHUN on 2021/9/30.
//

#include "SeedNodeInitializer.h"
#include "../util/LogUtil.h"
#include "../util/ThreadUtil.h"
#include "../setting/NetworkSetting.h"


namespace netcore{

    SeedNodeInitializer::SeedNodeInitializer(NetCoreConfiguration *netCoreConfiguration, NodeService *nodeService){
        this->netCoreConfiguration = netCoreConfiguration;
        this->nodeService = nodeService;
    }

    void SeedNodeInitializer::start(){
        try {
            while (true){
                addSeedNodes();
                ThreadUtil::millisecondSleep(netCoreConfiguration->getSeedNodeInitializeTimeInterval());
            }
        } catch (exception e) {
            LogUtil::error("定时将种子节点加入区块链网络出现异常",e);
        }
    }

    void SeedNodeInitializer::addSeedNodes(){
        if(!netCoreConfiguration->isAutoSearchNode()){
            return;
        }

        for(string seedNode: NetworkSetting::SEED_NODES){
            Node node{};
            node.ip=seedNode;
            node.blockchainHeight=0;
            nodeService->addNode(node);
            LogUtil::debug("种子节点初始化器提示您:种子节点["+node.ip+"]加入了区块链网络。");
        }
    }

}