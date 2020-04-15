#ifndef LOCATION_H
#define LOCATION_H

#include <cstdint>

class SSTable;

struct Location {
    const SSTable *sst;
    uint64_t pos;
    uint64_t offset;
    uint64_t len;
    Location() = default;
    Location(const SSTable *sst, uint64_t pos, uint64_t offset, uint64_t len);
};

#endif
