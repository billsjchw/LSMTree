#include "SkipList.h"
#include <iostream>

int main() {
    SkipList skipList;

    skipList.put(1, "xyz");
    skipList.put(3, "abc");
    skipList.put(2, "ijk");
    SkipList::Iterator itr = skipList.iterator();
    while (itr.hasNext())
        std::cout << itr.next().first << std::endl;

    return 0;
}