//
// Created by 40906 on 2021/10/4.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_WALLETAPPLICATIONAPI_H
#define HELLOWORLD_BLOCKCHAIN_CPP_WALLETAPPLICATIONAPI_H

#include <string>
using namespace std;



namespace WalletApplicationApi {


//生成账户(公钥、私钥、地址)
    const string CREATE_ACCOUNT = "/Api/WalletApplication/CreateAccount";

//生成账户(公钥、私钥、地址)并保存
    const string CREATE_AND_SAVE_ACCOUNT = "/Api/WalletApplication/CreateAndSaveAccount";

//新增账户
    const string SAVE_ACCOUNT = "/Api/WalletApplication/SaveAccount";

//删除账户
    const string DELETE_ACCOUNT = "/Api/WalletApplication/DeleteAccount";

//查询所有的账户
    const string QUERY_ALL_ACCOUNTS = "/Api/WalletApplication/QueryAllAccounts";

//构建交易
    const string AUTOMATIC_BUILD_TRANSACTION = "/Api/WalletApplication/AutomaticBuildTransaction";

//提交交易到区块链网络
    const string SUBMIT_TRANSACTION_TO_BLOCKCHIAIN_NEWWORK = "/Api/WalletApplication/SubmitTransactionToBlockchainNetwork";
};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_WALLETAPPLICATIONAPI_H
