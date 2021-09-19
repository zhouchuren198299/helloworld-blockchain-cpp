//
// Created by 40906 on 2021/9/18.
//

#include "ScriptDtoTool.h"
#include "../../crypto/ByteUtil.h"
#include "../model/model.h"

vector<unsigned char> ScriptDtoTool::bytesInputScript(dto::InputScriptDto inputScript){
    return bytesScript(inputScript);
}
vector<unsigned char> ScriptDtoTool::bytesOutputScript(dto::OutputScriptDto outputScript){
    return bytesScript(outputScript);
}
vector<unsigned char> ScriptDtoTool::bytesScript(dto::ScriptDto script) {
    vector<unsigned char> bytesScript;
    for(int i=0;i<script.size();i++){
        string operationCode = script[i];
        vector<unsigned char> bytesOperationCode = ByteUtil::hexStringToBytes(operationCode);
        if(ByteUtil::isEquals(model::OperationCodeEnum::OP_DUP.code,bytesOperationCode) ||
           ByteUtil::isEquals(model::OperationCodeEnum::OP_HASH160.code,bytesOperationCode) ||
           ByteUtil::isEquals(model::OperationCodeEnum::OP_EQUALVERIFY.code,bytesOperationCode) ||
           ByteUtil::isEquals(model::OperationCodeEnum::OP_CHECKSIG.code,bytesOperationCode)){
            bytesScript = ByteUtil::concatenate(bytesScript, ByteUtil::concatenateLength(bytesOperationCode));
        }else if(ByteUtil::isEquals(model::OperationCodeEnum::OP_PUSHDATA.code,bytesOperationCode)){
            string operationData = script[++i];
            vector<unsigned char> bytesOperationData = ByteUtil::hexStringToBytes(operationData);
            bytesScript = ByteUtil::concatenate3(bytesScript, ByteUtil::concatenateLength(bytesOperationCode), ByteUtil::concatenateLength(bytesOperationData));
        }else {
            throw new exception("不能识别的指令");
        }
    }
    return bytesScript;
}