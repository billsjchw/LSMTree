#include "Location.h"

Location::Location(const SSTable *sst, uint64_t pos, uint64_t offset, uint64_t len)
    : sst(sst), pos(pos), offset(offset), len(len) {}
