#include "Entry.h"
#include "KVStore.h"
#include <iostream>
#include <vector>
#include <filesystem>
#include <cstddef>
#include <ctime>

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
    clock_t s, t;

    if (std::filesystem::exists(std::filesystem::path("./data")))
        std::filesystem::remove_all(std::filesystem::path("./data"));

    KVStore store("./data");

    size_t n = 120 * 1024;
    std::cout << "Generating data ... ";
    std::vector<Entry> entries = generate(n, 8192, 8192);
    std::cout << "Done." << std::endl;

    std::vector<double> result(n / 1024);

    int roundCnt = 25;
    for (int k = 0; k < roundCnt; ++k) {
        std::cout << "Processing round " << k << " ... ";
        store.reset();
        for (size_t i = 0; i < n; i += 1024) {
            s = clock();
            for (size_t k = 0; k < 1024; ++k)
                store.put(entries[i + k].key, entries[i + k].value);
            t = clock();
            result[i / 1024] += (double) (t - s) / roundCnt;
        }
        std::cout << "Done." << std::endl;
    }

    for (size_t i = 0; i < n; i += 1024)
        std::cout << result[i / 1024] << " ";
    std::cout << std::endl;

    return 0;
}
