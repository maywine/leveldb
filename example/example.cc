#include "leveldb/env.h"
#include "leveldb/db.h"
#include "leveldb/filter_policy.h"
#include <iostream>

int main(int argc, char** argv) {
  leveldb::Options opt;
  opt.filter_policy = leveldb::NewBloomFilterPolicy(30);
  opt.create_if_missing = true;
  opt.error_if_exists = false;
  leveldb::DB* db_ptr = nullptr;
  auto s = leveldb::DB::Open(opt, "etcd", &db_ptr);
  if (!s.ok()) {
    std::cout << s.ToString() << std::endl;
  }
  leveldb::WriteOptions wop;
  for (int i = 0; i < 100; ++i) {
    auto key = std::to_string(i);
    db_ptr->Put(wop, key, key);
  }

  leveldb::ReadOptions rop;
  auto it = db_ptr->NewIterator(rop);
  it->SeekToFirst();
  while (it->Valid()) {
    std::cout << it->key().ToString() << " " << it->value().ToString()
              << std::endl;
    it->Next();
  }
  delete it;
  delete db_ptr;

  return 0;
}