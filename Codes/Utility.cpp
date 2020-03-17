#include "Utility.h"
#include <algorithm>

const uint64_t Utility::LEVEL_NON_ZERO_NUM = 6;

const uint64_t Utility::SSTABLE_BOUND = (uint64_t) 2 * 1024 * 1024;

const uint64_t Utility::LEVEL_ZERO_BOUND = (uint64_t) 8 * 1024 * 1024;

const uint64_t Utility::LEVEL_NON_ZERO_BOUNDS[] = {
    (uint64_t) 64 * 1024 * 1024,
    (uint64_t) 512 * 1024 * 1024,
    (uint64_t) 4096 * 1024 * 1024,
    (uint64_t) 32768 * 1024 * 1024,
    (uint64_t) 262144 * 1024 * 1024,
    UINT64_MAX
};

const char *const Utility::LEVEL_ZERO_NAME = "/L0";

const char *const Utility::LEVEL_NON_ZERO_NAMES[] = {"/L1", "/L2", "/L3", "/L4", "/L5", "/L6"};

std::vector<Entry> Utility::compact(const std::vector<std::vector<Entry>> &inputs) {
    size_t n = inputs.size();
    std::vector<Entry> ret;
    std::vector<size_t> poses(n);
    size_t nonEmpty = 0;
    for (size_t i = 0; i < n; ++i)
        if (!inputs[i].empty())
            ++nonEmpty;
    while (nonEmpty) {
        uint64_t key = UINT64_MAX;
        for (size_t i = 0; i < n; ++i)
            if (poses[i] < inputs[i].size())
                key = std::min(key, inputs[i][poses[i]].key);
        std::string value;
        for (size_t i = 0; i < n; ++i)
            if (poses[i] < inputs[i].size() && inputs[i][poses[i]].key == key) {
                value = inputs[i][poses[i]++].value;
                if (poses[i] == inputs[i].size())
                    --nonEmpty;
            }
        ret.emplace_back(key, value);
    }
    return ret;
}