//
// Created by XINGKAICHUN on 2021/9/30.
//

#include "NullsTool.h"


namespace NullsTool {


    string NULL_FLAG = "1234";


    BlockDto newNullBlockDto(){
        BlockDto blockDto{};
        blockDto.previousHash = NULL_FLAG;
        return blockDto;
    }
    bool isNullBlockDto(BlockDto blockDto){
        return blockDto.previousHash == NULL_FLAG;
    }

    GetBlockResponse newNullGetBlockResponse(){
        GetBlockResponse getBlockResponse{};
        getBlockResponse.block.previousHash = NULL_FLAG;
        return getBlockResponse;
    }
    bool isNullBlockGetBlockResponse(GetBlockResponse getBlockResponse){
        return getBlockResponse.block.previousHash == NULL_FLAG;
    }
};