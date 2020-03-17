#ifndef SEARCH_RESULT_H
#define SEARCH_RESULT_H

#include <string>

struct SearchResult {
    bool success;
    std::string value;
    SearchResult(bool success, const std::string &value);
};

#endif