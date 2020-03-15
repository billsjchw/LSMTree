#ifndef SSTABLE_INDEX_H
#define SSTABLE_INDEX_H

#include <cstdint>
#include <cstddef>

class SSTableIndex {
public:
    explicit SSTableIndex();
    ~SSTableIndex();
    SSTableIndex(const SSTableIndex &other);
    void add(uint64_t key, uint64_t len);
    uint64_t find(uint64_t key, bool &success);
private:
    uint64_t entrCnt;
    uint64_t byteCnt;
    uint64_t *keys;
    uint64_t *offsets;
    size_t arrSize;
    void enlargeArray();
};

#endif