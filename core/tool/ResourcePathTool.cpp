//
// Created by 40906 on 2021/9/20.
//

#include "ResourcePathTool.h"
#include "../../util/OperateSystemUtil.h"
#include "../../util/FileUtil.h"

namespace ResourcePathTool{

    /**
     * 获取区块链数据存放目录
     */
    string getDataRootPath() {
        string dataRootPath;
        if(OperateSystemUtil::isWindowsOperateSystem()){
            dataRootPath = "C:\\helloworld-blockchain-cpp\\";
        }else if(OperateSystemUtil::isMacOperateSystem()){
            dataRootPath = "/tmp/helloworld-blockchain-cpp/";
        }else if(OperateSystemUtil::isLinuxOperateSystem()){
            dataRootPath = "/tmp/helloworld-blockchain-cpp/";
        }else{
            dataRootPath = "/tmp/helloworld-blockchain-cpp/";
        }
        FileUtil::makeDirectory(dataRootPath);
        return dataRootPath;
    }

    /**
     * 获取测试区块链数据存放目录
     */
    string getTestDataRootPath() {
        string dataRootPath;
        if(OperateSystemUtil::isWindowsOperateSystem()){
            dataRootPath = "C:\\helloworld-blockchain-cpp-test\\";
        }else if(OperateSystemUtil::isMacOperateSystem()){
            dataRootPath = "/tmp/helloworld-blockchain-cpp-test/";
        }else if(OperateSystemUtil::isLinuxOperateSystem()){
            dataRootPath = "/tmp/helloworld-blockchain-cpp-test/";
        }else{
            dataRootPath = "/tmp/helloworld-blockchain-cpp-test/";
        }
        FileUtil::makeDirectory(dataRootPath);
        return dataRootPath;
    }
}