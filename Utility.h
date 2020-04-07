#include "Entry.h"
#include <vector>
#include <cstdint>

namespace Utility {
    extern const uint64_t LEVEL_NON_ZERO_NUM;
    extern const uint64_t SSTABLE_BOUND;
    extern const uint64_t LEVEL_ZERO_BOUND;
    extern const uint64_t LEVEL_NON_ZERO_BOUNDS[];
    extern const char *const LEVEL_ZERO_NAME;
    extern const char *const LEVEL_NON_ZERO_NAMES[];
    std::vector<Entry> compact(std::vector<std::vector<Entry>> &&inputs);
};