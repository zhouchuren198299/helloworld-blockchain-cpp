#include <iostream>
#include "crypto/Sha256Util.h"
#include "crypto/Ripemd160Util.h"
#include "crypto/Base58Util.h"
#include "crypto/AccountUtil.h"
#include "util/ByteUtil.h"
#include "util/TimeUtil.h"
#include <string>
#include "dto/dto.h"
#include "util/FileUtil.h"
#include "core/tool/ResourcePathTool.h"
#include "util/JsonUtil.h"
#include "core/BlockchainCore.h"
#include "core/BlockchainCoreFactory.h"
#include "util/ThreadUtil.h"
#include "util/SystemUtil.h"
#include "netcore/netcoreconfiguration/NetCoreConfiguration.h"
#include "netcore/netcoreservice/NodeService.h"
#include "netcore/netcoreserver/NodeServer.h"
#include "netcore/BlockchainNetCoreFactory.h"
#include <assert.h>

using json = nlohmann::json;

using namespace std;
using namespace dto;
using namespace core;
using namespace BlockchainCoreFactory;
using namespace netcoreconfiguration;
using namespace service;
using namespace server;
using namespace netcore;



#if 0
int main()
{
    BlockchainNetCore *blockchainNetCore = BlockchainNetCoreFactory::createBlockchainNetCore(ResourcePathTool::getTestDataRootPath());
    blockchainNetCore->start();

    ThreadUtil::millisecondSleep(10 * 60 * 60 * 1000);
}
#endif



#if 0
int main()
{
    BlockchainCore *blockchainCore = BlockchainCoreFactory::createBlockchainCore(ResourcePathTool::getTestDataRootPath());
    NetCoreConfiguration netCoreConfiguration = NetCoreConfiguration(ResourcePathTool::getTestDataRootPath());
    NodeDao nodeDao = NodeDao(&netCoreConfiguration);
    NodeService nodeService = NodeService(&nodeDao);
    NodeServer nodeServer = NodeServer(&netCoreConfiguration,blockchainCore,&nodeService);
    nodeServer.start();
}
#endif



#if 0
int main()
{
    FileUtil::deleteDirectory(ResourcePathTool::getTestDataRootPath());

    BlockchainCore *blockchainCore = BlockchainCoreFactory::createBlockchainCore(ResourcePathTool::getTestDataRootPath());
    blockchainCore->getMiner()->setMinerMineMaxBlockHeight(1L);
    blockchainCore->start();
    blockchainCore->getMiner()->active();
    //留5秒时间挖矿
    ThreadUtil::millisecondSleep(10000);

    //测试是否挖出一个区块
    unique_ptr<Block> block1 = blockchainCore->getBlockchainDatabase()->queryTailBlock();
    assert(1 == block1->height);



    Wallet *wallet = blockchainCore->getWallet();

    //收款地址
    string payeeAddress = "12F5oN7H2YspbPwUmwvdEq8uhSeKawN9Hh";
    //收款金额
    uint64_t payeeValue = uint64_t(5000000000);

    //构造收款方
    Payee payee;
    payee.value=payeeValue;
    payee.address=payeeAddress;
    vector<Payee> nonChangePayees;
    nonChangePayees.push_back(payee);
    AutoBuildTransactionRequest request;
    request.nonChangePayees=nonChangePayees;

    //构造交易
    AutoBuildTransactionResponse response = wallet->autoBuildTransaction(request);

    //将交易放入未确认交易池
    blockchainCore->getMiner()->getUnconfirmedTransactionDatabase()->insertTransaction(response.transaction);
    blockchainCore->getMiner()->setMinerMineMaxBlockHeight(2);

    //留5秒时间挖矿
    ThreadUtil::millisecondSleep(10000);


    unique_ptr<Block> block2 = blockchainCore->getBlockchainDatabase()->queryTailBlock();
    //测试是否挖出一个区块
    assert(2 == block2->height);
    //测试挖出的区块第二笔交易的交易输出是否是我们指定的收款地址
    assert(payeeAddress == block2->transactions[1].outputs[0].address);
    //测试挖出的区块第二笔交易的交易输出是否是我们指定的收款金额
    assert(payeeValue == block2->transactions[1].outputs[0].value);
    assert(blockchainCore->queryUnspentTransactionOutputByAddress(payeeAddress).get());
    assert(!blockchainCore->queryUnspentTransactionOutputByAddress(block1->transactions[0].outputs[0].address).get());

    exit(0);
}
#endif


#if 0
int main()
{
    FileUtil::deleteDirectory(ResourcePathTool::getTestDataRootPath());
    BlockchainCore *blockchainCore = BlockchainCoreFactory::createBlockchainCore(ResourcePathTool::getTestDataRootPath());
    blockchainCore->getMiner()->setMinerMineMaxBlockHeight(3);
    blockchainCore->getMiner()->active();
    blockchainCore->start();
    ThreadUtil::millisecondSleep(10000);
    Block block1 = blockchainCore->queryBlockByBlockHeight(1);
    cout << JsonUtil::toString(block1) << endl;
    Block block2 = blockchainCore->queryBlockByBlockHeight(2);
    cout << JsonUtil::toString(block2) << endl;
    Block block3 = blockchainCore->queryBlockByBlockHeight(3);
    cout << JsonUtil::toString(block3) << endl;
}
#endif


