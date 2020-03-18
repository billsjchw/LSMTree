#include <cstdint>
#include <string>

class KVStoreAPI {
public:
	KVStoreAPI(const std::string &dir) { }
	KVStoreAPI() = delete;
	virtual void put(uint64_t key, const std::string &value) = 0;
	virtual std::string get(uint64_t key) = 0;
	virtual bool del(uint64_t key) = 0;
	virtual void reset() = 0;
};