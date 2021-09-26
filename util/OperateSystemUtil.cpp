//
// Created by 40906 on 2021/9/12.
//

#include "OperateSystemUtil.h"

bool OperateSystemUtil::isWindowsOperateSystem(){
    #ifdef _WIN32
        return true;
    #else
        return false;
    #endif
}

bool OperateSystemUtil::isMacOperateSystem(){
    #ifdef __APPLE__
        return true;
    #else
        return false;
    #endif
}

bool OperateSystemUtil::isLinuxOperateSystem(){
    #ifdef __linux
        return true;
    #else
        return false;
    #endif
}

