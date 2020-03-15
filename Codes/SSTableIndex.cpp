#include "SSTableIndex.h"

SSTableIndex::SSTableIndex(): entrCnt(0), byteCnt(0) {}

void SSTableIndex::add(uint64_t key, uint64_t len) {
    keys.push_back(key);
    offsets.push_back(byteCnt);
    ++entrCnt;
    byteCnt += len;
}

uint64_t SSTableIndex::find(uint64_t key, bool &success) {
    uint64_t left = 0;
    uint64_t right = entrCnt;
    while (right - left > 2) {
        uint64_t mid = left + ((right - left) >> 1);
        if (keys[mid] < key)
            left = mid;
        else if (keys[mid] > key)
            right = mid;
        else {
            success = true;
            return mid;
        }
    }
    if (keys[left] == key) {
        success = true;
        return left;
    } else if (keys[right - 1] == key) {
        success = true;
        return right - 1;
    } else {
        success = false;
        return 0;
    }
}