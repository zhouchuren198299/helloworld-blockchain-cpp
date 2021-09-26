//
// Created by 40906 on 2021/9/20.
//

#include "BlockchainDatabaseKeyTool.h"
#include "../../util/ByteUtil.h"
#include "../../util/StringUtil.h"


namespace BlockchainDatabaseKeyTool{

    //区块链标识：它对应的值是区块链的高度
    string BLOCKCHAIN_HEIGHT_KEY = "A";
    //区块链标识：它对应的值是区块链的交易高度
    string BLOCKCHAIN_TRANSACTION_HEIGHT_KEY = "B";
    //区块链标识：它对应的值是区块链的交易输出高度
    string BLOCKCHAIN_TRANSACTION_OUTPUT_HEIGHT_KEY = "C";

    //哈希标识：哈希(区块哈希、交易哈希)的前缀
    string HASH_PREFIX_FLAG = "D";


    //区块标识：存储区块链高度到区块的映射
    string BLOCK_HEIGHT_TO_BLOCK_PREFIX_FLAG = "E";
    //区块标识：存储区块Hash到区块高度的映射
    string BLOCK_HASH_TO_BLOCK_HEIGHT_PREFIX_FLAG = "F";


    //交易标识：存储交易高度到交易的映射
    string TRANSACTION_HEIGHT_TO_TRANSACTION_PREFIX_FLAG = "G";
    //交易标识：存储交易哈希到交易高度的映射
    string TRANSACTION_HASH_TO_TRANSACTION_HEIGHT_PREFIX_FLAG = "H";


    //交易输出标识：存储交易输出高度到交易输出的映射
    string TRANSACTION_OUTPUT_HEIGHT_TO_TRANSACTION_OUTPUT_PREFIX_FLAG = "I";
    //交易输出标识：存储交易输出ID到交易输出高度的映射
    string TRANSACTION_OUTPUT_ID_TO_TRANSACTION_OUTPUT_HEIGHT_PREFIX_FLAG = "J";
    //交易输出标识：存储交易输出ID到未花费交易输出高度的映射
    string TRANSACTION_OUTPUT_ID_TO_UNSPENT_TRANSACTION_OUTPUT_HEIGHT_PREFIX_FLAG = "K";
    //交易输出标识：存储交易输出ID到已花费交易输出高度的映射
    string TRANSACTION_OUTPUT_ID_TO_SPENT_TRANSACTION_OUTPUT_HEIGHT_PREFIX_FLAG = "L";
    //交易输出标识：存储交易输出ID到来源交易高度的映射
    string TRANSACTION_OUTPUT_ID_TO_SOURCE_TRANSACTION_HEIGHT_PREFIX_FLAG = "M";
    //交易输出标识：存储交易输出ID到花费去向交易高度的映射
    string TRANSACTION_OUTPUT_ID_TO_DESTINATION_TRANSACTION_HEIGHT_PREFIX_FLAG = "N";


    //地址标识：存储地址
    string ADDRESS_PREFIX_FLAG = "O";
    //地址标识：存储地址到交易输出高度的映射
    string ADDRESS_TO_TRANSACTION_OUTPUT_HEIGHT_PREFIX_FLAG = "P";
    //地址标识：存储地址到未花费交易输出高度的映射
    string ADDRESS_TO_UNSPENT_TRANSACTION_OUTPUT_HEIGHT_PREFIX_FLAG = "Q";
    //地址标识：存储地址到已花费交易输出高度的映射
    string ADDRESS_TO_SPENT_TRANSACTION_OUTPUT_HEIGHT_PREFIX_FLAG = "R";


    //截止标识
    string END_FLAG = "#" ;
    //竖线分隔符
    string VERTICAL_LINE_FLAG = "|" ;



