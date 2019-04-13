/******************************************
* File  : leveldb_test.cc
* Date  : 2017-08-01
* Author: Zhang Tianjiu
* Email : zhangtianjiu@vip.qq.com
*******************************************/
#include <assert.h>
#include <iostream>
#include "leveldb/db.h"
//#include "leveldb/options.h"

using namespace std;

int main(int argc,char * argv[])
{
    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;
    std::string dbpath = "/tmp/testdb";
    leveldb::Status status = leveldb::DB::Open(options, dbpath, &db);
//	if (!status.ok()) std::cout << status.ToString() << endl;
    if (!status.ok()) {
	std::cout << status.ToString() << std::endl;
	return -1;
}
    std::string key1 = "Slogon";
    std::string key2 = "Hello world";
    cout<<"Open db OK"<<std::endl;

    std::string value;
    leveldb::Status s;
    s = db->Put(leveldb::WriteOptions(), key1, key2);
    s = db->Get(leveldb::ReadOptions(), key1, &value);

    cout<<value<<std::endl;
    delete db;

    return 0;
}
