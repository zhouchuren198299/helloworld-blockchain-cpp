//
// Created by 40906 on 2021/9/20.
//

#include "StructureTool.h"
#include "ScriptTool.h"
#include "../../util/LogUtil.h"
#include "../../util/StringUtil.h"
#include "BlockTool.h"
#include "../../setting/BlockSetting.h"

using namespace model;

namespace StructureTool{


    /**
  * 校验区块的结构
  */
    bool checkBlockStructure(Block *block) {
        vector<Transaction> transactions = block->transactions;
        if(transactions.empty()){
            LogUtil::debug("区块数据异常：区块中的交易数量为0。区块必须有一笔创世的交易。");
            return false;
        }
        //校验区块中交易的数量
        uint64_t transactionCount = BlockTool::getTransactionCount(block);
        if(transactionCount > BlockSetting::BLOCK_MAX_TRANSACTION_COUNT){
            LogUtil::debug("区块包含的交易数量是["+StringUtil::valueOfUint64(transactionCount)+"]，超过了限制["+ StringUtil::valueOfUint64(BlockSetting::BLOCK_MAX_TRANSACTION_COUNT)+"]。");
            return false;
        }
        for(int i=0; i<transactions.size(); i++){
            Transaction transaction = transactions[i];
            if(i == 0){
                if(transaction.transactionType != TransactionTypeEnum::GENESIS_TRANSACTION){
                    LogUtil::debug("区块数据异常：区块第一笔交易必须是创世交易。");
                    return false;
                }
            }else {
                if(transaction.transactionType != TransactionTypeEnum::STANDARD_TRANSACTION){
                    LogUtil::debug("区块数据异常：区块非第一笔交易必须是标准交易。");
                    return false;
                }
            }
        }
        //校验交易的结构
        for(Transaction transaction:transactions){
            if(!checkTransactionStructure(&transaction)){
                LogUtil::debug("交易数据异常：交易结构异常。");
                return false;
            }
        }
        return true;
    }
    /**
     * 校验交易的结构
     */
    bool checkTransactionStructure(Transaction *transaction) {
        if(transaction->transactionType == TransactionTypeEnum::GENESIS_TRANSACTION){
            vector<TransactionInput> inputs = transaction->inputs;
            if(!inputs.empty()){
                LogUtil::debug("交易数据异常：创世交易不能有交易输入。");
                return false;
            }
            vector<TransactionOutput> outputs = transaction->outputs;
            if(outputs.size() != 1){
                LogUtil::debug("交易数据异常：创世交易有且只能有一笔交易输出。");
                return false;
            }
        }else if(transaction->transactionType == TransactionTypeEnum::STANDARD_TRANSACTION){
            vector<TransactionInput> inputs = transaction->inputs;
            if(inputs.size() < 1){
                LogUtil::debug("交易数据异常：标准交易的交易输入数量至少是1。");
                return false;
            }
            vector<TransactionOutput> outputs = transaction->outputs;
            if(outputs.size() < 1){
                LogUtil::debug("交易数据异常：标准交易的交易输出数量至少是1。");
                return false;
            }
        }else {
            LogUtil::debug("交易数据异常：不能识别的交易的类型。");
            return false;
        }
        //校验输入脚本
        vector<TransactionInput> inputs = transaction->inputs;
        if(!inputs.empty()){
            for (TransactionInput input:inputs) {
                //这里采用严格校验，必须是P2PKH输入脚本。
                if(!ScriptTool::isPayToPublicKeyHashInputScript(&input.inputScript)){
                    LogUtil::debug("交易数据异常：交易输入脚本不是P2PKH输入脚本。");
                    return false;
                }
            }
        }
        //校验输出脚本
        vector<TransactionOutput> outputs = transaction->outputs;
        if(!outputs.empty()){
            for (TransactionOutput output:outputs) {
                //这里采用严格校验，必须是P2PKH输出脚本
                if(!ScriptTool::isPayToPublicKeyHashOutputScript(&output.outputScript)){
                    return false;
                }
            }
        }
        return true;
    }
}