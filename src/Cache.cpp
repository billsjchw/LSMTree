#include "Cache.h"
#include "SSTable.h"
#include "Option.h"
#include <string>
#include <sstream>

Cache::Cache(): byteCnt(0) {}

void Cache::complete(SearchResult &result) {
    result.value = result.success ? read(result.location) : "";
}

std::string Cache::read(Location location) {
    std::ostringstream oss;
    oss << location.sst->number() << "-" << location.pos;
    std::string blockId = oss.str();
    std::string block;
    if (hashMap.count(blockId)) {
        block = hashMap[blockId]->second;
        blocks.erase(hashMap[blockId]);
        blocks.emplace_front(blockId, block);
        hashMap[blockId] = blocks.begin();
    } else {
        block = location.sst->loadBlock(location.pos);
        byteCnt += block.size();
        if (byteCnt > Option::CACHE_SPACE) {
            hashMap.erase(blocks.back().first);
            byteCnt -= blocks.back().second.size();
            blocks.pop_back();
        }
        blocks.emplace_front(blockId, block);
        hashMap[blockId] = blocks.begin();
    }
    return block.substr(location.offset, location.len);
}
