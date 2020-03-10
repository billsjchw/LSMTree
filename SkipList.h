#ifndef SKIP_LIST_H
#define SKIP_LIST_H

#include <cstdint>
#include <string>
#include <random>

class SkipList {
public:
    explicit SkipList();
    ~SkipList();
    std::string get(uint64_t key);
    void put(uint64_t key, const std::string & value);
    bool del(uint64_t key);
    bool contains(uint64_t key);
    size_t size();
    bool empty();
    void clear();
    size_t space();
private:
    struct Tower {
        uint64_t key;
        std::string value;
        Tower **prevs;
        Tower **nexts;
        size_t height;
        explicit Tower(uint64_t key, const std::string & value, size_t height);
        Tower() = delete;
        ~Tower();
    };
    Tower *head, *tail;
    size_t totalEntries;
    size_t totalBytes;
    std::default_random_engine engine;
    std::uniform_int_distribution<int> dist;
    void init();
    Tower *find(uint64_t key);
    void enlargeHeight(size_t height);
};

#endif