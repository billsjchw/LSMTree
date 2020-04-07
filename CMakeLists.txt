cmake_minimum_required(VERSION 3.15)
project(LSMTree)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(
        correctness
        correctness.cpp
        DiskStorage.cpp
        DiskStorage.h
        Entry.cpp
        Entry.h
        KVStore.cpp
        KVStore.h
        KVStoreAPI.h
        LevelNonZero.cpp
        LevelNonZero.h
        LevelZero.cpp
        LevelZero.h
        SearchResult.cpp
        SearchResult.h
        SkipList.cpp
        SkipList.h
        SSTable.cpp
        SSTable.h
        SSTableId.cpp
        SSTableId.h
        Test.h
        Utility.cpp
        Utility.h
)

add_executable(
        persistence
        persistence.cpp
        DiskStorage.cpp
        DiskStorage.h
        Entry.cpp
        Entry.h
        KVStore.cpp
        KVStore.h
        KVStoreAPI.h
        LevelNonZero.cpp
        LevelNonZero.h
        LevelZero.cpp
        LevelZero.h
        SearchResult.cpp
        SearchResult.h
        SkipList.cpp
        SkipList.h
        SSTable.cpp
        SSTable.h
        SSTableId.cpp
        SSTableId.h
        Test.h
        Utility.cpp
        Utility.h
)
