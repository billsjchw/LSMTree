#ifndef UTIL_H
#define UTIL_H

#include "Entry.h"
#include <vector>

namespace Util {
    std::vector<Entry> compact(const std::vector<std::vector<Entry>> &inputs);
}

#endif
