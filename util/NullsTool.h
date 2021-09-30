//
// Created by XINGKAICHUN on 2021/9/30.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_NULLSTOOL_H
#define HELLOWORLD_BLOCKCHAIN_CPP_NULLSTOOL_H


#include "../dto/dto.h"

using namespace dto;


namespace NullsTool {

    BlockDto newNullBlockDto();
    bool isNullBlockDto(BlockDto blockDto);

    GetBlockResponse newNullGetBlockResponse();
    bool isNullBlockGetBlockResponse(GetBlockResponse getBlockResponse);
};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_NULLSTOOL_H
