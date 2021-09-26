//
// Created by 40906 on 2021/9/20.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_RESOURCEPATHTOOL_H
#define HELLOWORLD_BLOCKCHAIN_CPP_RESOURCEPATHTOOL_H

#include <string>
using namespace std;

namespace ResourcePathTool {
    /**
     * 获取区块链数据存放目录
     */
    string getDataRootPath() ;

    /**
     * 获取测试区块链数据存放目录
     */
    string getTestDataRootPath() ;
};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_RESOURCEPATHTOOL_H
