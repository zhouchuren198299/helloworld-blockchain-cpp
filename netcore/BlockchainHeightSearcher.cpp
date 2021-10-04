//
// Created by XINGKAICHUN on 2021/9/30.
//

#include "BlockchainHeightSearcher.h"
#include "../netcoreclient/NodeClient.h"
#include "../util/LogUtil.h"
#include "../util/ThreadUtil.h"
#include "../util/NullUtil.h"

using namespace netcoreclient;


namespace netcore{


    BlockchainHeightSearcher::BlockchainHeightSearcher(NetCoreConfiguration *netCoreConfiguration,NodeService *nodeService){
        this->netCoreConfiguration = netCoreConfiguration;
        this->nodeService = nodeService;
    }

    void BlockchainHeightSearcher::start(){
        try {
            while (true){
                searchBlockchainHeight();
                ThreadUtil::millisecondSleep(netCoreConfiguration->getBlockchainHeightSearchTimeInterval());
            }
        } catch (exception e) {
            LogUtil::error("在区块链网络中搜索节点的高度异常",e);
        }
    }

    void BlockchainHeightSearcher::searchBlockchainHeight(){
        vector<Node> nodes = nodeService->queryAllNodes();
        if(nodes.empty()){
            return;
        }

        for(Node node:nodes){
            NodeClient nodeClient(node.ip);
            GetBlockchainHeightRequest getBlockchainHeightRequest;
            unique_ptr<GetBlockchainHeightResponse> getBlockchainHeightResponse = nodeClient.getBlockchainHeight(getBlockchainHeightRequest);
            if(getBlockchainHeightResponse.get()){
                node.blockchainHeight=getBlockchainHeightResponse->blockchainHeight;
                nodeService->updateNode(node);
            }
        }
    }

}