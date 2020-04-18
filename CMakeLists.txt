cmake_minimum_required(VERSION 3.15)
project(LSMTree)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-m64)
add_compile_options(-O3)

aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_DIR)
include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(${PROJECT_SOURCE_DIR}/dep/snappy)

add_executable(correctness ./test/correctness.cpp ${SRC_DIR})
add_executable(persistence ./test/persistence.cpp ${SRC_DIR})
add_executable(latency ./test/latency.cpp ${SRC_DIR})
add_executable(throughput ./test/throughput.cpp ${SRC_DIR})

target_link_libraries(correctness snappy)
target_link_libraries(persistence snappy)
target_link_libraries(latency snappy)
target_link_libraries(throughput snappy)