    //拼装数据库Key的值
    vector<unsigned char> buildBlockchainHeightKey() {
        string stringKey = BLOCKCHAIN_HEIGHT_KEY + END_FLAG;
        return ByteUtil::stringToUtf8Bytes(stringKey);
    }
    vector<unsigned char> buildHashKey(string hash) {
        string stringKey = HASH_PREFIX_FLAG + hash + END_FLAG;
        return ByteUtil::stringToUtf8Bytes(stringKey);
    }
    vector<unsigned char> buildAddressKey(string address) {
        string stringKey = ADDRESS_PREFIX_FLAG + address + END_FLAG;
        return ByteUtil::stringToUtf8Bytes(stringKey);
    }
    vector<unsigned char> buildBlockHeightToBlockKey(uint64_t blockHeight) {
        string stringKey = BLOCK_HEIGHT_TO_BLOCK_PREFIX_FLAG + StringUtil::valueOfUint64(blockHeight) + END_FLAG;
        return ByteUtil::stringToUtf8Bytes(stringKey);
    }
    vector<unsigned char> buildBlockHashToBlockHeightKey(string blockHash) {
        string stringKey = BLOCK_HASH_TO_BLOCK_HEIGHT_PREFIX_FLAG + blockHash + END_FLAG;
        return ByteUtil::stringToUtf8Bytes(stringKey);
    }
    vector<unsigned char> buildTransactionHashToTransactionHeightKey(string transactionHash) {
        string stringKey = TRANSACTION_HASH_TO_TRANSACTION_HEIGHT_PREFIX_FLAG + transactionHash + END_FLAG;
        return ByteUtil::stringToUtf8Bytes(stringKey);
    }
    vector<unsigned char> buildTransactionOutputHeightToTransactionOutputKey(uint64_t transactionOutputHeight) {
        string stringKey = TRANSACTION_OUTPUT_HEIGHT_TO_TRANSACTION_OUTPUT_PREFIX_FLAG + StringUtil::valueOfUint64(transactionOutputHeight) + END_FLAG;
        return ByteUtil::stringToUtf8Bytes(stringKey);
    }
    vector<unsigned char> buildTransactionOutputIdToTransactionOutputHeightKey(string transactionHash,uint64_t transactionOutputIndex) {
        string transactionOutputId = buildTransactionOutputId(transactionHash, transactionOutputIndex);
        string stringKey = TRANSACTION_OUTPUT_ID_TO_TRANSACTION_OUTPUT_HEIGHT_PREFIX_FLAG + transactionOutputId + END_FLAG;
        return ByteUtil::stringToUtf8Bytes(stringKey);
    }
    vector<unsigned char> buildTransactionOutputIdToUnspentTransactionOutputHeightKey(string transactionHash,uint64_t transactionOutputIndex) {
        string transactionOutputId = buildTransactionOutputId(transactionHash, transactionOutputIndex);
        string stringKey = TRANSACTION_OUTPUT_ID_TO_UNSPENT_TRANSACTION_OUTPUT_HEIGHT_PREFIX_FLAG + transactionOutputId + END_FLAG;
        return ByteUtil::stringToUtf8Bytes(stringKey);
    }
    vector<unsigned char> buildTransactionOutputIdToSourceTransactionHeightKey(string transactionHash,uint64_t transactionOutputIndex) {
        string transactionOutputId = buildTransactionOutputId(transactionHash, transactionOutputIndex);
        string stringKey = TRANSACTION_OUTPUT_ID_TO_SOURCE_TRANSACTION_HEIGHT_PREFIX_FLAG + transactionOutputId + END_FLAG;
        return ByteUtil::stringToUtf8Bytes(stringKey);
    }
    vector<unsigned char> buildTransactionOutputIdToDestinationTransactionHeightKey(string transactionHash,uint64_t transactionOutputIndex) {
        string transactionOutputId = buildTransactionOutputId(transactionHash, transactionOutputIndex);
        string stringKey = TRANSACTION_OUTPUT_ID_TO_DESTINATION_TRANSACTION_HEIGHT_PREFIX_FLAG + transactionOutputId + END_FLAG;
        return ByteUtil::stringToUtf8Bytes(stringKey);
    }
    vector<unsigned char> buildAddressToTransactionOutputHeightKey(string address) {
        string stringKey = ADDRESS_TO_TRANSACTION_OUTPUT_HEIGHT_PREFIX_FLAG + address + END_FLAG;
        return ByteUtil::stringToUtf8Bytes(stringKey);
    }
    vector<unsigned char> buildAddressToUnspentTransactionOutputHeightKey(string address) {
        string stringKey = ADDRESS_TO_UNSPENT_TRANSACTION_OUTPUT_HEIGHT_PREFIX_FLAG + address + END_FLAG;
        return ByteUtil::stringToUtf8Bytes(stringKey);
    }
    vector<unsigned char> buildAddressToSpentTransactionOutputHeightKey(string address) {
        string stringKey = ADDRESS_TO_SPENT_TRANSACTION_OUTPUT_HEIGHT_PREFIX_FLAG + address + END_FLAG;
        return ByteUtil::stringToUtf8Bytes(stringKey);
    }
    vector<unsigned char> buildBlockchainTransactionHeightKey() {
        string stringKey = BLOCKCHAIN_TRANSACTION_HEIGHT_KEY + END_FLAG;
        return ByteUtil::stringToUtf8Bytes(stringKey);
    }
    vector<unsigned char> buildBlockchainTransactionOutputHeightKey() {
        string stringKey = BLOCKCHAIN_TRANSACTION_OUTPUT_HEIGHT_KEY + END_FLAG;
        return ByteUtil::stringToUtf8Bytes(stringKey);
    }
    vector<unsigned char> buildTransactionHeightToTransactionKey(uint64_t transactionHeight) {
        string stringKey = TRANSACTION_HEIGHT_TO_TRANSACTION_PREFIX_FLAG + StringUtil::valueOfUint64(transactionHeight) + END_FLAG;
        return ByteUtil::stringToUtf8Bytes(stringKey);
    }

    vector<unsigned char> buildTransactionOutputIdToSpentTransactionOutputHeightKey(string transactionHash,uint64_t transactionOutputIndex) {
        string transactionOutputId = buildTransactionOutputId(transactionHash, transactionOutputIndex);
        string stringKey = TRANSACTION_OUTPUT_ID_TO_SPENT_TRANSACTION_OUTPUT_HEIGHT_PREFIX_FLAG + transactionOutputId + END_FLAG;
        return ByteUtil::stringToUtf8Bytes(stringKey);
    }

    string buildTransactionOutputId(string transactionHash,uint64_t transactionOutputIndex) {
        string transactionOutputId = StringUtil::concatenate3(transactionHash, VERTICAL_LINE_FLAG, StringUtil::valueOfUint64(transactionOutputIndex));
        return transactionOutputId;
    }


}