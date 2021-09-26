//
// Created by 40906 on 2021/9/12.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_JSONUTIL_H
#define HELLOWORLD_BLOCKCHAIN_CPP_JSONUTIL_H

#include <sstream>
#include <string>
#include "../thirdpart/nlohmann/json.hpp"
using json = nlohmann::json;

using namespace std;

namespace JsonUtil {


    template <typename T>
    inline T const toObject (string jsonString, T const& object)
    {
        istringstream iss(jsonString);;
        json j;
        iss >> j;
        return j.get<T>();
    }

    template <typename T>
    inline string const toString (T const object)
    {
        const json j{object};
        const string jsonString = j[0].dump(4);
        return jsonString;
    }
};



#endif //HELLOWORLD_BLOCKCHAIN_CPP_JSONUTIL_H
