//
// Created by 40906 on 2021/9/12.
//

#include <chrono>
#include <thread>
#include "ThreadUtil.h"



void ThreadUtil::millisecondSleep(uint64_t millisecond){
    chrono::milliseconds dura(millisecond);
    this_thread::sleep_for(dura);
}