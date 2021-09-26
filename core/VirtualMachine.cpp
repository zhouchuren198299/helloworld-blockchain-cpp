//
// Created by 40906 on 2021/9/20.
//

#include "VirtualMachine.h"
#include "../util/ByteUtil.h"
#include "../util/StringUtil.h"
#include "tool/TransactionTool.h"
#include "../crypto/AccountUtil.h"

namespace core{

    ScriptExecuteResult VirtualMachine::executeScript(Transaction transactionEnvironment, Script script){
        ScriptExecuteResult stack;

        for(int i=0;i<script.size();i++){
            string operationCode = script[i];
            vector<unsigned char> bytesOperationCode = ByteUtil::hexStringToBytes(operationCode);
            if(ByteUtil::equals(OperationCodeEnum::OP_DUP.code,bytesOperationCode)){
                if(stack.size()<1){
                    throw new exception("指令运行异常");
                }
                stack.push(stack.top());
            }else if(ByteUtil::equals(OperationCodeEnum::OP_HASH160.code,bytesOperationCode)){
                if(stack.size()<1){
                    throw new exception("指令运行异常");
                }
                string publicKey = stack.top();stack.pop();
                string publicKeyHash = AccountUtil::publicKeyHashFromPublicKey(publicKey);
                stack.push(publicKeyHash);
            }else if(ByteUtil::equals(OperationCodeEnum::OP_EQUALVERIFY.code,bytesOperationCode)){
                if(stack.size()<2){
                    throw new exception("指令运行异常");
                }
                string top1 = stack.top();stack.pop();
                string top2 = stack.top();stack.pop();
                if(!StringUtil::equals(top1, top2)){
                    throw new exception("脚本执行失败");
                }
            }else if(ByteUtil::equals(OperationCodeEnum::OP_CHECKSIG.code,bytesOperationCode)){
                if(stack.size()<2){
                    throw new exception("指令运行异常");
                }
                string publicKey = stack.top();stack.pop();
                string signature = stack.top();stack.pop();
                vector<unsigned char> bytesSignature = ByteUtil::hexStringToBytes(signature);
                bool verifySignatureSuccess = TransactionTool::verifySignature(transactionEnvironment,publicKey,&bytesSignature);
                if(!verifySignatureSuccess){
                    throw new exception("脚本执行失败");
                }
                stack.push(ByteUtil::bytesToHexString(BooleanCodeEnum::TRUE.code));
            }else if(ByteUtil::equals(OperationCodeEnum::OP_PUSHDATA.code,bytesOperationCode)){
                if(script.size()<i+2){
                    throw new exception("指令运行异常");
                }
                i++;
                stack.push(script[i]);
            }else {
                throw new exception("不能识别的操作码");
            }
        }
        return stack;
    }
}
