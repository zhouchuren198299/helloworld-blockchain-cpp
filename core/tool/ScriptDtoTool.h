//
// Created by 40906 on 2021/9/18.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_SCRIPTDTOTOOL_H
#define HELLOWORLD_BLOCKCHAIN_CPP_SCRIPTDTOTOOL_H


#include "../../dto/dto.h"

namespace ScriptDtoTool {
    vector<unsigned char> bytesInputScript(dto::InputScriptDto inputScript);
    vector<unsigned char> bytesOutputScript(dto::OutputScriptDto outputScript);
    vector<unsigned char> bytesScript(dto::ScriptDto script);
};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_SCRIPTDTOTOOL_H
