//
// Created by 40906 on 2021/9/20.
//

#include "Wallet.h"
#include "tool/ScriptDtoTool.h"
#include "tool/TransactionDtoTool.h"
#include "../util/ByteUtil.h"
#include "../util/FileUtil.h"
#include "../util/EncodeDecodeTool.h"
#include "../util/StringUtil.h"


namespace core{

    const string WALLET_DATABASE_NAME = "WalletDatabase";


    Wallet::Wallet(core::CoreConfiguration *coreConfiguration, core::BlockchainDatabase *blockchainDatabase) {
        this->coreConfiguration = coreConfiguration;
        this->blockchainDatabase = blockchainDatabase;
    }


    vector<Account> Wallet::getAllAccounts() {
        vector<Account> accounts;
        //获取所有
        vector<vector<unsigned char>> bytesAccounts = KvDbUtil::gets(getWalletDatabasePath(),1,100000000);
        if(!bytesAccounts.empty()){
            for(vector<unsigned char> &bytesAccount:bytesAccounts){
                Account account = EncodeDecodeTool::decode(bytesAccount,Account{});
                accounts.push_back(account);
            }
        }
        return accounts;
    }


    vector<Account> Wallet::getNonZeroBalanceAccounts() {
        vector<Account> accounts;
        //获取所有
        vector<vector<unsigned char>> bytesAccounts = KvDbUtil::gets(getWalletDatabasePath(),1,100000000);
        if(!bytesAccounts.empty()){
            for(vector<unsigned char> &bytesAccount:bytesAccounts){
                Account account = EncodeDecodeTool::decode(bytesAccount,Account{});
                unique_ptr<TransactionOutput> utxo = blockchainDatabase->queryUnspentTransactionOutputByAddress(account.address);
                if(utxo.get() && utxo->value > 0){
                    accounts.push_back(account);
                }
            }
        }
        return accounts;
    }


    Account Wallet::createAccount() {
        return AccountUtil::randomAccount();
    }


    Account Wallet::createAndSaveAccount() {
        Account account = createAccount();
        saveAccount(account);
        return account;
    }


    void Wallet::saveAccount(Account account) {
        KvDbUtil::put(getWalletDatabasePath(),getKeyByAccount(account), EncodeDecodeTool::encode(account));
    }


    void Wallet::deleteAccountByAddress(string address) {
        KvDbUtil::delete0(getWalletDatabasePath(),getKeyByAddress(address));
    }


    uint64_t Wallet::getBalanceByAddress(string address) {
        unique_ptr<TransactionOutput> utxo = blockchainDatabase->queryUnspentTransactionOutputByAddress(address);
        if(utxo.get()){
            return utxo->value;
        }
        return 0L;
    }


    AutoBuildTransactionResponse Wallet::autoBuildTransaction(AutoBuildTransactionRequest request) {
        //校验[非找零]收款方
        vector<Payee> nonChangePayees = request.nonChangePayees;
        if(nonChangePayees.empty()){
            AutoBuildTransactionResponse response;
            response.buildTransactionSuccess=false;
            response.message=PayAlert::PAYEE_CAN_NOT_EMPTY;
            return response;
        }
        for(Payee payee : nonChangePayees){
            if(StringUtil::isEmpty(payee.address)){
                AutoBuildTransactionResponse response;
                response.buildTransactionSuccess=false;
                response.message=PayAlert::PAYEE_ADDRESS_CAN_NOT_EMPTY;
                return response;
            }
            if(payee.value <= 0){
                AutoBuildTransactionResponse response;
                response.buildTransactionSuccess=false;
                response.message=PayAlert::PAYEE_VALUE_CAN_NOT_LESS_EQUAL_THAN_ZERO;
                return response;
            }
        }

        //创建付款方
        vector<Payer> payers;
        //遍历钱包里的账户,用钱包里的账户付款
        vector<Account> allAccounts = getNonZeroBalanceAccounts();
        if(!allAccounts.empty()){
            for(Account account:allAccounts){
                unique_ptr<TransactionOutput> utxo = blockchainDatabase->queryUnspentTransactionOutputByAddress(account.address);
                //构建一个新的付款方
                Payer payer;
                payer.privateKey=account.privateKey;
                payer.address=account.address;
                payer.transactionHash=utxo->transactionHash;
                payer.transactionOutputIndex=utxo->transactionOutputIndex;
                payer.value=utxo->value;
                payers.push_back(payer);
                //设置默认手续费
                uint64_t fee = 0L;
                bool haveEnoughMoneyToPay0 = haveEnoughMoneyToPay(payers,nonChangePayees,fee);
                if(haveEnoughMoneyToPay0){
                    //创建一个找零账户，并将找零账户保存在钱包里。
                    Account changeAccount = createAndSaveAccount();
                    //创建一个找零收款方
                    unique_ptr<Payee> changePayee = createChangePayee(payers,nonChangePayees,changeAccount.address,fee);
                    //创建收款方(收款方=[非找零]收款方+[找零]收款方)
                    vector<Payee> payees(nonChangePayees);
                    if(changePayee.get()){
                        payees.push_back(*changePayee);
                    }
                    //构造交易
                    TransactionDto transactionDto = buildTransaction(payers,payees);
                    AutoBuildTransactionResponse response;
                    response.buildTransactionSuccess=true;
                    response.message=PayAlert::BUILD_TRANSACTION_SUCCESS;
                    response.transaction=transactionDto;
                    response.transactionHash=TransactionDtoTool::calculateTransactionHash(transactionDto);
                    response.fee=fee;
                    response.payers=payers;
                    response.nonChangePayees=nonChangePayees;
                    if(changePayee.get()){
                        response.changePayee=*changePayee;
                    }
                    response.payees=payees;
                    return response;
                }
            }
        }
        AutoBuildTransactionResponse response;
        response.message=PayAlert::NOT_ENOUGH_MONEY_TO_PAY;
        response.buildTransactionSuccess=false;
        return response;
    }





