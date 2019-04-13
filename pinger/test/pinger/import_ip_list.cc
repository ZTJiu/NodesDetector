/******************************************
* File  : leveldb_test.cc
* Date  : 2017-08-01
* Author: Zhang Tianjiu
* Email : zhangtianjiu@vip.qq.com
*******************************************/
#include <assert.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <arpa/inet.h>
#include "leveldb/db.h"

static void split_string(const std::string ip_data, char delim,
        std::vector<std::string> & results) {
    std::stringstream ss;
    ss.str(ip_data);
    std::string item;
    while (std::getline(ss, item, ',')) {
        results.emplace_back(item);
    }
}

using namespace std;

int main(int argc,char * argv[])
{
    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;
    std::string dbpath = "/tmp/MiIpChinaLib";
    leveldb::Status status = leveldb::DB::Open(options, dbpath, &db);
    if (!status.ok()) {
        std::cout << status.ToString() << std::endl;
        return -1;
    }
    std::cout << "Open db OK" << std::endl;

    //std::fstream infile("/Users/zhangtianjiu/MiData/mi_china.txt");
    std::fstream infile("./mi_china.txt");
    std::string ip_data;
    while (infile >> ip_data) {
        std::cout << "Raw ip data line:\t" << ip_data << std::endl;
        std::vector<std::string> elems;
        split_string(ip_data, ',', elems);
        if (elems.size() < 2) {
            std::cout << "bad raw data:\t" << ip_data << std::endl;
            continue;
        }
        uint32_t ip_start = 0;
        uint32_t ip_end = 0;
        try {
            ip_start = std::stoul(elems[0]);
            ip_end = std::stoul(elems[1]);
        } catch (...) {
            std::cout << "bad format: " << elems[0] << "\t" << elems[1] << std::endl;
            continue;
        }

        std::cout << ip_start << "," << ip_end << std::endl;
        break;
        std::string ip_str;
        struct in_addr in_addr;
        if (inet_aton(ip_str.c_str(), &in_addr) == 0) {
            std::cout << "invalid ip: " << ip_str << std::endl;
        } else {
            std::cout << ip_str << " : " << in_addr.s_addr << std::endl;
            std::string key(ip_str);
            std::string value(std::to_string(in_addr.s_addr));
            leveldb::Status s;
            s = db->Put(leveldb::WriteOptions(), key, value);
        }
    }

    std::cout << "Read all:" << std::endl;
    leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        cout << it->key().ToString() << ": "  << it->value().ToString() << endl;
    }
    assert(it->status().ok());  // Check for any errors found during the scan
    delete it;

    delete db;

    return 0;
}
