//
// Created by XINGKAICHUN on 2021/9/30.
//

#include "NodeCleaner.h"
#include "../netcoreclient/NodeClient.h"
#include "../util/LogUtil.h"
#include "../util/ThreadUtil.h"

using namespace netcoreclient;


namespace netcore{


    NodeCleaner::NodeCleaner(NetCoreConfiguration *netCoreConfiguration, NodeService *nodeService){
        this->netCoreConfiguration = netCoreConfiguration;
        this->nodeService = nodeService;
    }

    void NodeCleaner::start(){
        try {
            while (true){
                cleanDeadNodes();
                ThreadUtil::millisecondSleep(netCoreConfiguration->getNodeCleanTimeInterval());
            }
        } catch (exception e) {
            LogUtil::error("清理死亡节点出现异常",e);
        }
    }

    void NodeCleaner::cleanDeadNodes(){
        vector<Node> nodes = nodeService->queryAllNodes();
        if(nodes.empty()){
            return;
        }

        for(Node node:nodes){
            NodeClient nodeClient(node.ip);
            PingRequest pingRequest;
            unique_ptr<PingResponse> pingResponse = nodeClient.pingNode(pingRequest);
            if(!pingResponse.get()){
                nodeService->deleteNode(node.ip);
                LogUtil::debug("节点清理器发现死亡节点["+node.ip+"]，已在节点数据库中将该节点删除了。");
            }
        }
    }

}