#if 0
int main()
{
    FileUtil::deleteDirectory(ResourcePathTool::getTestDataRootPath());
    BlockchainCore *blockchainCore = BlockchainCoreFactory::createBlockchainCore(ResourcePathTool::getTestDataRootPath());

    string stringBlock1 = FileUtil::read(SystemUtil::systemRootDirectory()+"\\core"+"\\test\\resources\\blocks\\block1.json");
    BlockDto blockDto1 = JsonUtil::toObject(stringBlock1, BlockDto{});
    string stringBlock2 = FileUtil::read(SystemUtil::systemRootDirectory()+"\\core"+"\\test\\resources\\blocks\\block2.json");
    BlockDto blockDto2 = JsonUtil::toObject(stringBlock2, BlockDto{});
    string stringBlock3 = FileUtil::read(SystemUtil::systemRootDirectory()+"\\core"+"\\test\\resources\\blocks\\block3.json");
    BlockDto blockDto3 = JsonUtil::toObject(stringBlock3, BlockDto{});

    string block1Hash = "e213eaae8259e1aca2044f35036ec5fc3c4370a33fa28353a749e8257e1d2e9e";
    string block2Hash = "8759b498f57e3b359759b7723850a99968f6e8b4bd8143e2ea41b3dbfbb59942";
    string block3Hash = "739f3554dae0a4d2b73142ae8be398fccc8971c9fac52baea1741f4205dc0315";


    blockchainCore->addBlockDto(&blockDto1);
    blockchainCore->addBlockDto(&blockDto2);
    blockchainCore->addBlockDto(&blockDto3);

    Block block1 = blockchainCore->queryBlockByBlockHeight(1);
    assert(block1Hash == block1.hash);
    Block block2 = blockchainCore->queryBlockByBlockHeight(2);
    assert(block2Hash == block2.hash);
    Block block3 = blockchainCore->queryBlockByBlockHeight(3);
    assert(block3Hash == block3.hash);
}
#endif

#if 0
int main()
{
    cout << ByteUtil::bytesToHexString(Sha256Util::digest(ByteUtil::hexStringToBytes("00"))) << endl;
    cout << ByteUtil::bytesToHexString(Sha256Util::digest(ByteUtil::hexStringToBytes("01"))) << endl;
    cout << ByteUtil::bytesToHexString(Sha256Util::digest(ByteUtil::hexStringToBytes("02"))) << endl;
    cout << ByteUtil::bytesToHexString(Ripemd160Util::digest(ByteUtil::hexStringToBytes("00"))) << endl;
    cout << ByteUtil::bytesToHexString(Ripemd160Util::digest(ByteUtil::hexStringToBytes("01"))) << endl;
    cout << ByteUtil::bytesToHexString(Ripemd160Util::digest(ByteUtil::hexStringToBytes("02"))) << endl;
    cout << Base58Util::encode(ByteUtil::hexStringToBytes("18EFBA81F02B8BFF148118BB58F38820642CCCC159E32254AEC4606CE6C71CC4FC124A1C7B61122BABE0576669F515A77568EED494F1E60B65DF3284269A153C36EC8D5911D77998FA530C689531")) << endl;
    cout << ByteUtil::bytesToHexString(Base58Util::decode("ovT8b5DW8RXymPeAzFPVLvMHq7rxMbHmtdUkaEHPPbMeWXqqWXBPbYwSpJF1tLefTyKkQwwMxDdgZNxxiYxqWe22SJt3LiEywZW8J7gSBJ")) << endl;
    Account account = AccountUtil::randomAccount();
    cout << "---------------------account" << endl;
    cout << account.privateKey << endl;
    cout << account.publicKey << endl;
    cout << account.publicKeyHash << endl;
    cout << account.address << endl;
    Account account2 = AccountUtil::accountFromPrivateKey(account.privateKey);
    cout << "---------------------account2" << endl;
    cout << account2.privateKey << endl;
    cout << account2.publicKey << endl;
    cout << account2.publicKeyHash << endl;
    cout << account2.address << endl;
    cout << "---------------------signature" << endl;
    string sign = AccountUtil::signature(account2.privateKey,ByteUtil::hexStringToBytes("00"));
    cout << sign << endl;
    cout << AccountUtil::verifySignature(account2.publicKey,ByteUtil::hexStringToBytes("00"),ByteUtil::hexStringToBytes(sign)) << endl;
    cout << "---------------------TimeUtil" << endl;
    cout << TimeUtil::millisecondTimestamp() << endl;
    cout << TimeUtil::formatMillisecondTimestamp(TimeUtil::millisecondTimestamp()) << endl;

    string stringBlock1 = FileUtil::read(SystemUtil::systemRootDirectory()+"\\core"+"\\test\\resources\\blocks\\block1.json");
    dto::BlockDto blockDto = JsonUtil::toObject(stringBlock1,dto::BlockDto{});
    cout << "blockDto..." << JsonUtil::toString(blockDto) << endl;
}
#endif