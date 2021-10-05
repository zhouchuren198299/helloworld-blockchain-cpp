//
// Created by 40906 on 2021/10/4.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_NODECONSOLEAPPLICATIONAPI_H
#define HELLOWORLD_BLOCKCHAIN_CPP_NODECONSOLEAPPLICATIONAPI_H

#include <string>
using namespace std;



namespace NodeConsoleApplicationApi {


    //矿工是否激活
    const string IS_MINER_ACTIVE = "/Api/NodeConsoleApplication/IsMinerActive";

//激活矿工
    const string ACTIVE_MINER = "/Api/NodeConsoleApplication/ActiveMiner";

//停用矿工
    const string DEACTIVE_MINER = "/Api/NodeConsoleApplication/DeactiveMiner";

//是否"自动搜索新区块"
    const string IS_AUTO_SEARCH_BLOCK = "/Api/NodeConsoleApplication/IsAutoSearchBlock";

//开启"自动搜索新区块"选项
    const string ACTIVE_AUTO_SEARCH_BLOCK = "/Api/NodeConsoleApplicationle/ActiveAutoSearchBlock";

//关闭"自动搜索新区块"选项
    const string DEACTIVE_AUTO_SEARCH_BLOCK = "/Api/NodeConsoleApplication/DeactiveAutoSearchBlock";

//是否开启了自动寻找区块链节点的功能
    const string IS_AUTO_SEARCH_NODE = "/Api/NodeConsoleApplication/IsAutoSearchNode";

//开启"自动搜索节点"选项
    const string ACTIVE_AUTO_SEARCH_NODE = "/Api/NodeConsoleApplication/ActiveAutoSearchNode";

//关闭"自动搜索节点"选项
    const string DEACTIVE_AUTO_SEARCH_NODE = "/Api/NodeConsoleApplication/DeactiveAutoSearchNode";

//设置最大挖矿高度
    const string SET_MINER_MINE_MAX_BLOCK_HEIGHT = "/Api/NodeConsoleApplication/SetMinerMineMaxBlockHeight";

//获取最大挖矿高度
    const string GET_MINER_MINE_MAX_BLOCK_HEIGHT = "/Api/NodeConsoleApplication/GetMinerMineMaxBlockHeight";

//新增节点
    const string ADD_NODE = "/Api/NodeConsoleApplication/AddNode";

//更新节点信息
    const string UPDATE_NODE = "/Api/NodeConsoleApplication/UpdateNode";

//删除节点
    const string DELETE_NODE = "/Api/NodeConsoleApplication/DeleteNode";

//查询所有节点
    const string QUERY_ALL_NODES = "/Api/NodeConsoleApplication/QueryAllNodes";

//删除区块
    const string DELETE_BLOCKS = "/Api/NodeConsoleApplication/DeleteBlocks";
};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_NODECONSOLEAPPLICATIONAPI_H