    string Wallet::getWalletDatabasePath() {
        return FileUtil::newPath(coreConfiguration->getCorePath(), WALLET_DATABASE_NAME);
    }
    vector<unsigned char> Wallet::getKeyByAddress(string address){
        return ByteUtil::stringToUtf8Bytes(address);
    }
    vector<unsigned char> Wallet::getKeyByAccount(Account account){
        return getKeyByAddress(account.address);
    }


    bool Wallet::haveEnoughMoneyToPay(vector<Payer> payers, vector<Payee> payees, uint64_t fee) {
        //计算找零金额
        uint64_t changeValue0 = changeValue(payers,payees,fee);
        //判断是否有足够的金额去支付
        bool haveEnoughMoneyToPay = changeValue0>=0;
        return haveEnoughMoneyToPay;
    }
    unique_ptr<Payee> Wallet::createChangePayee(vector<Payer> payers, vector<Payee> payees, string changeAddress, uint64_t fee) {
        //计算找零金额
        uint64_t changeValue0 = changeValue(payers,payees,fee);
        if(changeValue0 <= 0){
            return unique_ptr<Payee>(nullptr);
        }
        //构造找零收款方
        Payee changePayee;
        changePayee.address=changeAddress;
        changePayee.value=changeValue0;
        return unique_ptr<Payee>(new Payee(changePayee));
    }
    uint64_t Wallet::changeValue(vector<Payer> payers, vector<Payee> payees, uint64_t fee) {
        //交易输入总金额
        uint64_t transactionInputValues = 0;
        for(Payer payer: payers){
            transactionInputValues += payer.value;
        }
        //收款方收款总金额
        uint64_t payeeValues = 0;
        if(!payees.empty()){
            for(Payee payee : payees){
                payeeValues += payee.value;
            }
        }
        //计算找零金额，找零金额=交易输入金额-收款方交易输出金额-交易手续费
        uint64_t changeValue = transactionInputValues -  payeeValues - fee;
        return changeValue;
    }
    TransactionDto Wallet::buildTransaction(vector<Payer> payers, vector<Payee> payees) {
        //构建交易输入
        vector<TransactionInputDto> transactionInputs;
        for(Payer payer: payers){
            TransactionInputDto transactionInput;
            transactionInput.transactionHash=payer.transactionHash;
            transactionInput.transactionOutputIndex=payer.transactionOutputIndex;
            transactionInputs.push_back(transactionInput);
        }
        //构建交易输出
        vector<TransactionOutputDto> transactionOutputs;
        //构造收款方交易输出
        if(!payees.empty()){
            for(Payee payee : payees){
                TransactionOutputDto transactionOutput;
                OutputScriptDto outputScript = ScriptDtoTool::createPayToPublicKeyHashOutputScript(payee.address);
                transactionOutput.value=payee.value;
                transactionOutput.outputScript=outputScript;
                transactionOutputs.push_back(transactionOutput);
            }
        }
        //构造交易
        TransactionDto transaction;
        transaction.inputs=transactionInputs;
        transaction.outputs=transactionOutputs;
        //签名
        for(int i=0; i<transaction.inputs.size(); i++){
            TransactionInputDto &transactionInput = transaction.inputs[i];
            Account account = AccountUtil::accountFromPrivateKey(payers[i].privateKey);
            string signature = TransactionDtoTool::signature(account.privateKey,transaction);
            InputScriptDto inputScript = ScriptDtoTool::createPayToPublicKeyHashInputScript(signature, account.publicKey);
            transactionInput.inputScript=inputScript;
        }
        return transaction;
    }
}
