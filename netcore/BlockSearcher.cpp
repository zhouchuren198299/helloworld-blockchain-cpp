//
// Created by XINGKAICHUN on 2021/9/30.
//

#include "BlockSearcher.h"
#include "../util/LogUtil.h"
#include "../util/ThreadUtil.h"
#include "../core/tool/BlockTool.h"
#include "../core/tool/Model2DtoTool.h"
#include "../core/tool/BlockDtoTool.h"
#include "../netcoreclient/NodeClient.h"
#include "../util/StringUtil.h"
#include "../setting/GenesisBlockSetting.h"

using namespace netcoreclient;


namespace netcore{

    BlockSearcher::BlockSearcher(NetCoreConfiguration *netCoreConfiguration, BlockchainCore *blockchainCore
            , BlockchainCore *slaveBlockchainCore, NodeService *nodeService){
        this->netCoreConfiguration = netCoreConfiguration;
        this->nodeService = nodeService;
        this->blockchainCore = blockchainCore;
        this->slaveBlockchainCore = slaveBlockchainCore;
    }


    void BlockSearcher::start() {
        try {
            while (true){
                searchNodesBlocks();
                ThreadUtil::millisecondSleep(netCoreConfiguration->getBlockSearchTimeInterval());
            }
        } catch (exception e) {
            LogUtil::error("在区块链网络中同步节点的区块出现异常",e);
        }
    }

    /**
     * 搜索新的区块，并同步这些区块到本地区块链系统
     */
    void BlockSearcher::searchNodesBlocks() {
        vector<Node> nodes = nodeService->queryAllNodes();
        if(nodes.empty()){
            return;
        }

        for(Node node:nodes){
            searchNodeBlocks(blockchainCore,slaveBlockchainCore,node);
        }
    }

    /**
     * 搜索新的区块，并同步这些区块到本地区块链系统
     */
    void BlockSearcher::searchNodeBlocks(BlockchainCore *masterBlockchainCore, BlockchainCore *slaveBlockchainCore, Node node) {
        if(!netCoreConfiguration->isAutoSearchBlock()){
            return;
        }
        uint64_t masterBlockchainHeight = masterBlockchainCore->queryBlockchainHeight();
        //本地区块链高度大于等于远程节点区块链高度，此时远程节点没有可以同步到本地区块链的区块。
        if(masterBlockchainHeight >= node.blockchainHeight){
            return;
        }
        //本地区块链与node区块链是否分叉？
        boolean fork = isForkNode(masterBlockchainCore,node);
        if(fork){
            boolean isHardFork = isHardForkNode(masterBlockchainCore,node);
            if(!isHardFork){
                //求分叉区块的高度
                uint64_t forkBlockHeight = getForkBlockHeight(masterBlockchainCore,node);
                //复制"主区块链核心"的区块至"从区块链核心"
                duplicateBlockchainCore(masterBlockchainCore, slaveBlockchainCore);
                //删除"从区块链核心"已分叉区块
                slaveBlockchainCore->deleteBlocks(forkBlockHeight);
                //同步远程节点区块至从区块链核心
                synchronizeBlocks(slaveBlockchainCore,node,forkBlockHeight);
                //同步从区块链核心的区块至主区块链核心
                promoteBlockchainCore(slaveBlockchainCore, masterBlockchainCore);
            }
        } else {
            //未分叉，同步远程节点区块至主区块链核心
            uint64_t nextBlockHeight = masterBlockchainCore->queryBlockchainHeight()+1;
            synchronizeBlocks(masterBlockchainCore,node,nextBlockHeight);
        }
    }

