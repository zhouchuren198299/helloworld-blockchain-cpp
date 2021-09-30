//
// Created by 40906 on 2021/9/12.
//

#include "Ripemd160Util.h"
#include <openssl/ripemd.h>
#include <vector>
using namespace std;


namespace Ripemd160Util{

    vector<unsigned char> digest(vector<unsigned char> input)
    {
        vector<unsigned char> result(20);
        RIPEMD160(&input[0], input.size(), &result[0]);
        return result;
    }
}