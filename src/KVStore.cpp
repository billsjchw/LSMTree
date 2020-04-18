#include "KVStore.h"
#include "SearchResult.h"
#include "Option.h"
#include <string>

KVStore::KVStore(const std::string &dir): KVStoreAPI(dir), disk(dir) {}

KVStore::~KVStore() {
	if (!mem.empty())
		disk.add(mem);
}

void KVStore::put(uint64_t key, const std::string &value) {
	mem.put(key, value);
	if (mem.space() > Option::SST_SPACE) {
		disk.add(mem);
		mem.clear();
	}
}

std::string KVStore::get(uint64_t key) {
	if (mem.contains(key))
		return mem.get(key);
	SearchResult result = disk.search(key, true);
	return result.value;
}

bool KVStore::del(uint64_t key) {
    bool exist;
    if (mem.contains(key))
        exist = !mem.get(key).empty();
    else {
        SearchResult result = disk.search(key, false);
        exist = result.success && result.location.len;
    }
	if (exist)
	    put(key, "");
	return exist;
}

void KVStore::reset() {
	mem.clear();
	disk.clear();
}