    /**
     * 复制区块链核心的区块，操作完成后，'来源区块链核心'区块数据不发生变化，'去向区块链核心'的区块数据与'来源区块链核心'的区块数据保持一致。
     * @param fromBlockchainCore 来源区块链核心
     * @param toBlockchainCore 去向区块链核心
     */
    void BlockSearcher::duplicateBlockchainCore(BlockchainCore *fromBlockchainCore,BlockchainCore *toBlockchainCore) {
        //删除'去向区块链核心'区块
        while (true){
            unique_ptr<Block> toBlockchainTailBlock = toBlockchainCore->queryTailBlock() ;
            if(!toBlockchainTailBlock.get()){
                break;
            }
            unique_ptr<Block> fromBlockchainBlock = fromBlockchainCore->queryBlockByBlockHeight(toBlockchainTailBlock->height);
            if(BlockTool::isBlockEquals(fromBlockchainBlock.get(),toBlockchainTailBlock.get())){
                break;
            }
            toBlockchainCore->deleteTailBlock();
        }
        //增加'去向区块链核心'区块
        while (true){
            uint64_t toBlockchainHeight = toBlockchainCore->queryBlockchainHeight();
            unique_ptr<Block> nextBlock = fromBlockchainCore->queryBlockByBlockHeight(toBlockchainHeight+1) ;
            if(!nextBlock.get()){
                break;
            }
            toBlockchainCore->addBlock(nextBlock.get());
        }
    }
    /**
     * 增加"去向区块链核心"的区块，操作完成后，"来源区块链核心"的区块不发生变化，"去向区块链核心"的高度不变或者增长。
     * @param fromBlockchainCore "来源区块链核心"
     * @param toBlockchainCore "去向区块链核心"
     */
    void BlockSearcher::promoteBlockchainCore(BlockchainCore *fromBlockchainCore, BlockchainCore *toBlockchainCore) {
        //此时，"去向区块链核心高度"大于"来源区块链核心高度"，"去向区块链核心高度"不能增加，结束逻辑。
        if(toBlockchainCore->queryBlockchainHeight() >= fromBlockchainCore->queryBlockchainHeight()){
            return;
        }
        //硬分叉
        if(isHardFork(toBlockchainCore,fromBlockchainCore)){
            return;
        }
        //此时，"去向区块链核心高度"小于"来源区块链核心高度"，且未硬分叉，可以增加"去向区块链核心高度"
        duplicateBlockchainCore(fromBlockchainCore,toBlockchainCore);
    }

    uint64_t BlockSearcher::getForkBlockHeight(BlockchainCore *blockchainCore, Node node) {
        //求分叉区块的高度，此时已知分叉了，从当前高度依次递减1，判断高度相同的区块的是否相等，若相等，(高度+1)即开始分叉高度。
        uint64_t masterBlockchainHeight = blockchainCore->queryBlockchainHeight();
        uint64_t forkBlockHeight = masterBlockchainHeight;
        while (true) {
            if (forkBlockHeight <= GenesisBlockSetting::HEIGHT) {
                break;
            }
            GetBlockRequest getBlockRequest{};
            getBlockRequest.blockHeight=forkBlockHeight;
            NodeClient nodeClient(node.ip);
            unique_ptr<GetBlockResponse> getBlockResponse = nodeClient.getBlock(getBlockRequest);
            if(!getBlockResponse.get()){
                break;
            }
            BlockDto &remoteBlock = getBlockResponse->block;
/* TODO           if(NullUtil::isNullBlockDto(remoteBlock)){
                break;
            }*/
            unique_ptr<Block> localBlock = blockchainCore->queryBlockByBlockHeight(forkBlockHeight);
            BlockDto blockDtoTemp = Model2DtoTool::block2BlockDto(localBlock.get());
            if(BlockDtoTool::isBlockEquals(&blockDtoTemp,&remoteBlock)){
                break;
            }
            forkBlockHeight--;
        }
        forkBlockHeight++;
        return forkBlockHeight;
    }

