//
// Created by 40906 on 2021/10/5.
//

#include "BlockchainBrowserApplicationController.h"
#include "CommonController.h"
#include "../../util/LogUtil.h"
#include "../../setting/GenesisBlockSetting.h"
#include "../../core/tool/BlockTool.h"
#include "../../core/tool/SizeTool.h"
#include "../../util/TimeUtil.h"
#include "../vo/nodevo.h"
#include "../../util/StringUtil.h"
#include "../../core/tool/TransactionDtoTool.h"


namespace controller{


    BlockchainBrowserApplicationController::BlockchainBrowserApplicationController(BlockchainNetCore *blockchainNetCore,BlockchainBrowserApplicationService *blockchainBrowserApplicationService){
        this->blockchainNetCore = blockchainNetCore;
        this->blockchainBrowserApplicationService = blockchainBrowserApplicationService;
    }
    void BlockchainBrowserApplicationController::queryTransactionByTransactionHash(const httplib::Request &req, httplib::Response &res){
        try {
            QueryTransactionByTransactionHashRequest request = JsonUtil::toObject(req.body, QueryTransactionByTransactionHashRequest{});

            TransactionVo transactionVo = blockchainBrowserApplicationService->queryTransactionByTransactionHash(request.transactionHash);
/*            if(transactionVo == null){
                return Response.fail(ResponseMessage.NOT_FOUND_TRANSACTION);
            }*/

            QueryTransactionByTransactionHashResponse response;
            response.transaction=transactionVo;
            success(res,response);
        } catch (exception e){
            string message = "根据交易哈希查询交易失败";
            LogUtil::error(message,e);
            serviceUnavailable(res);
        }
    }
    void BlockchainBrowserApplicationController::queryTransactionsByBlockHashTransactionHeight(const httplib::Request &req, httplib::Response &res){
        try {
            QueryTransactionsByBlockHashTransactionHeightRequest request = JsonUtil::toObject(req.body, QueryTransactionsByBlockHashTransactionHeightRequest{});

            PageCondition pageCondition = request.pageCondition;
            if(StringUtil::isEmpty(request.blockHash)){
                requestParamIllegal(res);
                return;
            }
            vector<TransactionVo> transactionVos = blockchainBrowserApplicationService->queryTransactionListByBlockHashTransactionHeight(request.blockHash,pageCondition.from,pageCondition.size);
            QueryTransactionsByBlockHashTransactionHeightResponse response;
            response.transactions=transactionVos;
            success(res,response);
        } catch (exception e){
            string message = "根据交易高度查询交易失败";
            LogUtil::error(message,e);
            serviceUnavailable(res);
        }
    }
    void BlockchainBrowserApplicationController::queryTransactionOutputByAddress(const httplib::Request &req, httplib::Response &res){
        try {
            QueryTransactionOutputByAddressRequest request = JsonUtil::toObject(req.body, QueryTransactionOutputByAddressRequest{});

            TransactionOutputVo3 transactionOutputVo3 = blockchainBrowserApplicationService->queryTransactionOutputByAddress(request.address);
            QueryTransactionOutputByAddressResponse response;
            response.transactionOutput=transactionOutputVo3;
            success(res,response);
        } catch (exception e){
            string message = "[查询交易输出]失败";
            LogUtil::error(message,e);
            serviceUnavailable(res);
        }
    }
    void BlockchainBrowserApplicationController::queryTransactionOutputByTransactionOutputId(const httplib::Request &req, httplib::Response &res){
        try {
            QueryTransactionOutputByTransactionOutputIdRequest request = JsonUtil::toObject(req.body, QueryTransactionOutputByTransactionOutputIdRequest{});

            TransactionOutputVo3 transactionOutputVo3 = blockchainBrowserApplicationService->queryTransactionOutputByTransactionOutputId(request.transactionHash,request.transactionOutputIndex);
            QueryTransactionOutputByTransactionOutputIdResponse response;
            response.transactionOutput=transactionOutputVo3;
            success(res,response);
        } catch (exception e){
            string message = "[查询交易输出]失败";
            LogUtil::error(message,e);
            serviceUnavailable(res);
        }
    }
    void BlockchainBrowserApplicationController::queryBlockchainHeight(const httplib::Request &req, httplib::Response &res){
        try {
            long blockchainHeight = blockchainNetCore->getBlockchainCore()->queryBlockchainHeight();
            QueryBlockchainHeightResponse response;
            response.blockchainHeight=blockchainHeight;
            success(res,response);
        } catch (exception e){
            string message = "查询区块链高度失败";
            LogUtil::error(message,e);
            serviceUnavailable(res);
        }
    }
    void BlockchainBrowserApplicationController::queryUnconfirmedTransactionByTransactionHash(const httplib::Request &req, httplib::Response &res){
        try {
            QueryUnconfirmedTransactionByTransactionHashRequest request = JsonUtil::toObject(req.body, QueryUnconfirmedTransactionByTransactionHashRequest{});

            UnconfirmedTransactionVo unconfirmedTransactionVo = blockchainBrowserApplicationService->queryUnconfirmedTransactionByTransactionHash(request.transactionHash);
/*            if(unconfirmedTransactionVo == null){
                return Response.fail(ResponseMessage.NOT_FOUND_UNCONFIRMED_TRANSACTIONS);
            }*/
            QueryUnconfirmedTransactionByTransactionHashResponse response;
            response.transaction=unconfirmedTransactionVo;
            success(res,response);
        } catch (exception e){
            string message = "根据交易哈希查询未确认交易失败";
            LogUtil::error(message,e);
            serviceUnavailable(res);
        }
    }
    void BlockchainBrowserApplicationController::queryUnconfirmedTransactions(const httplib::Request &req, httplib::Response &res){
        try {
            QueryUnconfirmedTransactionsRequest request = JsonUtil::toObject(req.body, QueryUnconfirmedTransactionsRequest{});

            PageCondition pageCondition = request.pageCondition;
            vector<TransactionDto> transactionDtos = blockchainNetCore->getBlockchainCore()->queryUnconfirmedTransactions(pageCondition.from,pageCondition.size);
/*            if(transactionDtos == null){
                return Response.fail(ResponseMessage.NOT_FOUND_UNCONFIRMED_TRANSACTIONS);
            }*/

            vector<UnconfirmedTransactionVo> unconfirmedTransactionVos;
            for(TransactionDto transactionDto : transactionDtos){
                UnconfirmedTransactionVo unconfirmedTransactionVo = blockchainBrowserApplicationService->queryUnconfirmedTransactionByTransactionHash(TransactionDtoTool::calculateTransactionHash(transactionDto));
 /*               if(unconfirmedTransactionVo != null){
                    unconfirmedTransactionVos.push_back(unconfirmedTransactionVo);
                }*/
            }
            QueryUnconfirmedTransactionsResponse response;
            response.unconfirmedTransactions=unconfirmedTransactionVos;
            success(res,response);
        } catch (exception e){
            string message = "查询未确认交易失败";
            LogUtil::error(message,e);
            serviceUnavailable(res);
        }
    }
    void BlockchainBrowserApplicationController::queryBlockByBlockHeight(const httplib::Request &req, httplib::Response &res){
        try {
            QueryBlockByBlockHeightRequest request = JsonUtil::toObject(req.body, QueryBlockByBlockHeightRequest{});

            BlockVo blockVo = blockchainBrowserApplicationService->queryBlockViewByBlockHeight(request.blockHeight);
/*            if(blockVo == null){
                return Response.fail(ResponseMessage.NOT_FOUND_BLOCK);
            }*/
            QueryBlockByBlockHeightResponse response;
            response.block=blockVo;
            success(res,response);
        } catch (exception e){
            string message = "查询获取失败";
            LogUtil::error(message,e);
            serviceUnavailable(res);
        }
    }
    void BlockchainBrowserApplicationController::queryBlockByBlockHash(const httplib::Request &req, httplib::Response &res){
        try {
            QueryBlockByBlockHashRequest request = JsonUtil::toObject(req.body, QueryBlockByBlockHashRequest{});

            Block block = blockchainNetCore->getBlockchainCore()->queryBlockByBlockHash(request.blockHash);
/*            if(block == null){
                return Response.fail(ResponseMessage.NOT_FOUND_BLOCK);
            }*/
            BlockVo blockVo = blockchainBrowserApplicationService->queryBlockViewByBlockHeight(block.height);
            QueryBlockByBlockHashResponse response;
            response.block=blockVo;
            success(res,response);
        } catch (exception e){
            string message = "[根据区块哈希查询区块]失败";
            LogUtil::error(message,e);
            serviceUnavailable(res);
        }
    }
    void BlockchainBrowserApplicationController::queryTop10Blocks(const httplib::Request &req, httplib::Response &res){
        try {
            vector<Block> blocks;
            long blockHeight = blockchainNetCore->getBlockchainCore()->queryBlockchainHeight();
            while (true){
                if(blockHeight <= GenesisBlockSetting::HEIGHT){
                    break;
                }
                Block block = blockchainNetCore->getBlockchainCore()->queryBlockByBlockHeight(blockHeight);
                blocks.push_back(block);
                if(blocks.size() >= 10){
                    break;
                }
                blockHeight--;
            }

            vector<BlockVo2> blockVos;
            for(Block &block : blocks){
                BlockVo2 blockVo;
                blockVo.height=block.height;
                blockVo.blockSize=SizeTool::calculateBlockSize(&block)+"字符";
                blockVo.transactionCount=BlockTool::getTransactionCount(&block);
                blockVo.minerIncentiveValue=BlockTool::getWritedIncentiveValue(&block);
                blockVo.time=TimeUtil::formatMillisecondTimestamp(block.timestamp);
                blockVo.hash=block.hash;
                blockVos.push_back(blockVo);
            }

            QueryTop10BlocksResponse response;
            response.blocks=blockVos;
            success(res,response);
        } catch (exception e){
            string message = "[查询最近的10个区块]失败";
            LogUtil::error(message,e);
            serviceUnavailable(res);
        }
    }

}

