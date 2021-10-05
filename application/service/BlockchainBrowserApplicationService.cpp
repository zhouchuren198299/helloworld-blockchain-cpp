//
// Created by 40906 on 2021/10/5.
//

#include "BlockchainBrowserApplicationService.h"
#include "../../util/NullUtil.h"
#include "../../core/tool/BlockTool.h"
#include "../../core/tool/SizeTool.h"
#include "../../core/tool/TransactionTool.h"
#include "../../util/TimeUtil.h"
#include "../../core/tool/ScriptTool.h"
#include "../../util/StringUtil.h"


namespace service {


        BlockchainBrowserApplicationService::BlockchainBrowserApplicationService(BlockchainNetCore *blockchainNetCore){
            this->blockchainNetCore = blockchainNetCore;
        }
        TransactionOutputVo3 BlockchainBrowserApplicationService::queryTransactionOutputByTransactionOutputId(string transactionHash, uint64_t transactionOutputIndex){
        //查询交易输出
            TransactionOutput transactionOutput = blockchainNetCore->getBlockchainCore()->getBlockchainDatabase()->queryTransactionOutputByTransactionOutputId(transactionHash,transactionOutputIndex);
/*            if(transactionOutput == null){
                return null;
            }*/

            TransactionOutputVo3 transactionOutputVo3;
            transactionOutputVo3.fromBlockHeight=transactionOutput.blockHeight;
            transactionOutputVo3.fromBlockHash=transactionOutput.blockHash;
            transactionOutputVo3.fromTransactionHash=transactionOutput.transactionHash;
            transactionOutputVo3.value=transactionOutput.value;
            transactionOutputVo3.fromOutputScript=ScriptTool::stringOutputScript(transactionOutput.outputScript);
            transactionOutputVo3.fromTransactionOutputIndex=transactionOutput.transactionOutputIndex;

            //是否是未花费输出
            TransactionOutput transactionOutputTemp = blockchainNetCore->getBlockchainCore()->getBlockchainDatabase()->queryUnspentTransactionOutputByTransactionOutputId(transactionOutput.transactionHash,transactionOutput.transactionOutputIndex);
/*
            transactionOutputVo3.spent(transactionOutputTemp==null);
*/

            //来源
            TransactionVo inputTransactionVo = queryTransactionByTransactionHash(transactionOutput.transactionHash);
            transactionOutputVo3.inputTransaction=inputTransactionVo;
            transactionOutputVo3.transactionType=inputTransactionVo.transactionType;


            //去向
            TransactionVo outputTransactionVo;
            if(!NullUtil::isNullTransactionOutput(transactionOutputTemp)){
                Transaction destinationTransaction = blockchainNetCore->getBlockchainCore()->getBlockchainDatabase()->queryDestinationTransactionByTransactionOutputId(transactionOutput.transactionHash,transactionOutput.transactionOutputIndex);
                vector<TransactionInput> &inputs = destinationTransaction.inputs;
                if(!inputs.empty()){
                    for(int inputIndex=0; inputIndex<inputs.size(); inputIndex++){
                        TransactionInput &transactionInput = inputs[inputIndex];
                        TransactionOutput unspentTransactionOutput = transactionInput.unspentTransactionOutput;
                        if(StringUtil::equals(transactionOutput.transactionHash,unspentTransactionOutput.transactionHash) &&
                           transactionOutput.transactionOutputIndex==unspentTransactionOutput.transactionOutputIndex){
                            transactionOutputVo3.toTransactionInputIndex=inputIndex+1;
                            transactionOutputVo3.toInputScript=ScriptTool::stringInputScript(transactionInput.inputScript);
                            break;
                        }
                    }
                }
                outputTransactionVo = queryTransactionByTransactionHash(destinationTransaction.transactionHash);
                transactionOutputVo3.toBlockHeight=outputTransactionVo.blockHeight;
                transactionOutputVo3.toBlockHash=outputTransactionVo.blockHash;
                transactionOutputVo3.toTransactionHash=outputTransactionVo.transactionHash;
                transactionOutputVo3.outputTransaction=outputTransactionVo;
            }
            return transactionOutputVo3;
        }
        TransactionOutputVo3 BlockchainBrowserApplicationService::queryTransactionOutputByAddress(string address){
            TransactionOutput transactionOutput = blockchainNetCore->getBlockchainCore()->queryTransactionOutputByAddress(address);
/*            if(transactionOutput == null){
                return null;
            }*/
            TransactionOutputVo3 transactionOutputVo3 = queryTransactionOutputByTransactionOutputId(transactionOutput.transactionHash,transactionOutput.transactionOutputIndex);
            return transactionOutputVo3;
        }
        TransactionVo BlockchainBrowserApplicationService::queryTransactionByTransactionHash(string transactionHash){
            Transaction transaction = blockchainNetCore->getBlockchainCore()->queryTransactionByTransactionHash(transactionHash);
/*            if(transaction == null){
                return null;
            }*/

            TransactionVo transactionVo;
            transactionVo.transactionHash=transaction.transactionHash;
            transactionVo.blockHeight=transaction.blockHeight;

            transactionVo.transactionFee=TransactionTool::calculateTransactionFee(&transaction);
            transactionVo.transactionType=transaction.transactionType;
            transactionVo.transactionInputCount=TransactionTool::getTransactionInputCount(&transaction);
            transactionVo.transactionOutputCount=TransactionTool::getTransactionOutputCount(&transaction);
            transactionVo.transactionInputValues=TransactionTool::getInputValue(&transaction);
            transactionVo.transactionOutputValues=TransactionTool::getOutputValue(&transaction);

            long blockchainHeight = blockchainNetCore->getBlockchainCore()->queryBlockchainHeight();
            Block block = blockchainNetCore->getBlockchainCore()->queryBlockByBlockHeight(transaction.blockHeight);
            transactionVo.confirmCount=blockchainHeight-block.height+1;
            transactionVo.blockTime=TimeUtil::formatMillisecondTimestamp(block.timestamp);
            transactionVo.blockHash=block.hash;

            vector<TransactionInput> inputs = transaction.inputs;
            vector<TransactionInputVo> transactionInputVos;
            if(!inputs.empty()){
                for(TransactionInput transactionInput:inputs){
                    TransactionInputVo transactionInputVo;
                    transactionInputVo.address=transactionInput.unspentTransactionOutput.address;
                    transactionInputVo.value=transactionInput.unspentTransactionOutput.value;
                    transactionInputVo.inputScript=ScriptTool::stringInputScript(transactionInput.inputScript);
                    transactionInputVo.transactionHash=transactionInput.unspentTransactionOutput.transactionHash;
                    transactionInputVo.transactionOutputIndex=transactionInput.unspentTransactionOutput.transactionOutputIndex;
                    transactionInputVos.push_back(transactionInputVo);
                }
            }
            transactionVo.transactionInputs=transactionInputVos;

            vector<TransactionOutput> outputs = transaction.outputs;
            vector<TransactionOutputVo> transactionOutputVos;
            if(!outputs.empty()){
                for(TransactionOutput transactionOutput:outputs){
                    TransactionOutputVo transactionOutputVo;
                    transactionOutputVo.address=transactionOutput.address;
                    transactionOutputVo.value=transactionOutput.value;
                    transactionOutputVo.outputScript=ScriptTool::stringOutputScript(transactionOutput.outputScript);
                    transactionOutputVo.transactionHash=transactionOutput.transactionHash;
                    transactionOutputVo.transactionOutputIndex=transactionOutput.transactionOutputIndex;
                    transactionOutputVos.push_back(transactionOutputVo);
                }
            }
            transactionVo.transactionOutputs=transactionOutputVos;

            vector<string> inputScripts;
            for (TransactionInputVo transactionInputVo : transactionInputVos){
                inputScripts.push_back(transactionInputVo.inputScript);
            }
            transactionVo.inputScripts=inputScripts;

            vector<string> outputScripts;
            for (TransactionOutputVo transactionOutputVo : transactionOutputVos){
                outputScripts.push_back(transactionOutputVo.outputScript);
            }
            transactionVo.outputScripts=outputScripts;

            return transactionVo;
        }
        vector<TransactionVo> BlockchainBrowserApplicationService::queryTransactionListByBlockHashTransactionHeight(string blockHash, uint64_t from, uint64_t size){
            Block block = blockchainNetCore->getBlockchainCore()->queryBlockByBlockHash(blockHash);
            vector<TransactionVo> transactionVos;
            for(long i=from; i<from+size; i++){
                if(from < 0){
                    break;
                }
                if(i > block.transactionCount){
                    break;
                }
                long transactionHeight = block.previousTransactionHeight + i;
                Transaction transaction = blockchainNetCore->getBlockchainCore()->queryTransactionByTransactionHeight(transactionHeight);
                TransactionVo transactionVo = queryTransactionByTransactionHash(transaction.transactionHash);
                transactionVos.push_back(transactionVo);
            }
            return transactionVos;
        }
        BlockVo BlockchainBrowserApplicationService::queryBlockViewByBlockHeight(uint64_t blockHeight){
            Block block = blockchainNetCore->getBlockchainCore()->queryBlockByBlockHeight(blockHeight);
/*            if(block == null){
                return null;
            }*/
            Block nextBlock = blockchainNetCore->getBlockchainCore()->queryBlockByBlockHeight(block.height+1);

            BlockVo blockVo;
            blockVo.height=block.height;
            blockVo.confirmCount=BlockTool::getTransactionCount(&block);
            blockVo.blockSize=SizeTool::calculateBlockSize(&block)+"字符";
            blockVo.transactionCount=BlockTool::getTransactionCount(&block);
            blockVo.time=TimeUtil::formatMillisecondTimestamp(block.timestamp);
            blockVo.minerIncentiveValue=BlockTool::getWritedIncentiveValue(&block);
            blockVo.difficulty=BlockTool::formatDifficulty(block.difficulty);
            blockVo.nonce=block.nonce;
            blockVo.hash=block.hash;
            blockVo.previousBlockHash=block.previousHash;
/*
            blockVo.nextBlockHash=nextBlock==null?null:nextBlock.hash;
*/
            blockVo.merkleTreeRoot=block.merkleTreeRoot;
            return blockVo;
        }
        UnconfirmedTransactionVo BlockchainBrowserApplicationService::queryUnconfirmedTransactionByTransactionHash(string transactionHash){
            try {
                TransactionDto transactionDto = blockchainNetCore->getBlockchainCore()->queryUnconfirmedTransactionByTransactionHash(transactionHash);
/*                if(NullUtil::isNullTransactionDto(&TransactionDto)){
                    return null;
                }*/
                Transaction transaction = blockchainNetCore->getBlockchainCore()->getBlockchainDatabase()->transactionDto2Transaction(&transactionDto);
                UnconfirmedTransactionVo transactionDtoVo;
                transactionDtoVo.transactionHash=transaction.transactionHash;

                vector<TransactionInputVo2> inputDtos;
                vector<TransactionInput> &inputs = transaction.inputs;
                if(!inputs.empty()){
                    for(TransactionInput &input:inputs){
                        TransactionInputVo2 transactionInputVo;
                        transactionInputVo.address=input.unspentTransactionOutput.address;
                        transactionInputVo.transactionHash=input.unspentTransactionOutput.transactionHash;
                        transactionInputVo.transactionOutputIndex=input.unspentTransactionOutput.transactionOutputIndex;
                        transactionInputVo.value=input.unspentTransactionOutput.value;
                        inputDtos.push_back(transactionInputVo);
                    }
                }
                transactionDtoVo.transactionInputs=inputDtos;

                vector<TransactionOutputVo2> outputDtos;
                vector<TransactionOutput> outputs = transaction.outputs;
                if(!outputs.empty()){
                    for(TransactionOutput &output:outputs){
                        TransactionOutputVo2 transactionOutputVo;
                        transactionOutputVo.address=output.address;
                        transactionOutputVo.value=output.value;
                        outputDtos.push_back(transactionOutputVo);
                    }
                }
                transactionDtoVo.transactionOutputs=outputDtos;

                return transactionDtoVo;
            }catch (exception e){
/*                LogUtil::error("根据交易哈希查询未确认交易异常",e);
                return null;*/
            }
        }
};