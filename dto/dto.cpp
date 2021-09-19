//
// Created by 40906 on 2021/9/19.
//

#include "dto.h"


void dto::to_json(json& j, const TransactionInputDto& p){
    j = json{
            { "transactionHash", p.transactionHash },
            { "transactionOutputIndex", p.transactionOutputIndex },
            { "inputScript", p.inputScript }
    };
}
void dto::from_json(const json& j, TransactionInputDto& p){
    j.at("transactionHash").get_to(p.transactionHash);
    j.at("transactionOutputIndex").get_to(p.transactionOutputIndex);
    j.at("inputScript").get_to(p.inputScript);
}

void dto::to_json(json& j, const TransactionOutputDto& p){
    j = json{
            { "outputScript", p.outputScript },
            { "value", p.value }
    };
}
void dto::from_json(const json& j, TransactionOutputDto& p){
    j.at("outputScript").get_to(p.outputScript);
    j.at("value").get_to(p.value);
}

void dto::to_json(json& j, const TransactionDto& p){
    j = json{
            { "inputs", p.inputs },
            { "outputs", p.outputs }
    };
}
void dto::from_json(const json& j, TransactionDto& p){
    j.at("inputs").get_to(p.inputs);
    j.at("outputs").get_to(p.outputs);
}

void dto::to_json(json& j, const BlockDto& p){
    j = json{
            { "timestamp", p.timestamp },
            { "previousHash", p.previousHash },
            { "transactions", p.transactions },
            { "nonce", p.nonce }
    };
}
void dto::from_json(const json& j, BlockDto& p){
    j.at("timestamp").get_to(p.timestamp);
    j.at("previousHash").get_to(p.previousHash);
    j.at("transactions").get_to(p.transactions);
    j.at("nonce").get_to(p.nonce);
}
