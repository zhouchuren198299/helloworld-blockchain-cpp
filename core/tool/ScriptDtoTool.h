//
// Created by 40906 on 2021/9/18.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_SCRIPTDTOTOOL_H
#define HELLOWORLD_BLOCKCHAIN_CPP_SCRIPTDTOTOOL_H


#include "../../dto/dto.h"

using namespace dto;

namespace ScriptDtoTool {
    vector<unsigned char> bytesInputScript(dto::InputScriptDto inputScript);
    vector<unsigned char> bytesOutputScript(dto::OutputScriptDto outputScript);
    vector<unsigned char> bytesScript(dto::ScriptDto script);

    /**
     * 脚本：将字节型脚本反序列化为脚本.
     */
    InputScriptDto inputScriptDto(vector<unsigned char> *bytesScript) ;
    /**
     * 脚本：将字节型脚本反序列化为脚本.
     */
    OutputScriptDto outputScriptDto(vector<unsigned char> *bytesScript) ;
    /**
     * 脚本：将字节型脚本反序列化为脚本.
     */
    vector<string> script(vector<unsigned char> *bytesScript) ;
    //endregion

    /**
     * 是否是P2PKH输入脚本
     */
    bool isPayToPublicKeyHashInputScript(InputScriptDto inputScriptDto) ;
    /**
     * 是否是P2PKH输出脚本
     */
    bool isPayToPublicKeyHashOutputScript(OutputScriptDto outputScriptDto) ;

    /**
     * 获取P2PKH输出脚本中的公钥哈希
     */
    string getPublicKeyHashFromPayToPublicKeyHashOutputScript(OutputScriptDto outputScript) ;

    /**
     * 创建P2PKH输出脚本
     */
    OutputScriptDto createPayToPublicKeyHashOutputScript(string address) ;
    /**
     * 创建P2PKH输入脚本
     */
    InputScriptDto createPayToPublicKeyHashInputScript(string sign, string publicKey) ;
};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_SCRIPTDTOTOOL_H
