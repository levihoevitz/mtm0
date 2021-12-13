cmake_minimum_required(VERSION 3.0.0)
project(ex1 VERSION 0.1.0 LANGUAGES C)

set(MTM_FLAGS_DEBUG "-std=c99 --pedantic-errors -Wall -Werror")
set(MTM_FLAGS_RELEASE "${MTM_FLAGS_DEBUG} -DNDEBUG")

set(CMAKE_C_FLAGS ${MTM_FLAGS_DEBUG})

add_executable(matamikya
        ex1/set/matamikya.c
        ex1/set/matamikya_print.c
        ex1/set/product.c
        ex1/set/orders.c
        ex1/set/tests/matamikya_tests.c
        ex1/set/tests/matamikya_main.c)

target_link_libraries(matamikya -L${CMAKE_SOURCE_DIR} -lmtm -las)