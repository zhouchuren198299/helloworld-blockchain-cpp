//
// Created by XINGKAICHUN on 2021/9/30.
//

#include "BlockBroadcaster.h"
#include "../core/tool/Model2DtoTool.h"
#include "../netcoreclient/NodeClient.h"
#include "../util/LogUtil.h"
#include "../util/ThreadUtil.h"
#include "../util/NullUtil.h"

using namespace netcoreclient;

namespace netcore{

    BlockBroadcaster::BlockBroadcaster(NetCoreConfiguration *netCoreConfiguration, BlockchainCore *blockchainCore, NodeService *nodeService){
        this->netCoreConfiguration = netCoreConfiguration;
        this->blockchainCore = blockchainCore;
        this->nodeService = nodeService;
    }
    void BlockBroadcaster::start(){
        try {
            while (true){
                broadcastBlock();
                ThreadUtil::millisecondSleep(netCoreConfiguration->getBlockBroadcastTimeInterval());
            }
        } catch (exception e) {
            LogUtil::error("在区块链网络中广播自己的区块出现异常",e);
        }
    }
    void BlockBroadcaster::broadcastBlock(){
        vector<Node> nodes = nodeService->queryAllNodes();
        if(nodes.empty()){
            return;
        }

        for(Node node:nodes){
            Block block = blockchainCore->queryTailBlock();
            if(NullUtil::isNullBlock(block)){
                return;
            }
            if(block.height <= node.blockchainHeight){
                continue;
            }
            BlockDto blockDto = Model2DtoTool::block2BlockDto(&block);
            NodeClient nodeClient(node.ip);
            PostBlockRequest postBlockRequest;
            postBlockRequest.block=blockDto;
            nodeClient.postBlock(postBlockRequest);
        }
    }

}