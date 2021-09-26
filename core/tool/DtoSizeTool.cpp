//
// Created by 40906 on 2021/9/20.
//

#include "DtoSizeTool.h"
#include "../../setting/BlockSetting.h"
#include "../../util/LogUtil.h"
#include "../../util/StringUtil.h"
#include "../../setting/ScriptSetting.h"
#include "../../setting/TransactionSetting.h"


namespace DtoSizeTool{



    bool checkInputScriptSize(InputScriptDto *inputScriptDto) ;
    uint64_t calculateTransactionsOutputSize(vector<TransactionOutputDto> *transactionOutputDtos) ;
    uint64_t calculateTransactionOutputSize(TransactionOutputDto *transactionOutputDto) ;
    uint64_t calculateTransactionInputsSize(vector<TransactionInputDto> *inputs) ;
    uint64_t calculateTransactionInputSize(TransactionInputDto *input) ;
    uint64_t calculateScriptSize(ScriptDto *script) ;
    uint64_t sizeOfString(string value) ;
    uint64_t sizeOfUint64(uint64_t number) ;
    
//region 校验大小
/**
 * 校验区块大小。用来限制区块的大小。
 * 注意：校验区块的大小，不仅要校验区块的大小
 * ，还要校验区块内部各个属性(时间戳、前哈希、随机数、交易)的大小。
 */
    bool checkBlockSize(BlockDto *blockDto) {
        //区块的时间戳的长度不需要校验  假设时间戳长度不正确，则在随后的业务逻辑中走不通

        //区块的前哈希的长度不需要校验  假设前哈希长度不正确，则在随后的业务逻辑中走不通

        //校验区块随机数大小
        uint64_t nonceSize = sizeOfString(blockDto->nonce);
        if(nonceSize != BlockSetting::NONCE_CHARACTER_COUNT){
            LogUtil::debug("nonce["+blockDto->nonce+"]长度非法。");
            return false;
        }

        //校验每一笔交易大小
        vector<TransactionDto> transactionDtos = blockDto->transactions;
        if(!transactionDtos.empty()){
            for(TransactionDto transactionDto:transactionDtos){
                if(!checkTransactionSize(&transactionDto)){
                    LogUtil::debug("交易数据异常，交易大小非法。");
                    return false;
                }
            }
        }

        //校验区块占用的存储空间
        uint64_t blockSize = calculateBlockSize(blockDto);
        if(blockSize > BlockSetting::BLOCK_MAX_CHARACTER_COUNT){
            LogUtil::debug("区块数据的大小是["+StringUtil::valueOfUint64(blockSize)+"]超过了限制["+StringUtil::valueOfUint64(BlockSetting::BLOCK_MAX_CHARACTER_COUNT) +"]。");
            return false;
        }
        return true;
    }
/**
 * 校验交易的大小：用来限制交易的大小。
 * 注意：校验交易的大小，不仅要校验交易的大小
 * ，还要校验交易内部各个属性(交易输入、交易输出)的大小。
 */
    bool checkTransactionSize(TransactionDto *transactionDto) {
        //校验交易输入
        vector<TransactionInputDto> transactionInputDtos = transactionDto->inputs;
        if(!transactionInputDtos.empty()){
            for(TransactionInputDto transactionInputDto:transactionInputDtos){
                //交易的未花费输出大小不需要校验  假设不正确，则在随后的业务逻辑中走不通

                //校验脚本大小
                InputScriptDto inputScriptDto = transactionInputDto.inputScript;
                //校验输入脚本的大小
                if(!checkInputScriptSize(&inputScriptDto)){
                    return false;
                }
            }
        }

        //校验交易输出
        vector<TransactionOutputDto> transactionOutputDtos = transactionDto->outputs;
        if(!transactionOutputDtos.empty()){
            for(TransactionOutputDto transactionOutputDto:transactionOutputDtos){
                //交易输出金额大小不需要校验  假设不正确，则在随后的业务逻辑中走不通

                //校验脚本大小
                OutputScriptDto outputScriptDto = transactionOutputDto.outputScript;
                //校验输出脚本的大小
                if(!checkOutputScriptSize(&outputScriptDto)){
                    return false;
                }

            }
        }

        //校验整笔交易大小十分合法
        uint64_t transactionSize = calculateTransactionSize(transactionDto);
        if(transactionSize > TransactionSetting::TRANSACTION_MAX_CHARACTER_COUNT){
            LogUtil::debug("交易的大小是["+StringUtil::valueOfUint64(transactionSize)+"]，超过了限制值["+StringUtil::valueOfUint64(TransactionSetting::TRANSACTION_MAX_CHARACTER_COUNT) +"]。");
            return false;
        }
        return true;
    }

/**
 * 校验输出脚本的大小
 */
    bool checkOutputScriptSize(OutputScriptDto *outputScriptDto) {
        //校验脚本大小
        if(!checkScriptSize(outputScriptDto)){
            return false;
        }
        return true;
    }

/**
 * 校验脚本的大小
 */
    bool checkScriptSize(ScriptDto *scriptDto) {
        //脚本内的操作码、操作数大小不需要校验，因为操作码、操作数不合规，在脚本结构上就构不成一个合格的脚本。
        if(calculateScriptSize(scriptDto) > ScriptSetting::SCRIPT_MAX_CHARACTER_COUNT){
            LogUtil::debug("交易校验失败：交易输出脚本大小超出限制。");
            return false;
        }
        return true;
    }
//endregion



//region 计算大小
    uint64_t calculateBlockSize(BlockDto *blockDto) {
        uint64_t size = 0;
        uint64_t timestamp = blockDto->timestamp;
        size += sizeOfUint64(timestamp);

        string previousBlockHash = blockDto->previousHash;
        size += sizeOfString(previousBlockHash);

        string nonce = blockDto->nonce;
        size += sizeOfString(nonce);
        vector<TransactionDto> transactionDtos = blockDto->transactions;
        for(TransactionDto transactionDto:transactionDtos){
            size += calculateTransactionSize(&transactionDto);
        }
        return size;
    }
    uint64_t calculateTransactionSize(TransactionDto *transactionDto) {
        uint64_t size = 0;
        vector<TransactionInputDto> transactionInputDtos = transactionDto->inputs;
        size += calculateTransactionInputsSize(&transactionInputDtos);
        vector<TransactionOutputDto> transactionOutputDtos = transactionDto->outputs;
        size += calculateTransactionsOutputSize(&transactionOutputDtos);
        return size;
    }
    //endregion


