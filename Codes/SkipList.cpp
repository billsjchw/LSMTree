#include "SkipList.h"
#include <cstring>

SkipList::SkipList(): dist(0, 1) {
    init();
}

SkipList::~SkipList() {
    clear();
    delete head;
    delete tail;
}

void SkipList::clear() {
    for (Tower *tower = head; tower != nullptr;) {
        Tower *next = tower->nexts[0];
        delete tower;
        tower = next;
    }
    init();
}

std::string SkipList::get(uint64_t key) const {
    Tower *tower = find(key);
    return tower != head && tower->key == key ? tower->value : "";
}

void SkipList::put(uint64_t key, const std::string &value) {
    Tower *prev = find(key);
    if (prev != head && prev->key == key) {
        prev->value = value;
        return;
    }
    size_t height = 1;
    while (dist(engine))
        ++height;
    if (head->height < height + 1)
        enlargeHeight(height + 1);
    Tower *tower = new Tower(key, value, height);
    for (size_t lvl = 0; lvl < height; ++lvl) {
        tower->prevs[lvl] = prev;
        tower->nexts[lvl] = prev->nexts[lvl];
        prev->nexts[lvl]->prevs[lvl] = tower;
        prev->nexts[lvl] = tower;
        while (lvl + 1 >= prev->height)
            prev = prev->prevs[lvl];
    }
    ++totalEntries;
    totalBytes += value.length();
}

bool SkipList::del(uint64_t key) {
    Tower *tower = find(key);
    if (tower == head || tower->key != key)
        return false;
    size_t height = tower->height;
    for (size_t lvl = 0; lvl < height; ++lvl) {
        tower->prevs[lvl]->nexts[lvl] = tower->nexts[lvl];
        tower->nexts[lvl]->prevs[lvl] = tower->prevs[lvl];
    }
    --totalEntries;
    totalBytes -= tower->value.length();
    delete tower;
    return true;
}

bool SkipList::contains(uint64_t key) const {
    Tower *tower = find(key);
    return tower != head && tower->key == key;
}

SkipList::Iterator SkipList::iterator() const {
    return {head->nexts[0]};
}

size_t SkipList::size() const {
    return totalEntries;
}

bool SkipList::empty() const {
    return totalEntries == 0;
}

size_t SkipList::space() const {
    return totalBytes;
}

void SkipList::init() {
    head = new Tower(0, "", 1);
    tail = new Tower(UINT64_MAX, "", 1);
    head->prevs[0] = nullptr;
    head->nexts[0] = tail;
    tail->prevs[0] = head;
    tail->nexts[0] = nullptr;
    totalBytes = 0;
    totalEntries = 0;
}

SkipList::Tower *SkipList::find(uint64_t key) const {
    Tower *tower = head;
    size_t height = head->height;
    for (size_t i = 1; i <= height; ++i) {
        while (tower->key <= key)
            tower = tower->nexts[height - i];
        tower = tower->prevs[height - i];
    }
    return tower;
}

void SkipList::enlargeHeight(size_t height) {
    size_t oldHeight = head->height;
    head->height = height;
    tail->height = height;
    Tower **oldHeadPrevs = head->prevs;
    Tower **oldHeadNexts = head->nexts;
    Tower **oldTailPrevs = tail->prevs;
    Tower **oldTailNexts = tail->nexts;
    head->prevs = new Tower*[height];
    head->nexts = new Tower*[height];
    tail->prevs = new Tower*[height];
    tail->nexts = new Tower*[height];
    for (size_t lvl = 0; lvl < height; ++lvl)
        head->prevs[lvl] = tail->nexts[lvl] = nullptr;
    for (size_t lvl = 0; lvl < oldHeight; ++lvl) {
        head->nexts[lvl] = oldHeadNexts[lvl];
        tail->prevs[lvl] = oldTailPrevs[lvl];
    }
    for (size_t lvl = oldHeight; lvl < height; ++lvl) {
        head->nexts[lvl] = tail;
        tail->prevs[lvl] = head;
    }
    delete[] oldHeadPrevs;
    delete[] oldHeadNexts;
    delete[] oldTailPrevs;
    delete[] oldTailNexts;
}

SkipList::Tower::Tower(uint64_t key, const std::string &value, size_t height)
    : key(key), value(value), height(height) {
    prevs = new Tower*[height];
    nexts = new Tower*[height];
}

SkipList::Tower::~Tower() {
    delete[] prevs;
    delete[] nexts;
}

SkipList::Iterator::Iterator(const Iterator &itr): tower(itr.tower) {}

std::pair<uint64_t, std::string> SkipList::Iterator::next() {
    std::pair<uint64_t, std::string> entry = std::make_pair(tower->key, tower->value);
    if (tower->nexts[0] != nullptr)
        tower = tower->nexts[0];
    return entry;
}

bool SkipList::Iterator::hasNext() const {
    return tower->nexts[0] != nullptr;
}