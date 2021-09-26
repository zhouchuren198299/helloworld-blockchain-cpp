//
// Created by 40906 on 2021/9/20.
//

#include "ScriptTool.h"
#include "Model2DtoTool.h"
#include "ScriptDtoTool.h"
#include "../../util/ByteUtil.h"
#include "../../crypto/AccountUtil.h"
#include "../../util/StringUtil.h"

using namespace model;

namespace ScriptTool{
//region 可视、可阅读的脚本，区块链浏览器使用
    string stringInputScript(InputScript inputScript) {
        return stringScript(inputScript);
    }
    string stringOutputScript(OutputScript outputScript) {
        return stringScript(outputScript);
    }
    string stringScript(Script script) {
        string stringScript = "";
        for(int i=0;i<script.size();i++){
            string operationCode = script[i];
            vector<unsigned char> bytesOperationCode = ByteUtil::hexStringToBytes(operationCode);
            if(ByteUtil::equals(OperationCodeEnum::OP_DUP.code,bytesOperationCode)){
                stringScript = StringUtil::concatenate3(stringScript,OperationCodeEnum::OP_DUP.name,StringUtil::BLANKSPACE);
            }else if(ByteUtil::equals(OperationCodeEnum::OP_HASH160.code,bytesOperationCode)){
                stringScript = StringUtil::concatenate3(stringScript,OperationCodeEnum::OP_HASH160.name,StringUtil::BLANKSPACE);
            }else if(ByteUtil::equals(OperationCodeEnum::OP_EQUALVERIFY.code,bytesOperationCode)){
                stringScript = StringUtil::concatenate3(stringScript,OperationCodeEnum::OP_EQUALVERIFY.name,StringUtil::BLANKSPACE);
            }else if(ByteUtil::equals(OperationCodeEnum::OP_CHECKSIG.code,bytesOperationCode)){
                stringScript = StringUtil::concatenate3(stringScript,OperationCodeEnum::OP_CHECKSIG.name,StringUtil::BLANKSPACE);
            }else if(ByteUtil::equals(OperationCodeEnum::OP_PUSHDATA.code,bytesOperationCode)){
                string operationData = script[++i];
                stringScript = StringUtil::concatenate3(stringScript,OperationCodeEnum::OP_PUSHDATA.name,StringUtil::BLANKSPACE);
                stringScript = StringUtil::concatenate3(stringScript,operationData,StringUtil::BLANKSPACE);
            }else {
                throw new exception("不能识别的指令");
            }
        }
        return stringScript;
    }
    //endregion

    /**
     * 构建完整脚本
     */
    Script createScript(InputScript inputScript, OutputScript outputScript) {
        Script script;
        for (string s:inputScript) {
            script.push_back(s);
        }
        for (string s:outputScript) {
            script.push_back(s);
        }
        return script;
    }

    /**
     * 创建P2PKH输入脚本
     */
    InputScript createPayToPublicKeyHashInputScript(string sign, string publicKey) {
        InputScript script;
        script.push_back(ByteUtil::bytesToHexString(OperationCodeEnum::OP_PUSHDATA.code));
        script.push_back(sign);
        script.push_back(ByteUtil::bytesToHexString(OperationCodeEnum::OP_PUSHDATA.code));
        script.push_back(publicKey);
        return script;
    }

    /**
     * 创建P2PKH输出脚本
     */
    OutputScript createPayToPublicKeyHashOutputScript(string address) {
        OutputScript script;
        script.push_back(ByteUtil::bytesToHexString(OperationCodeEnum::OP_DUP.code));
        script.push_back(ByteUtil::bytesToHexString(OperationCodeEnum::OP_HASH160.code));
        script.push_back(ByteUtil::bytesToHexString(OperationCodeEnum::OP_PUSHDATA.code));
        string publicKeyHash = AccountUtil::publicKeyHashFromAddress(address);
        script.push_back(publicKeyHash);
        script.push_back(ByteUtil::bytesToHexString(OperationCodeEnum::OP_EQUALVERIFY.code));
        script.push_back(ByteUtil::bytesToHexString(OperationCodeEnum::OP_CHECKSIG.code));
        return script;
    }

    /**
     * 是否是P2PKH输入脚本
     */
    bool isPayToPublicKeyHashInputScript(InputScript *inputScript) {
        InputScriptDto inputScriptDto = Model2DtoTool::inputScript2InputScriptDto(inputScript);
        return ScriptDtoTool::isPayToPublicKeyHashInputScript(inputScriptDto);
    }

    /**
     * 是否是P2PKH输出脚本
     */
    bool isPayToPublicKeyHashOutputScript(OutputScript *outputScript) {
        OutputScriptDto outputScriptDto = Model2DtoTool::outputScript2OutputScriptDto(outputScript);
        return ScriptDtoTool::isPayToPublicKeyHashOutputScript(outputScriptDto);
    }
}