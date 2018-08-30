cmake_minimum_required(VERSION 3.0)
project(lazy_val)
set(VERSION 0.1.0)

enable_testing()

find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

add_executable(lazy_val_tests tests.cpp lazy_val.h)

target_link_libraries(lazy_val_tests ${GTEST_BOTH_LIBRARIES})
