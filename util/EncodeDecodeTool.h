//
// Created by 40906 on 2021/9/20.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_ENCODEDECODETOOL_H
#define HELLOWORLD_BLOCKCHAIN_CPP_ENCODEDECODETOOL_H

#include <vector>
#include "JsonUtil.h"
#include "ByteUtil.h"

using namespace std;

namespace EncodeDecodeTool {

    template <typename T>
    inline vector<unsigned char> const encode (T const object)
    {
        return ByteUtil::stringToUtf8Bytes(JsonUtil::toString(object));
    }

    template <typename T>
    inline T const decode (vector<unsigned char> bytesObject, T const& object)
    {
        return JsonUtil::toObject(ByteUtil::utf8BytesToString(bytesObject),object);
    }

};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_ENCODEDECODETOOL_H
