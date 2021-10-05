//
// Created by 40906 on 2021/10/4.
//

#include "NodeConsoleApplicationController.h"
#include "CommonController.h"
#include "../vo/nodevo.h"
#include "../vo/minervo.h"
#include "../vo/synchronizervo.h"
#include "../../util/JsonUtil.h"
#include "../vo/blockvo.h"
#include "../../util/StringUtil.h"

using namespace vo;



namespace controller{


    NodeConsoleApplicationController::NodeConsoleApplicationController(BlockchainNetCore *blockchainNetCore){
        this->blockchainNetCore = blockchainNetCore;
    }



    void NodeConsoleApplicationController::IsMineActive(const httplib::Request &req, httplib::Response &res){
        boolean isMineActive = blockchainNetCore->getBlockchainCore()->getMiner()->isActive();
        IsMinerActiveResponse response;
        response.minerInActiveState=isMineActive;
        return success(res,response);
    }
    void NodeConsoleApplicationController::ActiveMiner(const httplib::Request &req, httplib::Response &res){
        blockchainNetCore->getBlockchainCore()->getMiner()->active();
        ActiveMinerResponse response;
        return success(res,response);
    }
    void NodeConsoleApplicationController::DeactiveMiner(const httplib::Request &req, httplib::Response &res){
        blockchainNetCore->getBlockchainCore()->getMiner()->deactive();
        DeactiveMinerResponse response;
        return success(res,response);
    }
    void NodeConsoleApplicationController::IsAutoSearchBlock(const httplib::Request &req, httplib::Response &res){
        boolean isAutoSearchBlock = blockchainNetCore->getNetCoreConfiguration()->isAutoSearchBlock();
        IsAutoSearchBlockResponse response;
        response.autoSearchBlock=isAutoSearchBlock;
        return success(res,response);
    }
    void NodeConsoleApplicationController::ActiveAutoSearchBlock(const httplib::Request &req, httplib::Response &res){
        blockchainNetCore->getNetCoreConfiguration()->activeAutoSearchBlock();
        ActiveAutoSearchBlockResponse response;
        return success(res,response);
    }
    void NodeConsoleApplicationController::DeactiveAutoSearchBlock(const httplib::Request &req, httplib::Response &res){
        blockchainNetCore->getNetCoreConfiguration()->deactiveAutoSearchBlock();
        DeactiveAutoSearchBlockResponse response;
        return success(res,response);
    }
    void NodeConsoleApplicationController::AddNode(const httplib::Request &req, httplib::Response &res){
        AddNodeRequest request = JsonUtil::toObject(req.body, AddNodeRequest{});

        string ip = request.ip;
        if(StringUtil::isEmpty(ip)){
            requestParamIllegal(res);
        }
        //TODO
/*        if(blockchainNetCore->getNodeService()->queryNode(ip) != null){
            //节点存在，认为是成功添加。
            AddNodeResponse response;
            response.addNodeSuccess=true;
            return success(res,response);
        }*/
        Node node;
        node.ip=ip;
        node.blockchainHeight=0;
        blockchainNetCore->getNodeService()->addNode(node);
        AddNodeResponse response;
        response.addNodeSuccess=true;
        return success(res,response);
    }
    void NodeConsoleApplicationController::UpdateNode(const httplib::Request &req, httplib::Response &res){
        UpdateNodeRequest request = JsonUtil::toObject(req.body, UpdateNodeRequest{});

        string ip = request.ip;
        if(StringUtil::isEmpty(ip)){
            return requestParamIllegal(res);
        }
        Node node;
        node.ip=ip;
        node.blockchainHeight=request.blockchainHeight;
        blockchainNetCore->getNodeService()->updateNode(node);
        UpdateNodeResponse response;
        return success(res,response);
    }
    void NodeConsoleApplicationController::DeleteNode(const httplib::Request &req, httplib::Response &res){
        DeleteNodeRequest request = JsonUtil::toObject(req.body, DeleteNodeRequest{});

        blockchainNetCore->getNodeService()->deleteNode(request.ip);
        DeleteNodeResponse response;
        return success(res,response);
    }

    void NodeConsoleApplicationController::queryAllNodes(const httplib::Request &req, httplib::Response &res){
        vector<Node> nodes = blockchainNetCore->getNodeService()->queryAllNodes();

        vector<NodeVo> nodeVos;
        if(!nodes.empty()){
            for (Node node:nodes) {
                NodeVo nodeVo;
                nodeVo.ip=node.ip;
                nodeVo.blockchainHeight=node.blockchainHeight;
                nodeVos.push_back(nodeVo);
            }
        }

        QueryAllNodesResponse response;
        response.nodes=nodeVos;

        success(res,response);
    }
    void NodeConsoleApplicationController::IsAutoSearchNode(const httplib::Request &req, httplib::Response &res){
        boolean isAutoSearchNode = blockchainNetCore->getNetCoreConfiguration()->isAutoSearchNode();
        IsAutoSearchNodeResponse response;
        response.autoSearchNode=isAutoSearchNode;
        return success(res,response);
    }
    void NodeConsoleApplicationController::ActiveAutoSearchNode(const httplib::Request &req, httplib::Response &res){
        blockchainNetCore->getNetCoreConfiguration()->activeAutoSearchNode();
        ActiveAutoSearchNodeResponse response;
        return success(res,response);
    }
    void NodeConsoleApplicationController::DeactiveAutoSearchNode(const httplib::Request &req, httplib::Response &res){
        blockchainNetCore->getNetCoreConfiguration()->deactiveAutoSearchNode();
        DeactiveAutoSearchNodeResponse response;
        return success(res,response);
    }
    void NodeConsoleApplicationController::DeleteBlocks(const httplib::Request &req, httplib::Response &res){
        DeleteBlocksRequest request = JsonUtil::toObject(req.body, DeleteBlocksRequest{});

        blockchainNetCore->getBlockchainCore()->deleteBlocks(request.blockHeight);
        DeleteBlocksResponse response;
        return success(res,response);
    }
    void NodeConsoleApplicationController::GetMinerMineMaxBlockHeight(const httplib::Request &req, httplib::Response &res){
        uint64_t maxBlockHeight = blockchainNetCore->getBlockchainCore()->getMiner()->getMinerMineMaxBlockHeight();

        GetMinerMineMaxBlockHeightResponse response;
        response.maxBlockHeight=maxBlockHeight;
        return success(res,response);
    }
    void NodeConsoleApplicationController::SetMinerMineMaxBlockHeight(const httplib::Request &req, httplib::Response &res){
        SetMinerMineMaxBlockHeightRequest request = JsonUtil::toObject(req.body, SetMinerMineMaxBlockHeightRequest{});

        uint64_t height = request.maxBlockHeight;
        blockchainNetCore->getBlockchainCore()->getMiner()->setMinerMineMaxBlockHeight(height);
        SetMinerMineMaxBlockHeightResponse response;
        return success(res,response);
    }
}

