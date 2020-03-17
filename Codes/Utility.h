#include "Entry.h"
#include <vector>

namespace Utility {
    extern uint64_t BOUND;
    std::vector<Entry> compact(const std::vector<std::vector<Entry>> &inputs);
};