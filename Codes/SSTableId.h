#ifndef SSTABLE_ID_H
#define SSTABLE_ID_H

#include <string>
#include <cstdint>

struct SSTableId {
    std::string dir;
    uint64_t no;
    SSTableId(const std::string &dir, uint64_t no);
    std::string name() const;
};

#endif