    /**
 * 校验输入脚本的大小
 */
    bool checkInputScriptSize(InputScriptDto *inputScriptDto) {
        //校验脚本大小
        if(!checkScriptSize(inputScriptDto)){
            return false;
        }
        return true;
    }
    uint64_t calculateTransactionsOutputSize(vector<TransactionOutputDto> *transactionOutputDtos) {
        uint64_t size = 0;
        if(transactionOutputDtos == nullptr){
            return size;
        }
        for(TransactionOutputDto transactionOutputDto:*transactionOutputDtos){
            size += calculateTransactionOutputSize(&transactionOutputDto);
        }
        return size;
    }
    uint64_t calculateTransactionOutputSize(TransactionOutputDto *transactionOutputDto) {
        uint64_t size = 0;
        OutputScriptDto outputScriptDto = transactionOutputDto->outputScript;
        size += calculateScriptSize(&outputScriptDto);
        uint64_t value = transactionOutputDto->value;
        size += sizeOfUint64(value);
        return size;
    }
    uint64_t calculateTransactionInputsSize(vector<TransactionInputDto> *inputs) {
        uint64_t size = 0;
        if(inputs == nullptr){
            return size;
        }
        for(TransactionInputDto transactionInputDto:*inputs){
            size += calculateTransactionInputSize(&transactionInputDto);
        }
        return size;
    }
    uint64_t calculateTransactionInputSize(TransactionInputDto *input) {
        uint64_t size = 0;
        string transactionHash = input->transactionHash;
        size += sizeOfString(transactionHash);
        uint64_t transactionOutputIndex = input->transactionOutputIndex;
        size += sizeOfUint64(transactionOutputIndex);
        InputScriptDto inputScriptDto = input->inputScript;
        size += calculateScriptSize(&inputScriptDto);
        return size;
    }
    uint64_t calculateScriptSize(ScriptDto *script) {
        uint64_t size = 0;
        if(script == nullptr){
            return size;
        }
        for(string scriptCode:*script){
            size += sizeOfString(scriptCode);
        }
        return size;
    }

    uint64_t sizeOfString(string value) {
        return StringUtil::length(value);
    }
    uint64_t sizeOfUint64(uint64_t number) {
        return StringUtil::length(StringUtil::valueOfUint64(number));
    }
}