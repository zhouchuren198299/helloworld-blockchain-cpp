//
// Created by 40906 on 2021/9/12.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_ACCOUNTUTIL_H
#define HELLOWORLD_BLOCKCHAIN_CPP_ACCOUNTUTIL_H
#include <string>
#include <vector>
#include "../thirdpart/nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;


struct Account{
    string privateKey;
    string publicKey;
    string publicKeyHash;
    string address;
};

void to_json(json& j, const Account& p);
void from_json(const json& j, Account& p);


namespace AccountUtil {

    Account randomAccount();
    Account accountFromPrivateKey(string privateKey);

    string publicKeyHashFromPublicKey(string publicKey);
    string publicKeyHashFromAddress(string address);

    string addressFromPublicKey(string privateKey);
    string addressFromPublicKeyHash(string publicKeyHash);

    string signature(string privateKey, vector<unsigned char> bytesMessage);
    bool verifySignature(string publicKey, vector<unsigned char> bytesMessage, vector<unsigned char> bytesSignature);

    string formatPrivateKey(string privateKey);
    bool isPayToPublicKeyHashAddress(string address);
};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_ACCOUNTUTIL_H
