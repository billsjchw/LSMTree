#include "BlockCache.h"
#include "SSTable.h"
#include "Option.h"
#include <string>
#include <sstream>

BlockCache::BlockCache(): byteCnt(0) {}

void BlockCache::complete(SearchResult &result) {
    result.value = result.success ? read(result.location) : "";
}

std::string BlockCache::read(Location location) {
    if (!Option::BLOCK_CACHE)
        return location.sst->loadBlock(location.pos).substr(location.offset, location.len);
    std::ostringstream oss;
    oss << location.sst->number() << "-" << location.pos;
    std::string blockId = oss.str();
    std::string block;
    if (hashMap.count(blockId)) {
        block = hashMap[blockId]->second;
        linkedList.erase(hashMap[blockId]);
        linkedList.emplace_front(blockId, block);
        hashMap[blockId] = linkedList.begin();
    } else {
        block = location.sst->loadBlock(location.pos);
        byteCnt += block.size();
        if (byteCnt > Option::BLOCK_CACHE_SPACE) {
            hashMap.erase(linkedList.back().first);
            byteCnt -= linkedList.back().second.size();
            linkedList.pop_back();
        }
        linkedList.emplace_front(blockId, block);
        hashMap[blockId] = linkedList.begin();
    }
    return block.substr(location.offset, location.len);
}
