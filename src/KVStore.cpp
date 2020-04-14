#include "KVStore.h"
#include "Util.h"
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
	SearchResult res = disk.search(key);
	return res.success ? res.value : "";
}

bool KVStore::del(uint64_t key) {
	bool ret = get(key) != "";
	put(key, "");
	return ret;
}

void KVStore::reset() {
	mem.clear();
	disk.clear();
}
