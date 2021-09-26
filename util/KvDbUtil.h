//
// Created by 40906 on 2021/9/12.
//

#ifndef HELLOWORLD_BLOCKCHAIN_CPP_KVDBUTIL_H
#define HELLOWORLD_BLOCKCHAIN_CPP_KVDBUTIL_H

#include <string>
#include <vector>
#include <mutex>

using namespace std;


namespace KvDbUtil {

    enum KvWriteAction {
        ADD,DELETE
    };
    class KvWrite{
    public:
        KvWriteAction kvWriteAction;
        vector<unsigned char> key;
        vector<unsigned char> value;
    };
    class KvWriteBatch{
    public:
        vector<KvWrite> kvWrites;
        void put(vector<unsigned char> key, vector<unsigned char> value);
        void delete0(vector<unsigned char> key);
    };


    void put(string dbPath, vector<unsigned char> bytesKey, vector<unsigned char> bytesValue);
    // TODO delete is cpp keyword , so named delete0
    void delete0(string dbPath, vector<unsigned char> bytesKey);
    vector<unsigned char> get(string dbPath, vector<unsigned char> bytesKey);
    vector<vector<unsigned char>> gets(string dbPath, uint64_t from, uint64_t size) ;
    void write(string dbPath, KvWriteBatch kvWriteBatch);

};


#endif //HELLOWORLD_BLOCKCHAIN_CPP_KVDBUTIL_H
