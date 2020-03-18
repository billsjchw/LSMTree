#include "kvstore.h"
#include "Utility.h"
#include "SearchResult.h"
#include <string>

KVStore::KVStore(const std::string &dir): KVStoreAPI(dir), disk(dir) {}

KVStore::~KVStore() {
	if (!memTable.empty())
		disk.add(memTable);
}

void KVStore::put(uint64_t key, const std::string &s) {
	memTable.put(key, s);
	if (memTable.space() > Utility::SSTABLE_BOUND) {
		disk.add(memTable);
		memTable.clear();
	}
}

std::string KVStore::get(uint64_t key) {
	if (memTable.contains(key))
		return memTable.get(key);
	SearchResult res = disk.search(key);
	return res.success ? res.value : "";
}

bool KVStore::del(uint64_t key) {
	bool ret = memTable.contains(key) || disk.search(key).success;
	put(key, "");
	return ret;
}

void KVStore::reset() {
	memTable.clear();
	disk.clear();
}
