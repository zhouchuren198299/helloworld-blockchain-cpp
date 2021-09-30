//
// Created by 40906 on 2021/9/28.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_NODEDAO_H
#define HELLOWORLD_BLOCKCHAIN_CPP_NODEDAO_H

#include <string>
#include <vector>
#include <cstdint>
#include "../netcorepo/netcorepo.h"
#include "../netcoreconfiguration/NetCoreConfiguration.h"

using namespace std;
using namespace netcorepo;
using namespace netcoreconfiguration;


namespace netcoredao {

    class NodeDao {
    private:
        NetCoreConfiguration *netCoreConfiguration;
    public:
        NodeDao(NetCoreConfiguration *netCoreConfiguration);
    public:
        /**
         * 查询节点
         */
        NodePo queryNode(string ip);
        /**
         * 查询所有节点
         */
        vector<NodePo> queryAllNodes();
        /**
         * 添加节点
         */
        void addNode(NodePo node);
        /**
         * 更新节点信息
         */
        void updateNode(NodePo node);
        /**
         * 删除节点
         */
        void deleteNode(string ip);

    private:
        string NodeDao::getNodeDatabasePath();
        vector<unsigned char> NodeDao::getKeyByNodePo(NodePo node);
        vector<unsigned char> NodeDao::getKeyByIp(string ip);
    };
};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_NODEDAO_H
