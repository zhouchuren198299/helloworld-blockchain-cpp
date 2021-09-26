//
// Created by 40906 on 2021/9/20.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_SCRIPTTOOL_H
#define HELLOWORLD_BLOCKCHAIN_CPP_SCRIPTTOOL_H


#include <string>
#include "../model/model.h"

using namespace model;
using namespace std;

namespace ScriptTool {
//region 可视、可阅读的脚本，区块链浏览器使用
    string stringInputScript(InputScript inputScript) ;
    string stringOutputScript(OutputScript outputScript) ;
    string stringScript(Script script) ;
    //endregion

    /**
     * 构建完整脚本
     */
    Script createScript(InputScript inputScript, OutputScript outputScript) ;

    /**
     * 创建P2PKH输入脚本
     */
    InputScript createPayToPublicKeyHashInputScript(string sign, string publicKey) ;

    /**
     * 创建P2PKH输出脚本
     */
    OutputScript createPayToPublicKeyHashOutputScript(string address) ;

    /**
     * 是否是P2PKH输入脚本
     */
    bool isPayToPublicKeyHashInputScript(InputScript *inputScript) ;

    /**
     * 是否是P2PKH输出脚本
     */
    bool isPayToPublicKeyHashOutputScript(OutputScript *outputScript) ;
};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_SCRIPTTOOL_H
