//
// Created by XINGKAICHUN on 2021/9/30.
//

#include "NodeBroadcaster.h"
#include "../netcoreclient/NodeClient.h"
#include "../util/LogUtil.h"
#include "../util/ThreadUtil.h"

using namespace netcoreclient;





namespace netcore{


    NodeBroadcaster::NodeBroadcaster(NetCoreConfiguration *netCoreConfiguration, NodeService *nodeService){
        this->netCoreConfiguration = netCoreConfiguration;
        this->nodeService = nodeService;
    }
    void NodeBroadcaster::start(){
        try {
            while (true){
                broadcastNode();
                ThreadUtil::millisecondSleep(netCoreConfiguration->getNodeBroadcastTimeInterval());
            }
        } catch (exception e) {
            LogUtil::error("在区块链网络中广播自己出现异常",e);
        }
    }
    void NodeBroadcaster::broadcastNode(){
        vector<Node> nodes = nodeService->queryAllNodes();
        if(nodes.empty()){
            return;
        }

        for(Node node:nodes){
            NodeClient nodeClient(node.ip);
            PingRequest pingRequest;
            nodeClient.pingNode(pingRequest);
        }
    }

}