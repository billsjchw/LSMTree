#ifndef SEARCH_RESULT_H
#define SEARCH_RESULT_H

#include "Location.h"
#include <string>

struct SearchResult {
    bool success;
    Location location;
    std::string value;
    SearchResult(bool success);
    SearchResult(bool success, Location location);
};

#endif