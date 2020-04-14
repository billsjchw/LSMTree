#include "SSTableId.h"
#include <sstream>

SSTableId::SSTableId(const std::string &dir, uint64_t no)
    : dir(dir), no(no) {}

std::string SSTableId::name() const {
    std::ostringstream oss;
    oss << dir << "/" << no << ".sst";
    return oss.str();
}