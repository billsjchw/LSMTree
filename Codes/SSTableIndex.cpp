#include "SSTableIndex.h"

SSTableIndex::SSTableIndex(): entrCnt(0), byteCnt(0), arrSize(10) {
    keys = new uint64_t[arrSize];
    offsets = new uint64_t[arrSize];
}

SSTableIndex::~SSTableIndex() {
    delete[] keys;
    delete[] offsets;
}

SSTableIndex::SSTableIndex(const SSTableIndex &other)
    : entrCnt(other.entrCnt), byteCnt(other.byteCnt), arrSize(other.arrSize) {
    keys = new uint64_t[arrSize];
    offsets = new uint64_t[arrSize];
    for (int i = 0; i < entrCnt; ++i) {
        keys[i] = other.keys[i];
        offsets[i] = other.offsets[i];
    }
}

void SSTableIndex::add(uint64_t key, uint64_t len) {
    if (entrCnt == arrSize)
        enlargeArray();
    keys[entrCnt] = key;
    offsets[entrCnt] = byteCnt;
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
            return offsets[mid];
        }
    }
    if (keys[left] == key) {
        success = true;
        return offsets[left];
    } else if (keys[right - 1] == key) {
        success = true;
        return offsets[right - 1];
    } else {
        success = false;
        return 0;
    }
}

void SSTableIndex::enlargeArray() {
    uint64_t *oldKeys = keys;
    uint64_t *oldOffsets = offsets;
    arrSize <<= 1;
    keys = new uint64_t[arrSize];
    offsets = new uint64_t[arrSize];
    for (int i = 0; i < entrCnt; ++i) {
        keys[i] = oldKeys[i];
        offsets[i] = oldOffsets[i];
    }
    delete[] oldKeys;
    delete[] oldOffsets;
}