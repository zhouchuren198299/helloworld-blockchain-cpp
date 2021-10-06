//
// Created by 40906 on 2021/9/28.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_NODESERVICE_H
#define HELLOWORLD_BLOCKCHAIN_CPP_NODESERVICE_H


#include "../netcoremodel/netcoremodel.h"
#include "../netcoredao/NodeDao.h"
#include <vector>

using namespace netcoremodel;
using namespace netcoredao;


namespace service {


    class NodeService {
    private:
        NodeDao *nodeDao;
    public:
        NodeService(NodeDao *nodeDao);
    public:
        /**
         * 查询node
         */
        unique_ptr<Node> queryNode(string ip);
        /**
         * 获取所有节点
         */
        vector<Node> queryAllNodes();

        /**
         * 删除节点
         */
        void deleteNode(string ip);

        /**
         * 新增节点
         */
        void addNode(Node node);

        /**
         * 更新节点
         */
        void updateNode(Node node);

    private:
        vector<Node> NodeService::nodePo2Nodes(vector<NodePo> nodePos);
        Node NodeService::nodePo2Node(NodePo nodePo);
        NodePo NodeService::node2NodePo(Node node);
    };
};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_NODESERVICE_H
