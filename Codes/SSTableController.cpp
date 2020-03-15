#include "SSTableController.h"

void SSTableController::open(const std::string &filename, SSTableIndex *index) {
    io.open(filename, std::ios::binary | std::ios::in | std::ios::out);
    this->index = index;
    itrPos = 0;
}

void SSTableController::close() {
    io.close();
}

void SSTableController::add(uint64_t key, const std::string &value) {
    uint64_t len = value.length();
    io.write(value.c_str(), len);
    index->add(key, len);
}

std::string SSTableController::get(uint64_t k) {
    uint64_t offset = index->offsets[k];
    uint64_t len = (k < index->entrCnt - 1 ? index->offsets[k + 1] : index->byteCnt) - index->offsets[k];
    char *buf = new char[len];
    io.seekg(offset, std::ios::beg);
    io.read(buf, len);
    std::string ret(buf);
    delete[] buf;
    return ret;
}

std::pair<uint64_t, std::string> SSTableController::next() {
    uint64_t key = index->keys[itrPos];
    uint64_t len = (itrPos < index->entrCnt - 1 ? index->offsets[itrPos + 1] : index->byteCnt) - index->offsets[itrPos];
    char *buf = new char[len];
    io.read(buf, len);
    std::string value(buf);
    delete[] buf;
    ++itrPos;
    return std::make_pair(key, value);
}

bool SSTableController::hasNext() {
    return itrPos < index->entrCnt;
}