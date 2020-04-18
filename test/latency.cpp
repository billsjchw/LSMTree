#include "KVStore.h"
#include "Entry.h"
#include <vector>
#include <cstdint>
#include <string>
#include <cstddef>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <algorithm>
#include <random>
#include <filesystem>

std::vector<Entry> generate(size_t n, size_t lo, size_t hi) {
    std::vector<Entry> entries;
    std::default_random_engine eng(time(nullptr));
    std::uniform_int_distribution<size_t> distSizeT(lo, hi);
    std::uniform_int_distribution<char> distChar('a', 'z');
    for (uint64_t key = 0; key < n; ++key) {
        std::string value;
        size_t len = distSizeT(eng);
        for (size_t i = 0; i < len; ++i)
            value += distChar(eng);
        entries.emplace_back(key, value);
    }
    return entries;
}

int main() {
    clock_t putTime, seqGetTime, rndGetTime, delTime;
    clock_t s, t;

    if (std::filesystem::exists(std::filesystem::path("./data")))
        std::filesystem::remove_all(std::filesystem::path("./data"));

    KVStore store("./data");

    std::cout << "Generating data ... ";
    std::vector<Entry> entries = generate(128 * 1024, 10000, 20000);
    size_t n = entries.size();
    std::vector<uint64_t> keys;
    for (const Entry &entry : entries)
        keys.push_back(entry.key);
    std::cout << "Done." << std::endl;

    std::cout << "Processing PUT ... ";
    s = clock();
    for (const Entry &entry : entries)
        store.put(entry.key, entry.value);
    t = clock();
    putTime = t - s;
    std::cout << "Done." << std::endl;

    std::cout << "Processing GET (seq) ... ";
    s = clock();
    for (uint64_t key : keys)
        store.get(key);
    t = clock();
    seqGetTime = t - s;
    std::cout << "Done." << std::endl;

    std::vector<uint64_t> rnd = keys;
    std::shuffle(rnd.begin(), rnd.end(), std::default_random_engine(time(nullptr)));
    std::cout << "Processing GET (rnd) ... ";
    s = clock();
    for (uint64_t key : rnd)
        store.get(key);
    t = clock();
    rndGetTime = t - s;
    std::cout << "Done." << std::endl;

    std::cout << "Processing DEL ... ";
    s = clock();
    for (uint64_t key : keys)
        store.del(key);
    t = clock();
    delTime = t - s;
    std::cout << "Done. " << std::endl;

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "PUT: " << (double) putTime / n << "ms" << std::endl;
    std::cout << "GET (seq): " << (double) seqGetTime / n << "ms" << std::endl;
    std::cout << "GET (rnd): " << (double) rndGetTime / n << "ms" << std::endl;
    std::cout << "DEL: " << (double) delTime / n << "ms" << std::endl;

    return 0;
}
