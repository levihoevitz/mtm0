# The first line states the minimum CMake version to use with this project
# and is requires by CMake.
cmake_minimum_required(VERSION 3.0.0)

# Set <PROJNAME> to be your project name. We set C here as the target language.
project(mtm0 VERSION 0.1.0 LANGUAGES C)

# If you want to see "under the hood" of CMake, uncomment this:
# set(CMAKE_VERBOSE_MAKEFILE ON)

# Define variables containing compiler flags for gcc
set(MTM_FLAGS_DEBUG "-std=c99 --pedantic-errors -Wall -Werror")
set(MTM_FLAGS_RELEASE "${MTM_FLAGS_DEBUG} -DNDEBUG")

# Set the flags for gcc
set(CMAKE_C_FLAGS ${MTM_FLAGS_DEBUG})

#ex0
add_executable(part1 ex0/part1/part1.c)
add_executable(mtm_buggy ex0/part2/mtm_buggy.c ex0/part2/mtm_buggy.c)

#ex1
add_executable(2_1 ex1/dry/2_1.c ex1/set/set.h ex1/set/matamikya_print.h ex1/set/list.h ex1/set/matamikya_print.c ex1/set/amount_set_str.h ex1/set/amount_set.h ex1/set/amount_set_str.c ex1/set/amount_set.c ex1/set/matamikya.h ex1/set/matamikya.c ex1/set/product.h ex1/set/product.c ex1/set/orders.h ex1/set/orders.c)
add_executable(2_2 ex1/dry/2_2.c)