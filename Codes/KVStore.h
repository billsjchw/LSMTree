#include "KVStoreAPI.h"
#include "SkipList.h"
#include "DiskStorage.h"

class KVStore: public KVStoreAPI {
public:
	KVStore(const std::string &dir);
	~KVStore();
	void put(uint64_t key, const std::string &s) override;
	std::string get(uint64_t key) override;
	bool del(uint64_t key) override;
	void reset() override;
private:
	SkipList memTable;
	DiskStorage disk;
};
