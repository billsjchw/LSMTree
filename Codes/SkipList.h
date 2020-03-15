#ifndef SKIP_LIST_H
#define SKIP_LIST_H

#include <cstdint>
#include <cstddef>
#include <string>
#include <random>
#include <utility>

class SkipList {
public:
    class Iterator;
    explicit SkipList();
    ~SkipList();
    std::string get(uint64_t key) const;
    void put(uint64_t key, const std::string & value);
    bool del(uint64_t key);
    bool contains(uint64_t key) const;
    Iterator iterator() const;
    size_t size() const;
    bool empty() const;
    void clear();
    size_t space() const;
private:
    struct Tower;
    Tower *head, *tail;
    size_t totalEntries;
    size_t totalBytes;
    std::default_random_engine engine;
    std::uniform_int_distribution<int> dist;
    void init();
    Tower *find(uint64_t key) const;
    void enlargeHeight(size_t height);
};

struct SkipList::Tower {
    uint64_t key;
    std::string value;
    Tower **prevs;
    Tower **nexts;
    size_t height;
    explicit Tower(uint64_t key, const std::string & value, size_t height);
    Tower() = delete;
    ~Tower();
};

class SkipList::Iterator {
public:
    std::pair<uint64_t, std::string> next();
    bool hasNext() const;
private:
    Tower *tower;
    Iterator(Tower *tower);
    Iterator() = delete;
friend SkipList;
};

#endif