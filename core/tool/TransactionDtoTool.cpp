//
// Created by 40906 on 2021/9/18.
//

#include "TransactionDtoTool.h"

#include "BlockDtoTool.h"
#include "../../crypto/Sha256Util.h"
#include "../../crypto/ByteUtil.h"
#include "ScriptDtoTool.h"


/**
 * 序列化。将交易转换为字节数组，要求生成的字节数组反过来能还原为原始交易。
 */
vector<unsigned char> bytesTransaction(dto::TransactionDto transactionDto, bool omitInputScript) {
    vector<vector<unsigned char>> bytesUnspentTransactionOutputs;
    vector<dto::TransactionInputDto> inputs = transactionDto.inputs;
    if(inputs.size() != 0){
        for(dto::TransactionInputDto transactionInputDto:inputs){
            vector<unsigned char>  bytesUnspentTransactionOutput;
            if(omitInputScript){
                vector<unsigned char>  bytesTransactionHash = ByteUtil::hexStringToBytes(transactionInputDto.transactionHash);
                vector<unsigned char>  bytesTransactionOutputIndex = ByteUtil::uint64ToBytes(transactionInputDto.transactionOutputIndex);
                bytesUnspentTransactionOutput = ByteUtil::concatenate(ByteUtil::concatenateLength(bytesTransactionHash),
                                                                      ByteUtil::concatenateLength(bytesTransactionOutputIndex));
            }else {
                vector<unsigned char>  bytesTransactionHash = ByteUtil::hexStringToBytes(transactionInputDto.transactionHash);
                vector<unsigned char>  bytesTransactionOutputIndex = ByteUtil::uint64ToBytes(transactionInputDto.transactionOutputIndex);
                vector<unsigned char> bytesInputScript = ScriptDtoTool::bytesInputScript(transactionInputDto.inputScript);
                bytesUnspentTransactionOutput = ByteUtil::concatenate3(ByteUtil::concatenateLength(bytesTransactionHash),
                                                                       ByteUtil::concatenateLength(bytesTransactionOutputIndex),ByteUtil::concatenateLength(bytesInputScript));
            }
            bytesUnspentTransactionOutputs.push_back(ByteUtil::concatenateLength(bytesUnspentTransactionOutput));
        }
    }

    vector<vector<unsigned char>> bytesTransactionOutputs;
    vector<dto::TransactionOutputDto> outputs = transactionDto.outputs;
    if(outputs.size() != 0){
        for(dto::TransactionOutputDto transactionOutputDto:outputs){
            vector<unsigned char>  bytesOutputScript = ScriptDtoTool::bytesOutputScript(transactionOutputDto.outputScript);
            vector<unsigned char>  bytesValue = ByteUtil::uint64ToBytes(transactionOutputDto.value);
            vector<unsigned char>  bytesTransactionOutput = ByteUtil::concatenate(ByteUtil::concatenateLength(bytesOutputScript),ByteUtil::concatenateLength(bytesValue));
            bytesTransactionOutputs.push_back(ByteUtil::concatenateLength(bytesTransactionOutput));
        }
    }

    vector<unsigned char>  data = ByteUtil::concatenate(ByteUtil::flatAndConcatenateLength(bytesUnspentTransactionOutputs),
                                                        ByteUtil::flatAndConcatenateLength(bytesTransactionOutputs));
    return data;
}

string TransactionDtoTool::calculateTransactionHash(dto::TransactionDto transactionDto){
    vector<unsigned char> bytesTransaction1 = bytesTransaction(transactionDto,false);
    vector<unsigned char> bytesTransactionHash = Sha256Util::doubleDigest(bytesTransaction1);
    return ByteUtil::bytesToHexString(bytesTransactionHash);
}
