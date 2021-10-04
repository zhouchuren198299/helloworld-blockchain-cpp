//
// Created by XINGKAICHUN on 2021/9/30.
//

#include "UnconfirmedTransactionsSearcher.h"
#include "../netcoreclient/NodeClient.h"
#include "../util/LogUtil.h"
#include "../util/ThreadUtil.h"
#include "../util/NullUtil.h"

using namespace netcoreclient;


namespace netcore{


    UnconfirmedTransactionsSearcher::UnconfirmedTransactionsSearcher(NetCoreConfiguration *netCoreConfiguration, BlockchainCore *blockchainCore, NodeService *nodeService){
        this->netCoreConfiguration = netCoreConfiguration;
        this->blockchainCore = blockchainCore;
        this->nodeService = nodeService;
    }

    void UnconfirmedTransactionsSearcher::start(){
        try {
            while (true){
                searchUnconfirmedTransactions();
                ThreadUtil::millisecondSleep(netCoreConfiguration->getUnconfirmedTransactionsSearchTimeInterval());
            }
        } catch (exception e) {
            LogUtil::error("在区块链网络中搜寻未确认交易出现异常",e);
        }
    }

    void UnconfirmedTransactionsSearcher::searchUnconfirmedTransactions(){
        vector<Node> nodes = nodeService->queryAllNodes();
        if(nodes.empty()){
            return;
        }

        for(Node node:nodes){
            NodeClient nodeClient(node.ip);
            GetUnconfirmedTransactionsRequest getUnconfirmedTransactionsRequest;
            unique_ptr<GetUnconfirmedTransactionsResponse> getUnconfirmedTransactionsResponse = nodeClient.getUnconfirmedTransactions(getUnconfirmedTransactionsRequest);
            if(!getUnconfirmedTransactionsResponse.get()){
                continue;
            }
            vector<TransactionDto> &transactions = getUnconfirmedTransactionsResponse->transactions;
            if(transactions .empty()){
                continue;
            }
            for(TransactionDto transaction:transactions){
                blockchainCore->getUnconfirmedTransactionDatabase()->insertTransaction(transaction);
            }
        }
    }

}