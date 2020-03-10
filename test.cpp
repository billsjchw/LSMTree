#include "SkipList.h"
#include <iostream>

int main() {
    SkipList skipList;

    skipList.put(1, "xyz");
    skipList.put(1, "abc");
    skipList.put(2, "ijk");
    skipList.del(1);
    std::cout << skipList.get(2) << std::endl;

    return 0;
}