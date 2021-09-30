//
// Created by XINGKAICHUN on 2021/9/30.
//

#include "NodeClient.h"
#include "../util/JsonUtil.h"
#include "../util/NetUtil.h"
#include "../setting/NetworkSetting.h"
#include "../util/StringUtil.h"


namespace netcoreclient{



    NodeClient::NodeClient(string ip) {
        this->ip = ip;
    }

    
    PostTransactionResponse NodeClient::postTransaction(PostTransactionRequest request) {
        string requestUrl = getUrl(API::POST_TRANSACTION);
        string requestBody = JsonUtil::toString(request);
        string responseHtml = NetUtil::get(requestUrl,requestBody);
        return JsonUtil::toObject(responseHtml,PostTransactionResponse{});
    }

    
    PingResponse NodeClient::pingNode(PingRequest request) {
        string requestUrl = getUrl(API::PING);
        string requestBody = JsonUtil::toString(request);
        string responseHtml = NetUtil::get(requestUrl,requestBody);
        return JsonUtil::toObject(responseHtml,PingResponse{});
    }

    
    GetBlockResponse NodeClient::getBlock(GetBlockRequest request) {
        string requestUrl = getUrl(API::GET_BLOCK);
        string requestBody = JsonUtil::toString(request);
        string responseHtml = NetUtil::get(requestUrl,requestBody);
        return JsonUtil::toObject(responseHtml,GetBlockResponse{});
    }

    
    GetNodesResponse NodeClient::getNodes(GetNodesRequest request) {
        string requestUrl = getUrl(API::GET_NODES);
        string requestBody = JsonUtil::toString(request);
        string responseHtml = NetUtil::get(requestUrl,requestBody);
        return JsonUtil::toObject(responseHtml, GetNodesResponse{});
    }

    
    PostBlockResponse NodeClient::postBlock(PostBlockRequest request) {
        string requestUrl = getUrl(API::POST_BLOCK);
        string requestBody = JsonUtil::toString(request);
        string responseHtml = NetUtil::get(requestUrl,requestBody);
        return JsonUtil::toObject(responseHtml,PostBlockResponse{});
    }

    
    PostBlockchainHeightResponse NodeClient::postBlockchainHeight(PostBlockchainHeightRequest request) {
        string requestUrl = getUrl(API::POST_BLOCKCHAIN_HEIGHT);
        string requestBody = JsonUtil::toString(request);
        string responseHtml = NetUtil::get(requestUrl,requestBody);
        return JsonUtil::toObject(responseHtml, PostBlockchainHeightResponse{});
    }

    
    GetBlockchainHeightResponse NodeClient::getBlockchainHeight(GetBlockchainHeightRequest request) {
        string requestUrl = getUrl(API::GET_BLOCKCHAIN_HEIGHT);
        string requestBody = JsonUtil::toString(request);
        string responseHtml = NetUtil::get(requestUrl,requestBody);
        return JsonUtil::toObject(responseHtml, GetBlockchainHeightResponse{});
    }

    
    GetUnconfirmedTransactionsResponse NodeClient::getUnconfirmedTransactions(GetUnconfirmedTransactionsRequest request) {
        string requestUrl = getUrl(API::GET_UNCONFIRMED_TRANSACTIONS);
        string requestBody = JsonUtil::toString(request);
        string responseHtml = NetUtil::get(requestUrl,requestBody);
        return JsonUtil::toObject(responseHtml, GetUnconfirmedTransactionsResponse{});
    }





    string NodeClient::getUrl(string api) {
        return "http://" + ip + ":" + StringUtil::valueOfUint64(NetworkSetting::PORT) + api;
    }


}