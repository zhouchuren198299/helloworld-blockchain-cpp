//
// Created by 40906 on 2021/9/12.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_STRINGUTIL_H
#define HELLOWORLD_BLOCKCHAIN_CPP_STRINGUTIL_H

#include <string>

using namespace std;


namespace StringUtil {
    const string BLANKSPACE = " ";
    bool equals(string str1, string str2);
    bool isEmpty(string str);
    string prefixPadding(string rawValue,int targetLength,string paddingValue);
    string concatenate(string str1,string str2);
    string concatenate3(string str1, string str2, string str3);
    string valueOfUint64(uint64_t number);
    uint64_t length(string value) ;
};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_STRINGUTIL_H
