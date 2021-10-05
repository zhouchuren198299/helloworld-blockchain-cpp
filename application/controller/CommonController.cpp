//
// Created by 40906 on 2021/10/5.
//

#include "CommonController.h"


namespace controller {

    void fail(httplib::Response &res, string message) {
        string s = "{\"status\":\"fail\",\"message\":\"" + message + "\",\"data\":null" + "}";
        res.set_content(s, "text/json");
    }
    void requestParamIllegal(httplib::Response &res) {
        fail(res, REQUEST_PARAM_ILLEGAL);
    }
    void serviceUnavailable(httplib::Response &res) {
        fail(res, SERVICE_UNAVAILABLE);
    }
    void ServiceUnauthorized(httplib::Response &res) {
        fail(res, SERVICE_UNAUTHORIZED);
    }
}