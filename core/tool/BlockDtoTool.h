//
// Created by 40906 on 2021/9/18.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_BLOCKDTOTOOL_H
#define HELLOWORLD_BLOCKCHAIN_CPP_BLOCKDTOTOOL_H


#include "../../dto/dto.h"

using namespace std;


namespace BlockDtoTool {

    string calculateBlockHash(dto::BlockDto blockDto);
    string calculateBlockMerkleTreeRoot(dto::BlockDto blockDto);
    bool isBlockEquals(dto::BlockDto block1, dto::BlockDto block2);
};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_BLOCKDTOTOOL_H
