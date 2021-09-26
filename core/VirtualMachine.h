//
// Created by 40906 on 2021/9/20.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_VIRTUALMACHINE_H
#define HELLOWORLD_BLOCKCHAIN_CPP_VIRTUALMACHINE_H

#include "model/model.h"
using namespace std;
using namespace model;

namespace core{

    class VirtualMachine {
    public:
        ScriptExecuteResult executeScript(Transaction transactionEnvironment, Script script);
    };
}



#endif //HELLOWORLD_BLOCKCHAIN_CPP_VIRTUALMACHINE_H
