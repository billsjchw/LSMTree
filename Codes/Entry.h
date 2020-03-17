#ifndef ENTRY_H
#define ENTRY_H

#include <cstdint>
#include <string>

struct Entry {
    uint64_t key;
    std::string value;
    Entry(uint64_t key, const std::string &value);
};

#endif