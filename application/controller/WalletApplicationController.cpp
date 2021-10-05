//
// Created by 40906 on 2021/10/5.
//

#include "WalletApplicationController.h"
#include "CommonController.h"
#include "../vo/accountvo.h"
#include "../../util/LogUtil.h"
#include "../../util/StringUtil.h"

using namespace vo;



namespace controller{


        WalletApplicationController::WalletApplicationController(BlockchainNetCore *blockchainNetCore,WalletApplicationService *walletApplicationService){
            this->blockchainNetCore = blockchainNetCore;
            this->walletApplicationService = walletApplicationService;
        }
        void WalletApplicationController::createAccount(const httplib::Request &req, httplib::Response &res){
            try {
                Account account = AccountUtil::randomAccount();
                AccountVo accountVo;
                accountVo.privateKey = account.privateKey;
                accountVo.publicKey = account.publicKey;
                accountVo.publicKeyHash = account.publicKeyHash;
                accountVo.address = account.address;
                CreateAccountResponse response;
                response.account=accountVo;
                success(res,response);
            } catch (exception e){
                string message = "生成账户失败";
                LogUtil::error(message,e);
                return serviceUnavailable(res);
            }
        }
        void WalletApplicationController::createAndSaveAccount(const httplib::Request &req, httplib::Response &res){
            try {
                Account account = blockchainNetCore->getBlockchainCore()->getWallet()->createAndSaveAccount();
                AccountVo accountVo;
                accountVo.privateKey = account.privateKey;
                accountVo.publicKey = account.publicKey;
                accountVo.publicKeyHash = account.publicKeyHash;
                accountVo.address = account.address;
                
                CreateAndSaveAccountResponse response;
                response.account=accountVo;
                success(res,response);
            } catch (exception e){
                string message = "[生成账户并保存]失败";
                LogUtil::error(message,e);
                serviceUnavailable(res);
            }
        }
        void WalletApplicationController::saveAccount(const httplib::Request &req, httplib::Response &res){
            try {
                SaveAccountRequest request = JsonUtil::toObject(req.body, SaveAccountRequest{});

                string privateKey = request.privateKey;
                if(StringUtil::isEmpty(privateKey)){
                    requestParamIllegal(res);
                }
                Account account = AccountUtil::accountFromPrivateKey(privateKey);
                blockchainNetCore->getBlockchainCore()->getWallet()->saveAccount(account);
                SaveAccountResponse response;
                success(res,response);
            } catch (exception e){
                string message = "新增账户失败";
                LogUtil::error(message,e);
                serviceUnavailable(res);
            }
        }
        void WalletApplicationController::deleteAccount(const httplib::Request &req, httplib::Response &res){
            try {
                DeleteAccountRequest request = JsonUtil::toObject(req.body, DeleteAccountRequest{});

                string address = request.address;
                if(StringUtil::isEmpty(address)){
                    requestParamIllegal(res);
                }
                blockchainNetCore->getBlockchainCore()->getWallet()->deleteAccountByAddress(address);
                DeleteAccountResponse response;
                success(res,response);
            } catch (exception e){
                string message = "删除账号失败";
                LogUtil::error(message,e);
                serviceUnavailable(res);
            }
        }
        void WalletApplicationController::queryAllAccounts(const httplib::Request &req, httplib::Response &res){
            try {
                Wallet *wallet = blockchainNetCore->getBlockchainCore()->getWallet();
                vector<Account> allAccounts = wallet->getAllAccounts();

                vector<AccountVo2> accountVos;
                if(!allAccounts.empty()){
                    for(Account account:allAccounts){
                        AccountVo2 accountVo;
                        accountVo.address=account.address;
                        accountVo.privateKey=account.privateKey;
                        accountVo.value=wallet->getBalanceByAddress(account.address);
                        accountVos.push_back(accountVo);
                    }
                }

                long balance = 0;
                for(AccountVo2 accountVo : accountVos){
                    balance += accountVo.value;
                }

                QueryAllAccountsResponse response;
                response.accounts=accountVos;
                response.balance=balance;
                success(res,response);
            } catch (exception e){
                string message = "[查询所有账户]失败";
                LogUtil::error(message,e);
                serviceUnavailable(res);
            }
        }
        void WalletApplicationController::automaticBuildTransaction(const httplib::Request &req, httplib::Response &res){
            try {
                AutomaticBuildTransactionRequest request = JsonUtil::toObject(req.body, AutomaticBuildTransactionRequest{});

                //构建交易
                AutomaticBuildTransactionResponse autoBuildTransactionResponse = walletApplicationService->automaticBuildTransaction(request);
                if(autoBuildTransactionResponse.buildTransactionSuccess){
                    success(res,autoBuildTransactionResponse);
                }else {
                    fail(res,autoBuildTransactionResponse.message);
                }
            } catch (exception e){
                string message = "构建交易失败";
                LogUtil::error(message,e);
                serviceUnavailable(res);
            }
        }
        void WalletApplicationController::submitTransactionToBlockchainNetwork(const httplib::Request &req, httplib::Response &res){
            try {
                SubmitTransactionToBlockchainNetworkRequest request = JsonUtil::toObject(req.body, SubmitTransactionToBlockchainNetworkRequest{});

                SubmitTransactionToBlockchainNetworkResponse response = walletApplicationService->submitTransactionToBlockchainNetwork(request);
                success(res,response);
            } catch (exception e){
                string message = "提交交易到区块链网络失败";
                LogUtil::error(message,e);
                serviceUnavailable(res);
            }
        }
}
