//
// Created by 40906 on 2021/9/28.
//

#include "NodeService.h"
#include "../../util/NullUtil.h"


namespace service{

    NodeService::NodeService(NodeDao *nodeDao){
        this->nodeDao = nodeDao;
    }

    void NodeService::deleteNode(string ip){
        nodeDao->deleteNode(ip);
    }

    vector<Node> NodeService::queryAllNodes(){
        vector<NodePo> nodePos = nodeDao->queryAllNodes();
        return nodePo2Nodes(nodePos);
    }

    void NodeService::addNode(Node node){
        if(!NullUtil::isNullNodePo(nodeDao->queryNode(node.ip))){
            return;
        }
        NodePo nodePo = node2NodePo(node);
        nodeDao->addNode(nodePo);
    }

    
    void NodeService::updateNode(Node node){
        NodePo nodePo = nodeDao->queryNode(node.ip);
        if(NullUtil::isNullNodePo(nodePo)){
            return;
        }
        nodePo = node2NodePo(node);
        nodeDao->updateNode(nodePo);
    }

    
    Node NodeService::queryNode(string ip){
        NodePo nodePo = nodeDao->queryNode(ip);
        if(NullUtil::isNullNodePo(nodePo)){
           return NullUtil::newNullNode();
        }
        return nodePo2Node(nodePo);
    }

    vector<Node> NodeService::nodePo2Nodes(vector<NodePo> nodePos){
        vector<Node> nodes;
        if(!nodePos.empty()){
            for(NodePo &nodePo : nodePos){
                Node node = nodePo2Node(nodePo);
                nodes.push_back(node);
            }
        }
        return nodes;
    }
    Node NodeService::nodePo2Node(NodePo nodePo){
        Node node;
        node.ip=nodePo.ip;
        node.blockchainHeight=nodePo.blockchainHeight;
        return node;
    }
    NodePo NodeService::node2NodePo(Node node){
        NodePo nodePo;
        nodePo.ip=node.ip;
        nodePo.blockchainHeight=node.blockchainHeight;
        return nodePo;
    }
}