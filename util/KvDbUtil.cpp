//
// Created by 40906 on 2021/9/12.
//

#include "KvDbUtil.h"
#include <leveldb/db.h>
#include <iostream>
#include "leveldb/write_batch.h"

leveldb::WriteBatch toLevelDbWriteBatch(KvWriteBatch &batch) {
    leveldb::WriteBatch writeBatch;
    vector<KvWrite> kvWrites = batch.kvWrites;
    vector<KvWrite>::iterator it = kvWrites.begin();
    for(; it != kvWrites.end(); ++it)
    {
        KvWrite kvWrite = *it;
        std::string key(kvWrite.key.begin(), kvWrite.key.end());
        std::string value(kvWrite.value.begin(), kvWrite.value.end());

        if(kvWrite.kvWriteActionEnum == KvWriteActionEnum::ADD){
            writeBatch.Put(key,value);
        } else if(kvWrite.kvWriteActionEnum == KvWriteActionEnum::DELETE){
            writeBatch.Delete(key);
        }
    }
    return writeBatch;
}


void KvDbUtil::put(string dbPath, vector<unsigned char> bytesKey, vector<unsigned char> bytesValue){

    std::string key(bytesKey.begin(), bytesKey.end());
    std::string value(bytesValue.begin(), bytesValue.end());

    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, dbPath, &db);
    db->Put(leveldb::WriteOptions(), key, value);
    delete db;
}
void KvDbUtil::delete0(string dbPath, vector<unsigned char> bytesKey){

    std::string key(bytesKey.begin(), bytesKey.end());

    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, dbPath, &db);
    db->Delete(leveldb::WriteOptions(), key);
    delete db;
}
vector<unsigned char> KvDbUtil::get(string dbPath, vector<unsigned char> bytesKey){
    std::string key(bytesKey.begin(), bytesKey.end());
    std::string value;

    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, dbPath, &db);
    db->Get(leveldb::ReadOptions(), key, &value);
    delete db;

    std::vector<unsigned char> bytesValue(value.begin(), value.end());
    return bytesValue;
}
vector<vector<unsigned char>> KvDbUtil::gets(string dbPath, long from, long size){
    vector<vector<unsigned char>> resp;
    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, dbPath, &db);
    leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        string key = it->key().ToString();
        string value = it->value().ToString();
        std::vector<unsigned char> bytesValue(value.begin(), value.end());
        resp.push_back(bytesValue);
    }
    delete it;
    delete db;
    return resp;
}
void KvDbUtil::write(string dbPath, KvWriteBatch kvWriteBatch){
    leveldb::WriteBatch batch = toLevelDbWriteBatch(kvWriteBatch);
    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, dbPath, &db);
    db->Write(leveldb::WriteOptions(), &batch);
    delete db;
}

#if 0
int main()
{
    string key = "111";
    vector<unsigned char> bytesKey(key.begin(), key.end());
    std::vector<unsigned char> bytesValue = KvDbUtil::get("E:\\logs", bytesKey);
    cout << "value is : " << string(bytesValue.begin(),bytesValue.end())<< endl;

    string value = "111";
    std::vector<unsigned char> bytesValue2(value.begin(), value.end());
    KvDbUtil::put("E:\\logs", bytesKey,bytesValue2);

    bytesValue = KvDbUtil::get("E:\\logs", bytesKey);
    cout << "value is : " <<  string(bytesValue.begin(),bytesValue.end())<< endl;
}
#endif
