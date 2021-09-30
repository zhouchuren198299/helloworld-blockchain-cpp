//
// Created by 40906 on 2021/9/11.
//
#include <openssl/sha.h>
#include "Sha256Util.h"
#include <vector>
using namespace std;


namespace Sha256Util{

    vector<unsigned char> digest(vector<unsigned char> input)
    {
        vector<unsigned char> result(32);
        SHA256(&input[0], input.size(), &result[0]);
        return result;
    }

    vector<unsigned char> doubleDigest(vector<unsigned char> input)
    {
        return digest(digest(input));
    }
}