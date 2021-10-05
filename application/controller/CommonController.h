//
// Created by 40906 on 2021/10/5.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_COMMONCONTROLLER_H
#define HELLOWORLD_BLOCKCHAIN_CPP_COMMONCONTROLLER_H

#include "../../thirdpart/httplib/httplib.h"
#include "../../util/JsonUtil.h"
#include <string>
using namespace std;



namespace controller {


    const string REQUEST_PARAM_ILLEGAL = "request_param_illegal";
    const string SERVICE_UNAVAILABLE = "service_unavailable";
    const string SERVICE_UNAUTHORIZED = "service_unauthorized";

    const string NOT_FOUND_TRANSACTION = "not_found_transaction";
    const string NOT_FOUND_BLOCK = "not_found_block";
    const string NOT_FOUND_UNCONFIRMED_TRANSACTIONS = "not_found_unconfirmed_transactions";

    template <typename T>
    void success(httplib::Response &res, T const& response) {
        string s = "{\"status\":\"success\",\"message\":null,\"data\":" + JsonUtil::toString(response) + "}";
        res.set_content(s, "text/json");
    }
    void fail(httplib::Response &res, string message) ;
    void requestParamIllegal(httplib::Response &res) ;
    void serviceUnavailable(httplib::Response &res) ;
    void ServiceUnauthorized(httplib::Response &res) ;
};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_COMMONCONTROLLER_H
