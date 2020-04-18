#ifndef CACHE_H
#define CACHE_H

#include "SearchResult.h"
#include "Location.h"
#include <string>
#include <list>
#include <utility>
#include <unordered_map>
#include <cstdint>

class BlockCache {
public:
    explicit BlockCache();
    void complete(SearchResult &result);
private:
    uint64_t byteCnt;
    std::list<std::pair<std::string, std::string>> linkedList;
    std::unordered_map<std::string, std::list<std::pair<std::string, std::string>>::iterator> hashMap;
    std::string read(Location location);
};

#endif
