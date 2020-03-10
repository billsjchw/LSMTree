#include "kvstore.h"

KVStore::KVStore(const std::string &dir): KVStoreAPI(dir) {
}

KVStore::~KVStore() {
}

/**
 * Insert/Update the key-value pair.
 * No return values for simplicity.
 */
void KVStore::put(uint64_t key, const std::string &value) {
	memTable.put(key, value);
}
/**
 * Returns the (string) value of the given key.
 * An empty string indicates not found.
 */
std::string KVStore::get(uint64_t key)
{
	return memTable.get(key);
}
/**
 * Delete the given key-value pair if it exists.
 * Returns false iff the key is not found.
 */
bool KVStore::del(uint64_t key) {
	return memTable.del(key);
}

/**
 * This resets the kvstore. All key-value pairs should be removed,
 * including memtable and all sstables files.
 */
void KVStore::reset() {
	memTable.clear();
}
