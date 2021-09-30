//
// Created by XINGKAICHUN on 2021/9/30.
//

#include "NodeSearcher.h"
#include "../netcoreclient/NodeClient.h"
#include "../util/LogUtil.h"
#include "../util/ThreadUtil.h"

using namespace netcoreclient;


namespace netcore{


    NodeSearcher::NodeSearcher(NetCoreConfiguration *netCoreConfiguration, NodeService *nodeService){
        this->netCoreConfiguration = netCoreConfiguration;
        this->nodeService = nodeService;
    }
    
    void NodeSearcher::start(){
        try {
            while (true){
                searchNodes();
                ThreadUtil::millisecondSleep(netCoreConfiguration->getNodeSearchTimeInterval());
            }
        } catch (exception e) {
            LogUtil::error("在区块链网络中搜索新的节点出现异常",e);
        }
    }
    
    void NodeSearcher::searchNodes(){
        if(!netCoreConfiguration->isAutoSearchNode()){
            return;
        }
        vector<Node> nodes = nodeService->queryAllNodes();
        if(nodes.empty()){
            return;
        }

        for(Node node:nodes){
            if(!netCoreConfiguration->isAutoSearchNode()){
                return;
            }
            NodeClient nodeClient(node.ip);
            GetNodesRequest getNodesRequest;
            GetNodesResponse getNodesResponse = nodeClient.getNodes(getNodesRequest);
            handleGetNodesResponse(getNodesResponse);
        }
    }
    
    void NodeSearcher::handleGetNodesResponse(GetNodesResponse getNodesResponse){
/*TODO        if(getNodesResponse == null){
            return;
        }*/
        vector<NodeDto> nodes = getNodesResponse.nodes;
        if(nodes.empty()){
            return;
        }

        for(NodeDto node:nodes){
            if(!netCoreConfiguration->isAutoSearchNode()){
                return;
            }
            NodeClient nodeClient(node.ip);
            PingRequest pingRequest;
            PingResponse pingResponse = nodeClient.pingNode(pingRequest);
/*TODO            if(pingResponse != null){
                Node n{};
                n.ip=node.ip;
                n.blockchainHeight=0;
                nodeService->addNode(n);
                LogUtil::debug("自动机制发现节点["+node.ip+"]，已在节点数据库中添加了该节点。");
            }*/
        }
    }

}