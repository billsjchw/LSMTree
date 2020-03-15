#ifndef SSTABLE_CONTROLLER_H
#define SSTABLE_CONTROLLER_H

#include "SSTableIndex.h"
#include <fstream>
#include <string>
#include <utility>

class SSTableController {
public:
    void open(const std::string &filename, SSTableIndex *index);
    void close();
    void add(uint64_t key, const std::string &value);
    std::string get(uint64_t k);
    std::pair<uint64_t, std::string> next();
    bool hasNext();
private:
    std::fstream io;
    SSTableIndex *index;
    uint64_t itrPos;
};

#endif