    void BlockSearcher::synchronizeBlocks(BlockchainCore *blockchainCore, Node node, uint64_t startBlockHeight) {
        while (true){
            GetBlockRequest getBlockRequest;
            getBlockRequest.blockHeight=startBlockHeight;
            NodeClient nodeClient(node.ip);
            unique_ptr<GetBlockResponse> getBlockResponse = nodeClient.getBlock(getBlockRequest);
            if(!getBlockResponse.get()){
                break;
            }
            BlockDto remoteBlock = getBlockResponse->block;
/*TODO            if(NullUtil::isNullBlockDto(remoteBlock)){
                break;
            }*/
            boolean isAddBlockSuccess = blockchainCore->addBlockDto(&remoteBlock);
            if(!isAddBlockSuccess){
                break;
            }
            startBlockHeight++;
        }
    }

    bool BlockSearcher::isForkNode(BlockchainCore *blockchainCore, Node node) {
        unique_ptr<Block> block = blockchainCore->queryTailBlock();
        if(!block.get()){
            return false;
        }
        GetBlockRequest getBlockRequest;
        getBlockRequest.blockHeight=block->height;
        NodeClient nodeClient(node.ip);
        unique_ptr<GetBlockResponse> getBlockResponse = nodeClient.getBlock(getBlockRequest);
        //没有查询到区块，这里则认为远程节点没有该高度的区块存在，远程节点的高度没有本地区块链高度高，所以不分叉。
        if(!getBlockResponse.get()){
            return false;
        }
        BlockDto &blockDto = getBlockResponse->block;
/*TODO        if(NullUtil::isNullBlockDto(blockDto)){
            return false;
        }*/
        string blockHash = BlockDtoTool::calculateBlockHash(&blockDto);
        return !StringUtil::equals(block->hash, blockHash);
    }

    bool BlockSearcher::isHardFork(BlockchainCore *blockchainCore1, BlockchainCore *blockchainCore2) {
        BlockchainCore *longer;
        BlockchainCore *shorter;
        if(blockchainCore1->queryBlockchainHeight()>=blockchainCore2->queryBlockchainHeight()){
            longer = blockchainCore1;
            shorter = blockchainCore2;
        }else {
            longer = blockchainCore2;
            shorter = blockchainCore1;
        }

        uint64_t shorterBlockchainHeight = shorter->queryBlockchainHeight();
        if(shorterBlockchainHeight < netCoreConfiguration->getHardForkBlockCount()){
            return false;
        }

        uint64_t criticalPointBlocHeight = shorterBlockchainHeight-netCoreConfiguration->getHardForkBlockCount()+1;
        unique_ptr<Block> longerBlock = longer->queryBlockByBlockHeight(criticalPointBlocHeight);
        unique_ptr<Block> shorterBlock = shorter->queryBlockByBlockHeight(criticalPointBlocHeight);
        return !BlockTool::isBlockEquals(longerBlock.get(), shorterBlock.get());
    }

    bool BlockSearcher::isHardForkNode(BlockchainCore *blockchainCore, Node node) {
        uint64_t blockchainHeight = blockchainCore->queryBlockchainHeight();
        if (blockchainHeight < netCoreConfiguration->getHardForkBlockCount()) {
            return false;
        }
        uint64_t criticalPointBlocHeight = blockchainHeight-netCoreConfiguration->getHardForkBlockCount()+1;
        if(criticalPointBlocHeight <= GenesisBlockSetting::HEIGHT){
            return false;
        }
        GetBlockRequest getBlockRequest{};
        getBlockRequest.blockHeight=criticalPointBlocHeight;
        NodeClient nodeClient(node.ip);
        unique_ptr<GetBlockResponse> getBlockResponse = nodeClient.getBlock(getBlockRequest);
        if(!getBlockResponse.get()){
            return false;
        }
        BlockDto &remoteBlock = getBlockResponse->block;
/*
 * TODO        if(NullUtil::isNullBlockDto(remoteBlock)){
            return false;
        }*/
        unique_ptr<Block> localBlock = blockchainCore->queryBlockByBlockHeight(criticalPointBlocHeight);
        BlockDto blockDtoTemp = Model2DtoTool::block2BlockDto(localBlock.get());
        return !BlockDtoTool::isBlockEquals(&blockDtoTemp,&remoteBlock);
    }
}
