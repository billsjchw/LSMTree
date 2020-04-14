#include "Util.h"
#include <algorithm>

std::vector<Entry> Util::compact(const std::vector<std::vector<Entry>> &inputs) {
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
