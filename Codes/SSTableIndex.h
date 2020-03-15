#ifndef SSTABLE_INDEX_H
#define SSTABLE_INDEX_H

#include <cstdint>
#include <cstddef>
#include <vector>

class SSTableController;

class SSTableIndex {
public:
    explicit SSTableIndex();
    void add(uint64_t key, uint64_t len);
    uint64_t find(uint64_t key, bool &success);
private:
    uint64_t entrCnt;
    uint64_t byteCnt;
    std::vector<uint64_t> keys;
    std::vector<uint64_t> offsets;
friend SSTableController;
};

#endif